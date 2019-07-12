#include <condition_variable>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <boost/asio.hpp>
//using namespace std;

#include "constants.h"
#include "Demodulator.h"
#include "Entry.h"
#include "loguru.hpp"
#include "messages.h"
#include "ServerManager.h"
#include "ScannerSM.h"
#include "SystemList.h"

//enum {
//	SYSTEM_CONTROL,
//	SCANNER_SM,
//	DEMOD,
//	SERVER_MAN,
//	AUDIO_MAN,
//	CLIENT,
//};

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
		loguru::set_thread_name("MessageManager");
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
		_connectionManager.start();
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

void sigIntHandler(int signal){
	LOG_F(INFO, "Stop triggered by interrupt");
	sysRun = false;
}

void sigTermHandler(int signal){
	LOG_F(WARNING, "Terminating - resources may not be properly released");
	sysRun = false;
	exit(1);
}

void setDemodulator(DemodInterface* demod) {
	DCHECK_F(demod != nullptr);
	Entry::demod = demod;
}

void runIO(){
	loguru::set_thread_name("IO Service");
	DLOG_F(2, "Starting IO service");
	io_service.run();
}

void exit(int code){
	LOG_F(INFO, "PiScan stopped, exiting");

	io_service.stop();
	ioThread.join();

	if(activeMessages > 0)
		DLOG_F(WARNING, "Memory leak: %i messages not deleted!", activeMessages);

	std::exit(code);
}

}

using namespace piscan;

int main(int argc, char **argv) {
	loguru::init(argc, argv);
	loguru::add_file(LOG_PATH, loguru::Truncate, LOG_VERBOSITY);
	//loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
	loguru::g_preamble_file = false;

	signal(SIGINT, sigIntHandler);
	signal(SIGTERM, sigTermHandler);

	LOG_F(INFO, "Starting PiScan");

	messageManager.setReceiver(SYSTEM_CONTROL, &sysControl);
	messageManager.setReceiver(SCANNER_SM, &scanner);
	messageManager.setReceiver(DEMOD, &demod);
	messageManager.setReceiver(SERVER_MAN, &connectionManager);
	//messageManager.setReceiver(AUDIO_MAN, &audioControl);

	setDemodulator(&demod);

	

	try {
		messageManager.start();
		sysControl.start();
		ioThread = std::thread(runIO);
	} catch (std::exception& e) {
		LOG_F(ERROR, e.what());
		goto sysexit;
	}



	while(sysRun)
		usleep(100000);

	sysexit:
	try {
		sysControl.stop();
		messageManager.stop(true);
	} catch (std::exception& e) {
		LOG_F(ERROR, e.what());
	}

	piscan::exit(0);

	return 0;
}

