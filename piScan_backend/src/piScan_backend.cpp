#include <iostream>
#include <thread>
using namespace std;

#include "constants.h"
#include "Demodulator.h"
#include "Entry.h"
//#include "loguru.hpp"
#include "loguru.cpp"
#include "messages.h"
#include "ServerManager.h"
#include "ScannerStateMachine.h"
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

class MessageManager : public MessageReceiver {
public:
	MessageManager() : _run(false) {};
	~MessageManager() {};

	void setReceiver(unsigned char id, MessageReceiver* ptr){
		if(id < MESSAGE_RECEIVERS){
			_receivers[id] = ptr;
		}
		else{
			DLOG_F(ERROR, "Invalid receiver ID: %d", id);
		}
	}

private:
	moodycamel::ConcurrentQueue<Message*> _queue;
	std::thread _workThread;
	bool _run;
	MessageReceiver* _receivers[MESSAGE_RECEIVERS];

	void _handlerThreadFunc(void){
		while(_run){
			Message* message;
			if(_queue.try_dequeue(message)){
				DCHECK_F(message != nullptr);
				DCHECK_F(message->destination != message->source);

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
			}
		}
	}

	void giveMessage(Message& message){
		_queue.enqueue(&message);
	}
};

class SystemController : public MessageReceiver {
public:
	SystemController(MessageReceiver& central) : _centralQueue(central) {}
	~SystemController() {};
private:
	MessageReceiver& _centralQueue;

	void giveMessage(Message& message){
		auto msg = dynamic_cast<ControllerMessage&>(message);

		switch(msg.type){
		case ControllerMessage::CLIENT_REQUEST:
			//TODO request handling
			break;

		case ControllerMessage::NOTIFY_READY:
			//TODO module start
			break;

		case ControllerMessage::NOTIFY_STOPPED:
			//TODO module delete
			break;
		}

	}
};

static MessageManager messageManager;
static SystemList scanSystems;
static ScannerStateMachine scanner(messageManager, scanSystems);
static ServerManager connectionManager(messageManager);
static SystemController sysControl(messageManager);
static Demodulator demod;

void setDemodulator(DemodInterface* demod) {
	DCHECK_F(demod != nullptr);
	Entry::demod = demod;
}

int main(int argc, char **argv) {
	loguru::init(argc, argv);
	loguru::add_file(LOG_PATH, loguru::Truncate, loguru::Verbosity_MAX);
	loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
	LOG_F(INFO, "PiScan started");

	messageManager.setReceiver(SYSTEM_CONTROL, &sysControl);
	messageManager.setReceiver(SCANNER_SM, &scanner);
	messageManager.setReceiver(DEMOD, &demod);
	messageManager.setReceiver(SERVER_MAN, &connectionManager);
	//messageManager.setReceiver(AUDIO_MAN, &audioControl);

	setDemodulator(&demod);

	return 0;
}
