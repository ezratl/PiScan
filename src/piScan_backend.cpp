#include <condition_variable>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <boost/asio.hpp>
//using namespace std;

#include "PiScan.h"
#include "constants.h"
#include "Configuration.h"
#include "Demodulator.h"
#include "Entry.h"
#include "loguru.hpp"
#include "messages.h"
#include "ServerManager.h"
#include "ScannerSM.h"
#include "SystemList.h"
#include "threadname.h"

//enum {
//	SYSTEM_CONTROL,
//	SCANNER_SM,
//	DEMOD,
//	SERVER_MAN,
//	AUDIO_MAN,
//	CLIENT,
//};

#define IO_THREAD_NAME	"IO Service"

#define SCANNER_FLAG	0x01
#define CONNMGR_FLAG	0x02
#define DEMOD_FLAG		0x04
#define AUDIO_FLAG		0x08

//#define ALL_FLAG		0x0F
#define ALL_FLAG		0x07

//using namespace piscan;
namespace piscan {

static bool sysRun;

class MessageManager : public MessageReceiver {
public:
	MessageManager() {};
	~MessageManager() {};

	void setReceiver(unsigned char id, MessageReceiver* ptr){
		if(id < MESSAGE_RECEIVERS){
			_receivers[id] = ptr;
		}
		else{
			DLOG_F(ERROR, "Invalid receiver ID: %d", id);
		}
	}

	void start() {
		_run = true;
		_workThread = std::thread(&MessageManager::_handlerThreadFunc, this);
	}

	void stop(bool block) {
		_run = false;
		std::unique_lock<std::mutex> lock(_msgMutex, std::defer_lock);
		if (lock.try_lock()) {
			_msgAvailable = true;
			lock.unlock();
			_cv.notify_one();
		}
		if(block)
			_workThread.join();
	}
private:
	moodycamel::ConcurrentQueue<std::shared_ptr<Message>> _queue;
	std::thread _workThread;
	std::mutex _msgMutex;
	std::condition_variable _cv;
	bool _msgAvailable = false;
	bool _run = false;
	MessageReceiver* _receivers[MESSAGE_RECEIVERS];

	void _handlerThreadFunc(void){
		setThreadName("MessageManager");
		LOG_F(2, "MessageManager started");

		while(_run){
			std::unique_lock<std::mutex> lock(_msgMutex);
			_cv.wait(lock, [this]{return this->_msgAvailable;});

			std::shared_ptr<Message> message;
			while(_queue.try_dequeue(message)){
				DCHECK_F(message != nullptr);
				DCHECK_F(message->destination != message->source);
				DLOG_F(7, "Message receive | dst:%d | src:%d", message->destination, message->source);

				if(message->destination < MESSAGE_RECEIVERS){
					MessageReceiver* receiver = _receivers[message->destination];
					if(receiver == nullptr){
						DLOG_F(ERROR, "Message bound for null receiver | dst:%d | src:%d", message->destination, message->source);
						
					}
					else{
						receiver->giveMessage(message);
					}
				}
				else{
					DLOG_F(ERROR, "Message has invalid destination | dst:%d | src:%d", message->destination, message->source);
					
				}

				_msgAvailable = false;
			}

			lock.unlock();

		}
		LOG_F(2, "MessageManager stopped");
	}

	void giveMessage(std::shared_ptr<Message> message){
		DLOG_F(7, "Queueing message");
		if(message->destination > MESSAGE_RECEIVERS){
			DLOG_F(ERROR, "Message has invalid destination | dst:%d | src:%d", message->destination, message->source);
			
			return;
		}
		_queue.enqueue(message);
		std::unique_lock<std::mutex> lock(_msgMutex, std::defer_lock);
		if (lock.try_lock()) {
			_msgAvailable = true;
			lock.unlock();
			_cv.notify_one();
		}
	}
};

class SystemController : public MessageReceiver {
public:
	SystemController(MessageReceiver& central, SystemList& syslist,
			ScannerSM& scan, ServerManager& conmgr, Demodulator& dm) :
			_centralQueue(central), _systemList(syslist), _scanner(scan), _connectionManager(
					conmgr), _demod(dm), _flagLock(_flagsMutex, std::defer_lock) {
		//_flagLock(_flagsMutex);
		//_flagLock.unlock();
	}
	~SystemController() {};

	void start(){
		LOG_F(1, "System Control start");
		_scanner.start();
		//_connectionManager.start();
		_demod.start();

		/* let a stop call break the loop */
		while(sysRun){
			//_flagLock.lock();
			if(_activeModules == ALL_FLAG){
				//_flagLock.unlock();
				break;
			}
			//_flagLock.unlock();
		}

		LOG_F(INFO, "System initialized");

		_connectionManager.allowConnections();
		_scanner.startScan();

		sysRun = true;
	}

	void stop(){
		LOG_F(INFO, "Stopping system");
		_scanner.stopScanner();
		_centralQueue.giveMessage(std::make_shared<ServerMessage>(SYSTEM_CONTROL, ServerMessage::STOP, nullptr));
		_demod.stop();

		while(1){
			//_flagLock.lock();
			if(_activeModules == 0){
				//_flagLock.unlock();
				break;
			}
			//_flagLock.unlock();
		}

		LOG_F(2, "All modules stopped");
	}

private:
	MessageReceiver& _centralQueue;
	SystemList& _systemList;
	ScannerSM& _scanner;
	ServerManager& _connectionManager;
	Demodulator& _demod;

	std::mutex _flagsMutex;
	std::unique_lock<std::mutex> _flagLock;
	unsigned char _activeModules = 0;

	void giveMessage(std::shared_ptr<Message> message){
		auto msg = std::dynamic_pointer_cast<ControllerMessage>(message);

		ClientRequest* request;
		switch(msg->type){
		case ControllerMessage::CLIENT_REQUEST:
			request = reinterpret_cast<ClientRequest*>(message->pData);
			processRequest(*request);
			break;

		case ControllerMessage::NOTIFY_READY:
			switch(msg->source){
			case SCANNER_SM:
				_activeModules |= SCANNER_FLAG;
				DLOG_F(8, "scanner started");
				break;
			case DEMOD:
				_activeModules |= DEMOD_FLAG;
				DLOG_F(8, "demod started");
				break;
			case SERVER_MAN:
				_activeModules |= CONNMGR_FLAG;
				DLOG_F(8, "conmgr started");
				break;
			case AUDIO_MAN:
				_activeModules |= AUDIO_FLAG;
				break;
			default:
				break;
			}
			break;

		case ControllerMessage::NOTIFY_STOPPED:
			switch (msg->source) {
			case SCANNER_SM:
				DLOG_F(8, "scanner stopped");
				_activeModules &= ~SCANNER_FLAG;
				break;
			case DEMOD:
				_activeModules &= ~DEMOD_FLAG;
				DLOG_F(8, "demod stopped");
				break;
			case SERVER_MAN:
				DLOG_F(8, "conmgr stopped");
				_activeModules &= ~CONNMGR_FLAG;
				break;
			case AUDIO_MAN:
				_activeModules &= ~AUDIO_FLAG;
				break;
			default:
				break;
			}
			break;
		}

	}

	void processRequest(ClientRequest& request){

		switch(request.rqInfo.type){
		case SYSTEM_FUNCTION:
			switch(request.rqInfo.subType){
			case SYSTEM_STOP:
				LOG_F(1, "Stop request from client %i", request.source);
				sysRun = false;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}

		delete &request;
	}
};

static boost::asio::io_service io_service;
static std::thread ioThread;
static MessageManager messageManager;
static SystemList scanSystems;
static ScannerSM scanner(messageManager, scanSystems);
static ServerManager connectionManager(io_service, messageManager);
static Demodulator demod(messageManager);
static SystemController sysControl(messageManager, scanSystems, scanner, connectionManager, demod);

static std::atomic_bool steadyState(false);

void terminate(){
	LOG_F(WARNING, "Terminating - resources may not be properly released");
	std::terminate();
}

void sigTermHandler(int signal){

	sysRun = false;
	//exit(1);
	piscan::terminate();
}

void sigIntHandler(int signal){
	LOG_F(INFO, "Stop triggered by interrupt");

	if(!sysRun)
		piscan::terminate();

	sysRun = false;
}

void setDemodulator(DemodInterface* demod) {
	DCHECK_F(demod != nullptr);
	Entry::demod = demod;
}

void runIO(){
	setThreadName(IO_THREAD_NAME);
	DLOG_F(2, "Starting IO service");
	io_service.run();
}

void exit(int code){
	LOG_F(INFO, "PiScan stopped, exiting");

	io_service.stop();
	ioThread.join();

	
	std::exit(code);
}

bool app::stopSystem(){
	if(steadyState.load()){
		return true;
	}
	return false;
}

void app::startScan(){
	scanner.startScan();
}

void app::holdScan(std::vector<int> index){
	scanner.holdScan(index);
}

void app::stopScanner(){
	scanner.stopScanner();
}

void app::manualEntry(app::ManualEntryData* freq){
	scanner.manualEntry(freq);
}

ScannerContext app::getScannerContext(){
	return scanner.getCurrentContext();
}

void app::setTunerGain(float gain){
	demod.setTunerGain(gain);
}

void app::setDemodSquelch(float level){
	demod.setSquelch(level);
}

DemodContext app::getDemodContext(){
	return DemodContext(demod.getTunerGain(), demod.getSquelch());
}

void app::squelchBreak(bool mute){
	demod.squelchBreak(mute);
}

long long app::getTunerSampleRate() {
	return demod.getTunerSampleRate();
}

const SystemInfo app::getSystemInfo(){
	SystemInfo info = {
			.version = "debug",
			.buildNumber = 0,
			.squelchRange = {-100, 0},
			.supportedModulations = {"FM", "AM"},
	};
	return info;
}

void app::scannerContextUpdate(ScannerContext ctx){
	connectionManager.giveMessage(make_shared<ServerMessage>(SCANNER_SM, ServerMessage::CONTEXT_UPDATE, new ScannerContext(ctx)));
}

void app::demodContextUpdate(DemodContext ctx){
	connectionManager.giveMessage(make_shared<ServerMessage>(DEMOD, ServerMessage::CONTEXT_UPDATE, new DemodContext(ctx)));
}

void app::signalLevelUpdate(int level){
	connectionManager.giveMessage(make_shared<ServerMessage>(DEMOD, ServerMessage::SIGNAL_LEVEL, new int(level)));
}

}

using namespace piscan;

int main(int argc, char **argv) {
	loguru::init(argc, argv);

	loguru::g_preamble_file = false;

	signal(SIGINT, sigIntHandler);
	signal(SIGTERM, sigTermHandler);

	LOG_F(INFO, "Starting PiScan");

	Configuration& config = Configuration::getConfig();
	bool useDebugConsole = false;
	bool spawnClient = false;

	int logVerbosity = config.getGeneralConfig().logfileVerbosity;

	int c;
	while((c = getopt(argc,argv,"dp:f:l")) != -1){
		switch(c){
			case 'd':
				useDebugConsole = true;
				break;
			case 'p':
				if(optarg)
					config.setWorkingDirectory(std::string(optarg));
				break;
			case 'f':
				if(optarg)
					logVerbosity = std::atoi(optarg);
				break;
			case 'l':
				spawnClient = true;
				break;
		}
	}

	
	config.loadConfig();
	config.loadState();

	loguru::add_file(config.getDatedLogPath().c_str(), loguru::Truncate, logVerbosity);
	loguru::add_file(config.getLatestLogPath().c_str(), loguru::Truncate, logVerbosity);

	messageManager.setReceiver(SYSTEM_CONTROL, &sysControl);
	messageManager.setReceiver(SCANNER_SM, &scanner);
	messageManager.setReceiver(DEMOD, &demod);
	messageManager.setReceiver(SERVER_MAN, &connectionManager);
	//messageManager.setReceiver(AUDIO_MAN, &audioControl);

	setDemodulator(&demod);

	//connectionManager.useDebugServer(useDebugConsole);

	try {
		messageManager.start();
		//sysControl.start();

		{
			scanner.start();
			connectionManager.start(useDebugConsole, spawnClient);
			demod.start();

			/*while(sysRun){
			//_flagLock.lock();
				if(_activeModules == ALL_FLAG){
					//_flagLock.unlock();
					break;
				}
			//_flagLock.unlock();
			}*/

			LOG_F(4, "scanner wait");
			scanner.waitReady();
			LOG_F(4, "server wait");
			connectionManager.waitReady();
			LOG_F(4, "demod wait");
			demod.waitReady();

			connectionManager.allowConnections();
			scanner.startScan();

			sysRun = true;
		}

		ioThread = std::thread(runIO);
	} catch (std::exception& e) {
		LOG_F(ERROR, e.what());
		goto sysexit;
	}

	steadyState.store(true);
	LOG_F(INFO, "System initialized");

	while(sysRun)
		usleep(100000);

	sysexit:
	steadyState.store(false);
	try {
		//sysControl.stop();

		{
			LOG_F(INFO, "Stopping system");
		scanner.stopScanner();
		//_centralQueue.giveMessage(std::make_shared<ServerMessage>(SYSTEM_CONTROL, ServerMessage::STOP, nullptr));
		connectionManager.stop();
		demod.stop();

		/*while(1){
			//_flagLock.lock();
			if(_activeModules == 0){
				//_flagLock.unlock();
				break;
			}
			//_flagLock.unlock();
		}*/

		LOG_F(4, "scanner wait");
		scanner.waitDeinit();
		LOG_F(4, "server wait");
		connectionManager.waitDeinit();
		LOG_F(4, "demod wait");
		demod.waitDeinit();

		LOG_F(2, "All modules stopped");
		}

		messageManager.stop(true);
	} catch (std::exception& e) {
		LOG_F(ERROR, e.what());
	}

	config.saveState();
	config.saveConfig();

	piscan::exit(0);

	return 0;
}

