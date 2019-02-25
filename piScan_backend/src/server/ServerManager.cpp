/*
 * ServerManager.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#include "ServerManager.h"

#define MAX_CONNECTIONS	5
#define QUEUE_SIZE		64

ServerManager::ServerManager(MessageReceiver& central) :
		_centralQueue(central), _queue(QUEUE_SIZE), _activeConnections(0), _connections(
				MAX_CONNECTIONS) {

}

void ServerManager::start(){
	//TODO
}

void ServerManager::allowConnections(){
	//TODO
}

void ServerManager::disconnectClients(){
	//TODO might need locks for array
	for(int i = 0; i < MAX_CONNECTIONS; ++i){
		Connection::Connection& con = _connections[i];
		if(con != NULL){
			con.disconnect();
		}
	}
}

void ServerManager::_queueThreadFunc(void){
	//todo break condition
	while(1){
		Message& message;
		if(_queue.try_dequeue(message)){
			if(message.destination != SERVER_MAN){
				_centralQueue.giveMessage(message);
			}
			else{
				_handleMessage(message);
			}
		}
	}
}

void ServerManager::giveMessage(Message& message){
	_queue.enqueue(message);
}

ServerInterface::RequestResponse ServerManager::requestConnection(Connection& client){
	if(_activeConnections < MAX_CONNECTIONS){
		_addConnection(client);
		return RQ_ACCEPTED;
	}
	else{
		delete client;
		return RQ_DENIED;
	}
}

ServerInterface::RequestResponse ServerManager::giveRequest(ClientRequest& request){
	if(_connections[request.src._handle] != request.src){
		delete request;
		return RQ_INVALID_HANDLE;
	}
	else if(request.src._level < ClientRequest::permissionMap[request.requestType]){
		delete request;
		return RQ_INSUFFICIENT_PERMISSION;
	}

	//todo new message, request passed as data
	//unsigned char dest = 0;
	Message message;
	switch(request.requestType){
	case ClientRequest::SYSTEM_FUNCTION:
		//dest = SYSTEM_CONTROL;
		break;
	case ClientRequest::SCANNER_FUNCTION:
		//dest = SCANNER_SM;
		message = new ScannerMessage(CLIENT, ScannerMessage::CLIENT_REQUEST, &request);
		break;
	case ClientRequest::DATABASE_RETRIEVE:
		//dest = SYSTEM_CONTROL;
		break;
	case ClientRequest::DATABASE_MODIFY:
		//dest = SYSTEM_CONTROL;
		break;
	case ClientRequest::CONFIG_RETRIEVE:
		//dest = SYSTEM_CONTROL;
		break;
	case ClientRequest::CONFIG_MODIFY:
		//dest = SYSTEM_CONTROL;
		break;
	}

	this->giveMessage(message);
	//delete request;
	return RQ_ACCEPTED;
}

void ServerManager::_handleMessage(Message& message){
	//TODO
}

void ServerManager::_addConnection(Connection& client){
	//TODO
	for(int i = 0; i < MAX_CONNECTIONS; ++i){
		if(_connections[i] == NULL){
			_connections[i] = client;
			client._handle = i;
			client.serverManager = this;
			client.connect();
		}
	}
}
