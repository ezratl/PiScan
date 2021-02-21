/*
 * DebugConsole.h
 *
 *  Created on: Jul 5, 2020
 *      Author: ezra
 */

#pragma once

#include <thread>

#include "connection.h"
#include "DebugServer.h"

namespace piscan {
namespace server {
namespace connection {

class DebugConsole : public Connection {
public:
	DebugConsole() : Connection(FULL_CONTROL, AUDIO_NONE) {}
	~DebugConsole() {
		_requestThread.join();
	};

	bool connect();
	void disconnect();
	void contextUpdate(const piscan::server::context::ScannerContext context);
	void contextUpdate(const piscan::server::context::DemodContext context);
	void handleSystemMessage(const piscan::server::context::GeneralMessage message);
	void handleSystemInfo(const piscan::server::context::SystemInfo info);
	void handleSignalLevel(const int level);

	const std::string identifier() {
		return "Debug Console";
	}

	friend piscan::server::DebugServer;

private:
	bool _run = false;
	std::thread _requestThread;

	void _consoleInputFunc();

	void gainReceived(int handle, int gain);
};

}
}
}
