/*
 * ServerDebugOutput.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef SERVERDEBUGOUTPUT_H_
#define SERVERDEBUGOUTPUT_H_

#include <thread>
#include <boost/shared_ptr.hpp>

#include "BackendServer.h"
#include "connection.h"

namespace piscan {

class DebugServer;

class DebugConsole : public Connection {
public:
	DebugConsole() : Connection(FULL_CONTROL, AUDIO_NONE) {}
	~DebugConsole() {
		_requestThread.join();
	};

	bool connect();
	void disconnect();
	void giveMessage(Message& message);
	void contextUpdate(ScannerContext context);
	void contextUpdate(DemodContext context);
	void systemMessage(GeneralMessage message);

	const std::string identifier() {
		return "Debug Console";
	}

	friend DebugServer;

private:
	bool _run = false;
	std::thread _requestThread;

	void _consoleInputFunc();

	void gainReceived(int handle, int gain);
};

class DebugServer : public BackendServer {
public:
	DebugServer(ServerInterface& host) : BackendServer(host), _connection(new DebugConsole()) {}
	~DebugServer() {};

	void start();
	void stop();
	void giveMessage(Message& message);
private:
	boost::shared_ptr<DebugConsole> _connection;
};
}
#endif /* SERVERDEBUGOUTPUT_H_ */
