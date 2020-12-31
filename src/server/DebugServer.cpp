/*
 * ServerDebugOutput.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "constants.h"
#include "DebugServer.h"
#include "connection/DebugConsole.h"
#include "loguru.hpp"
#include "threadname.h"
#include "BackendServer.h"

namespace piscan::server {

DebugServer::DebugServer(ServerInterface& host) : BackendServer(host), _connection(new connection::DebugConsole()) {}

void DebugServer::start(){
	//this->_connection(new DebugConsole());
	this->_host.requestConnection(_connection);
	if(_connection == nullptr)
		DLOG_F(WARNING, "Debug connection failed");
}

void DebugServer::stop(){
	if (_connection->_run)
		_connection->disconnect();
}

void DebugServer::giveMessage(std::shared_ptr<Message> message){
	//delete &message;
}

}
