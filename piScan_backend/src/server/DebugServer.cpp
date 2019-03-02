/*
 * ServerDebugOutput.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include <iostream>
#include <string>
#include <sstream>

#include "constants.h"
#include "DebugServer.h"
#include "loguru.hpp"

void DebugConsole::connect(){
	std::cout << "Connecting...";
	_run = true;
	_requestThread = std::thread(&DebugConsole::_consoleInputFunc, this);
}

void DebugConsole::disconnect(){
	_run = false;

	std::fprintf(stdin, "\n");
	std::cout << "\nConsole disconnected\n";
	notifyDisconnected();
}

void DebugConsole::giveMessage(Message& message){
	delete &message;
}

void DebugConsole::_consoleInputFunc() {
	std::string input = "";
	std::cout << "\nConsole connected\n";
	while(_run){
		input.clear();
		std::getline(std::cin, input);

		if(input.compare("stop") == 0){
			_run = false;

			ClientRequest::RequestParams params;
			issueRequest(params);
		}
	}
	std::cout << "\nConsole thread exited\n";
}

void DebugServer::start(){
	this->_connection = new DebugConsole();
	this->_host.requestConnection(_connection);
	if(_connection == nullptr)
		DLOG_F(WARNING, "Debug connection failed");
}

void DebugServer::stop(){
	if (_connection->_run)
		_connection->disconnect();
}

void DebugServer::giveMessage(Message& message){
	delete &message;
}

