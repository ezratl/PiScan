/*
 * ServerManager.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#include "ServerManager.h"
#include "loguru.hpp"
#include "DebugServer.h"

#define MAX_CONNECTIONS	5
#define QUEUE_SIZE		64

ServerManager::ServerManager(MessageReceiver& central) :
		_centralQueue(central), _queue(QUEUE_SIZE), _activeConnections(0), _connections(
				MAX_CONNECTIONS) {
	_servers.push_back(new DebugServer(*this));
}

void ServerManager::start(){
	_run = true;
	_queueThread = std::thread(&ServerManager::_queueThreadFunc, this);

	for(unsigned int i = 0; i < _servers.size(); i++)
		_servers[i]->start();

	Message* message = new ControllerMessage(SERVER_MAN, ControllerMessage::NOTIFY_READY);
	_centralQueue.giveMessage(*message);
	LOG_F(1, "Connection Manager started");
}

void ServerManager::allowConnections(){
	_allowConnections = true;
	LOG_F(2, "Begin accepting connections");
}

void ServerManager::disconnectClients(){
	LOG_F(1, "Disconnecting all clients");
	//TODO might need locks for array
	_allowConnections = false;
	for(int i = 0; i < MAX_CONNECTIONS; ++i){
		Connection* con = _connections[i];
		if(con != nullptr){
			con->disconnect();
		}
	}
}

void ServerManager::_queueThreadFunc(void){
	while(_run){
		Message* message;
		if(_queue.try_dequeue(message)){
			assert(message != nullptr);
			if(message->destination != SERVER_MAN && message->source != CLIENT){
				_centralQueue.giveMessage(*message);
			}
			else{
				_handleMessage(*message);
			}
		}

		Connection* newCon = nullptr;
		if(_allowConnections && _connectionQueue.try_dequeue(newCon)){
			_addConnection(*newCon);
		}
	}

	for(int i = 0; i < _servers.size(); i++)
		_servers[i]->stop();

	// empty queue
	Message* m;
	while(_queue.try_dequeue(m))
		delete m;

	_centralQueue.giveMessage(*(new ControllerMessage(SERVER_MAN, ControllerMessage::NOTIFY_STOPPED)));
	LOG_F(1, "Connection Manager stopped");
}

void ServerManager::giveMessage(Message& message){
	_queue.enqueue(&message);
}

int ServerManager::requestConnection(void* client){
	if(_activeConnections < MAX_CONNECTIONS){

		_connectionQueue.enqueue(static_cast<Connection*>(client));
		return RQ_ACCEPTED;
	}
	else{
		delete &client;
		return RQ_DENIED;
	}
}

int ServerManager::giveRequest(void* request){
	assert(request != nullptr);
	auto rq = static_cast<ClientRequest*>(request);
	/*if(_connections[rq->src]._handle != rq->src){
		delete &rq;
		return RQ_INVALID_HANDLE;
	}
	else if(rq->src->_level < Connection::permissionMap[rq->rqInfo.type]){
		delete &rq;
		return RQ_INSUFFICIENT_PERMISSION;
	}*/

	Message* message;
	switch(rq->rqInfo.type){
	case NOTIFY_DISCONNECTED:
		delete _connections[rq->source];
		break;
	case SYSTEM_FUNCTION:
		//dest = SYSTEM_CONTROL;
		message = new ControllerMessage(CLIENT, ControllerMessage::CLIENT_REQUEST, rq);
		break;
	case SCANNER_FUNCTION:
		//dest = SCANNER_SM;
		message = new ScannerMessage(CLIENT, ScannerMessage::CLIENT_REQUEST, rq);
		break;
	case DATABASE_RETRIEVE:
		//dest = SYSTEM_CONTROL;
		delete rq;
		break;
	case DATABASE_MODIFY:
		//dest = SYSTEM_CONTROL;
		delete rq;
		break;
	case CONFIG_RETRIEVE:
		//dest = SYSTEM_CONTROL;
		delete rq;
		break;
	case CONFIG_MODIFY:
		//dest = SYSTEM_CONTROL;
		delete rq;
		break;
	}

	this->giveMessage(*message);
	return rq->rqHandle;
}

void ServerManager::_handleMessage(Message& message){
	//TODO
	if(message.source == CLIENT){
		if(_allowConnections){
			_addConnection(*(static_cast<Connection*>(message.pData)));
		}
		else {
			// connections not yet allowed, put request back on queue
			giveMessage(message);
			return;
		}
	}
	else{
		auto msg = dynamic_cast<ServerMessage&>(message);
		switch(msg.type){
		case ServerMessage::CONTEXT_UPDATE:
			break;
		case ServerMessage::NOTIFY_ALL_CLIENTS:
			break;
		case ServerMessage::NOTIFY_USERS:
			break;
		case ServerMessage::NOTIFY_VIEWERS:
			break;
		case ServerMessage::STOP:
			disconnectClients();
			_run = false;
			break;
		default:
			break;
		}
	}

	delete &message;
}

void ServerManager::_addConnection(Connection& client){
	//TODO
	for(int i = 0; i < MAX_CONNECTIONS; ++i){
		if(_connections[i] == nullptr){
			LOG_F(1, "Initiating connection with handle %i", i);

			_connections[i] = &client;
			client._handle = i;
			client._serverManager = this;
			client.connect();
			break;
		}
	}
}

/*ConnectionLevel Connection::permissionMap[] = {
		[SYSTEM_FUNCTION] = FULL_CONTROL,
		[SCANNER_FUNCTION] = FULL_CONTROL,
		[DATABASE_RETRIEVE] = VIEWER,
		[DATABASE_MODIFY] = FULL_CONTROL,
		[CONFIG_RETRIEVE] = VIEWER,
		[CONFIG_MODIFY] = FULL_CONTROL,
};*/

