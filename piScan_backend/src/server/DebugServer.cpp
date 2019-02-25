/*
 * ServerDebugOutput.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include <iostream>

#include "DebugServer.h"

void DebugConsole::connect(){
	std::cout << "Console connected";
}

void DebugConsole::disconnect(){


	std::cout << "Console disconnected";
	notifyDisconnected();
}

void DebugConsole::giveMessage(Message& message){
	delete message;
}



void DebugServer::start(){
	_connection();
	_host.requestConnection(_connection);
}

void DebugServer::stop(){

}

void DebugServer::giveMessage(Message& message){
	delete message;
}

