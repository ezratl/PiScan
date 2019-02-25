/*
 * ServerDebugOutput.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef SERVERDEBUGOUTPUT_H_
#define SERVERDEBUGOUTPUT_H_

#include "BackendServer.h"
#include "ServerManager.h"

class DebugConsole : public Connection {
public:
	DebugConsole() : Connection(FULL_CONTROL, AUDIO_NONE) {}

	void connect();
	void disconnect();
	void giveMessage(Message& message);
};

class DebugServer : public BackendServer {
public:
	DebugServer(ServerInterface& host) : _host(host) {}

	void start();
	void stop();
	void giveMessage(Message& message);
private:
	DebugConsole _connection;
};

#endif /* SERVERDEBUGOUTPUT_H_ */
