/*
 * ServerDebugOutput.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef SERVERDEBUGOUTPUT_H_
#define SERVERDEBUGOUTPUT_H_

#include <thread>

#include "BackendServer.h"
#include "connection.h"

class DebugServer;

class DebugConsole : public Connection {
public:
	DebugConsole() : Connection(FULL_CONTROL, AUDIO_NONE) {}
	~DebugConsole() {
		_requestThread.join();
	};

	void connect();
	void disconnect();
	void giveMessage(Message& message);

	friend DebugServer;

private:
	bool _run = false;
	std::thread _requestThread;

	void _consoleInputFunc();
};

class DebugServer : public BackendServer {
public:
	DebugServer(ServerInterface& host) : BackendServer(host), _connection(nullptr) {}
	~DebugServer() {};

	void start();
	void stop();
	void giveMessage(Message& message);
private:
	DebugConsole* _connection;
};

#endif /* SERVERDEBUGOUTPUT_H_ */
