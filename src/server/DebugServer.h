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

	bool connect();
	void disconnect();
	void giveMessage(Message& message);
	void contextUpdate(ScannerContext context);
	void contextUpdate(DemodContext context);
	void systemMessage(GeneralMessage message);

	friend DebugServer;

private:
	bool _run = false;
	std::thread _requestThread;

	void _consoleInputFunc();

	void gainReceived(int handle, int gain);
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
