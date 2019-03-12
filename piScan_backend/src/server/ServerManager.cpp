/*
 * ServerManager.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

//#include <unistd.h>

#include "ServerManager.h"
#include "loguru.hpp"
#include "DebugServer.h"

#define MAX_CONNECTIONS	5
#define QUEUE_SIZE		64

static ConnectionLevel const permissionMap[] = {
		static_cast<ConnectionLevel>(0), //NOTIFY_DISCONNECTED
		FULL_CONTROL, //SYSTEM_FUNCTION
		FULL_CONTROL, //SCANNER_FUNCTION
		VIEWER, //DATABASE_RETRIEVE
		FULL_CONTROL, //DATABASE_MODIFY
		VIEWER, //CONFIG_RETRIEVE
		FULL_CONTROL, //CONFIG_MODIFY
		FULL_CONTROL, //DEMOD_CONFIGURE
};

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
		std::unique_lock<std::mutex> lock(_msgMutex);
		_cv.wait(lock, [this]{return this->_msgAvailable;});

		Message* message;
		while(_queue.try_dequeue(message)){
			assert(message != nullptr);
			if(message->destination != SERVER_MAN){
				_centralQueue.giveMessage(*message);
			}
			else{
				_handleMessage(*message);
			}
			_msgAvailable = false;
		}

		Connection* newCon = nullptr;
		while(_allowConnections && _connectionQueue.try_dequeue(newCon)){
			_addConnection(*newCon);
			_msgAvailable = false;
		}

		lock.unlock();
	}

	for(unsigned int i = 0; i < _servers.size(); i++)
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
	std::unique_lock<std::mutex> lock(_msgMutex);
	_msgAvailable = true;
	lock.unlock();
	_cv.notify_one();
}

int ServerManager::requestConnection(void* client){
	if(_activeConnections < MAX_CONNECTIONS){
		DLOG_F(8, "New connection request");
		_connectionQueue.enqueue(static_cast<Connection*>(client));
		std::unique_lock<std::mutex> lock(_msgMutex);
		_msgAvailable = true;
		lock.unlock();
		_cv.notify_one();
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
	else */
	int clientLevel = _connections[rq->source]->_level;
	int requestLevel = static_cast<int>(permissionMap[rq->rqInfo.type]);
	if(clientLevel < requestLevel){
		delete rq;
		DLOG_F(ERROR, "Insufficient permission request: %i < %i", clientLevel, requestLevel);
		return RQ_INSUFFICIENT_PERMISSION;
	}

	Message* message;
	switch(rq->rqInfo.type){
	case NOTIFY_DISCONNECTED:
		LOG_F(INFO, "Connection %i disconnected", rq->source);
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
	case DEMOD_CONFIGURE:
		message = new DemodMessage(CLIENT, DemodMessage::CLIENT_REQUEST, rq);
		break;
	default:
		delete rq;
	}

	if(message != nullptr)
		this->giveMessage(*message);
	return rq->rqHandle;
}

void ServerManager::_handleMessage(Message& message){
	assert(message.destination == SERVER_MAN);
	auto msg = dynamic_cast<ServerMessage&>(message);
	switch (msg.type) {
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

	delete &message;
}

void ServerManager::_addConnection(Connection& client){
	//TODO
	for(int i = 0; i < MAX_CONNECTIONS; ++i){
		if(_connections[i] == nullptr){
			LOG_F(1, "Initiating connection with handle %i", i);


			client._handle = i;
			client._serverManager = this;
			if(client.connect()){
				_connections[i] = &client;
				LOG_F(INFO, "Client %i connected, permission level %i", i, client._level);
			}
			else{
				LOG_F(INFO, "Connection attempt failed");
				delete &client;
			}
			break;
		}
	}
}

