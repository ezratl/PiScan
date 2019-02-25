#include <iostream>
using namespace std;

#include "constants.h"
#include "Demodulator.h"
//#include "loguru.hpp"
#include "loguru.cpp"
#include "messages.h"
#include "ServerManager.h"
#include "ScannerStateMachine.h"
#include "SystemList.h"

class MessageManager : public MessageReceiver {
public:
	MessageManager();
private:
	moodycamel::ConcurrentQueue<Message> _queue;

	void giveMessage(Message& message){
		_queue.enqueue(message);
	}
};

int main(int argc, char **argv) {
	//cout << "Hello world";

	loguru::init(argc, argv);
	loguru::add_file(LOG_PATH, loguru::Truncate, loguru::Verbosity_MAX);
	loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
	LOG_F(INFO, "PiScan started");

	return 0;
}
