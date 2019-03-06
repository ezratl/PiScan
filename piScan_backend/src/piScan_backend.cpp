#include <condition_variable>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

#include "constants.h"
#include "Demodulator.h"
#include "Entry.h"
#include "loguru.hpp"
#include "messages.h"
#include "ServerManager.h"
#include "ScannerSM.h"
#include "SystemList.h"

#include "demo_system.h"

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
		std::unique_lock<std::mutex> lock(_msgMutex);
		_msgAvailable = true;
		lock.unlock();
		_cv.notify_one();
		if(block)
			_workThread.join();
	}
private:
	moodycamel::ConcurrentQueue<Message*> _queue;
	std::thread _workThread;
	std::mutex _msgMutex;
	std::condition_variable _cv;
	bool _msgAvailable = false;
	bool _run = false;
	MessageReceiver* _receivers[MESSAGE_RECEIVERS];

	void _handlerThreadFunc(void){
		LOG_F(2, "MessageManager started");

		while(_run){
			std::unique_lock<std::mutex> lock(_msgMutex);
			_cv.wait(lock, [this]{return this->_msgAvailable;});

			Message* message;
			if(_queue.try_dequeue(message)){
				DCHECK_F(message != nullptr);
				DCHECK_F(message->destination != message->source);
				DLOG_F(9, "Message receive | dst:%d | src:%d", message->destination, message->source);

				if(message->destination < MESSAGE_RECEIVERS){
					MessageReceiver* receiver = _receivers[message->destination];
					if(receiver == nullptr){
						DLOG_F(ERROR, "Message bound for null receiver | dst:%d | src:%d", message->destination, message->source);
						delete &message;
					}
					else{
						receiver->giveMessage(*message);
					}
				}
				else{
					DLOG_F(ERROR, "Message has invalid destination | dst:%d | src:%d", message->destination, message->source);
					delete &message;
				}

				_msgAvailable = false;
			}

			lock.unlock();

		}
		LOG_F(2, "MessageManager stopped");
	}

	void giveMessage(Message& message){
		_queue.enqueue(&message);
		std::unique_lock<std::mutex> lock(_msgMutex);
		_msgAvailable = true;
		lock.unlock();
		_cv.notify_one();
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

		while(1){
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
		_centralQueue.giveMessage(*(new ServerMessage(SYSTEM_CONTROL, ServerMessage::STOP, nullptr)));
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

	void giveMessage(Message& message){
		auto msg = dynamic_cast<ControllerMessage&>(message);

		ClientRequest* request;
		switch(msg.type){
		case ControllerMessage::CLIENT_REQUEST:
			request = reinterpret_cast<ClientRequest*>(message.pData);
			processRequest(*request);
			break;

		case ControllerMessage::NOTIFY_READY:
			switch(msg.source){
			case SCANNER_SM:
				_activeModules |= SCANNER_FLAG;
				break;
			case DEMOD:
				_activeModules |= DEMOD_FLAG;
				break;
			case SERVER_MAN:
				_activeModules |= CONNMGR_FLAG;
				break;
			case AUDIO_MAN:
				_activeModules |= AUDIO_FLAG;
				break;
			default:
				break;
			}
			break;

		case ControllerMessage::NOTIFY_STOPPED:
			switch (msg.source) {
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
		delete &message;
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

static MessageManager messageManager;
static SystemList scanSystems;
static ScannerSM scanner(messageManager, scanSystems);
static ServerManager connectionManager(messageManager);
static Demodulator demod(messageManager);
static SystemController sysControl(messageManager, scanSystems, scanner, connectionManager, demod);

void sigHandler(int signal){
	LOG_F(INFO, "Stop triggered by interrupt");
	sysRun = false;
}

void setDemodulator(DemodInterface* demod) {
	DCHECK_F(demod != nullptr);
	Entry::demod = demod;
}

int main(int argc, char **argv) {
	loguru::init(argc, argv);
	loguru::add_file(LOG_PATH, loguru::Truncate, loguru::Verbosity_2);
	//loguru::g_stderr_verbosity = loguru::Verbosity_MAX;

	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	LOG_F(INFO, "Starting PiScan");

	messageManager.setReceiver(SYSTEM_CONTROL, &sysControl);
	messageManager.setReceiver(SCANNER_SM, &scanner);
	messageManager.setReceiver(DEMOD, &demod);
	messageManager.setReceiver(SERVER_MAN, &connectionManager);
	//messageManager.setReceiver(AUDIO_MAN, &audioControl);

	//DEMO
	AnalogSystem demo = DemoSystem();
	scanSystems.addSystem(dynamic_cast<RadioSystem&>(demo));

	setDemodulator(&demod);

	messageManager.start();
	sysControl.start();

	while(sysRun)
		usleep(100000);

	sysControl.stop();
	messageManager.stop(true);

	LOG_F(INFO, "PiScan stopped, exiting");

	if(activeMessages > 0)
		DLOG_F(WARNING, "Memory leak: %i messages not deleted!", activeMessages);

	return 0;
}
