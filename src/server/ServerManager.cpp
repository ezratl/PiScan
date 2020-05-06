/*
 * ServerManager.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

//#include <unistd.h>

#include <boost/asio.hpp>

#include "ServerManager.h"
#include "loguru.hpp"
#include "DebugServer.h"
#include "SocketServer.h"
#include "threadname.h"


#define QUEUE_SIZE		64

#define SM_THREAD_NAME	"ServerManager"

using namespace piscan;

static ConnectionLevel const permissionMap[] = {
		static_cast<ConnectionLevel>(0), //NOTIFY_DISCONNECTED
		FULL_CONTROL, //SYSTEM_FUNCTION
		FULL_CONTROL, //SCANNER_FUNCTION
		VIEWER, //DATABASE_RETRIEVE
		FULL_CONTROL, //DATABASE_MODIFY
		VIEWER, //CONFIG_RETRIEVE
		FULL_CONTROL, //CONFIG_MODIFY
		FULL_CONTROL, //DEMOD_CONFIGURE
		RECEIVE_ONLY, //GET_CONTEXT
};

ServerManager::ServerManager(boost::asio::io_service& io_service, MessageReceiver& central) : _io_service(io_service),
		_centralQueue(central), _queue(QUEUE_SIZE), _activeConnections(0)/*, _connections(
				MAX_CONNECTIONS)*/ {

}

void ServerManager::start(bool useDebugServer, bool spawnLocalClient){
	if(useDebugServer){
		_debugServer = new DebugServer(*this);
		_servers.push_back(_debugServer);
	}
	_sockServer = new SocketServer(*this, _io_service);
	_servers.push_back(_sockServer);

	_audioServer = new AudioStreamServer(*this);
	_servers.push_back(_audioServer);

	_run = true;
	_queueThread = std::thread(&ServerManager::_queueThreadFunc, this);

	for(unsigned int i = 0; i < _servers.size(); i++)
		_servers[i]->start();

	if(spawnLocalClient)
		_sockServer->spawnLocalClient();

	LOG_F(1, "Connection Manager started");
	notifyReady();
}

void ServerManager::stop() {
	disconnectClients();
	_run = false;
	std::unique_lock<std::mutex> lock(_msgMutex, std::defer_lock);
	if (lock.try_lock()) {
		_msgAvailable = true;
		lock.unlock();
		_cv.notify_one();
	}
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
		Connection* con = _connections[i].get();
		if(con != nullptr){
			con->disconnect();
		}
	}
}

void ServerManager::_queueThreadFunc(void){
	setThreadName(SM_THREAD_NAME);

	while(_run){
		std::unique_lock<std::mutex> lock(_msgMutex);
		_cv.wait(lock, [this]{return this->_msgAvailable;});

		std::shared_ptr<Message> message;
		while(_queue.try_dequeue(message)){
			assert(message != nullptr);
			DLOG_F(7, "Message receive | dst:%d | src:%d", message->destination, message->source);
			if(message->destination != SERVER_MAN){
				_centralQueue.giveMessage(message);
			}
			else{
				_handleMessage(message);
			}
			_msgAvailable = false;
		}

		boost::shared_ptr<Connection> newCon;
		while(_allowConnections && _connectionQueue.try_dequeue(newCon)){
			_addConnection(newCon);
			_msgAvailable = false;
		}

		lock.unlock();
	}

	for(unsigned int i = 0; i < _servers.size(); i++)
		_servers[i]->stop();

	// empty queue
	std::shared_ptr<Message> m;
	while(_queue.try_dequeue(m));

	//_centralQueue.giveMessage(std::make_shared<ControllerMessage>(SERVER_MAN, ControllerMessage::NOTIFY_STOPPED));
	LOG_F(1, "Connection Manager stopped");
	notifyDeinit();
}

void ServerManager::giveMessage(std::shared_ptr<Message> message){
	DLOG_F(7, "Message queue | dst:%d | src:%d", message->destination, message->source);
	_queue.enqueue(message);
	std::unique_lock<std::mutex> lock(_msgMutex, std::defer_lock);
	if (lock.try_lock()) {
		_msgAvailable = true;
		lock.unlock();
		_cv.notify_one();
	}
}

int ServerManager::requestConnection(boost::shared_ptr<Connection> client){
	if(_activeConnections < MAX_CONNECTIONS){
		DLOG_F(8, "New connection request");
		_connectionQueue.enqueue(client);
		std::unique_lock<std::mutex> lock(_msgMutex, std::defer_lock);
		if(lock.try_lock()){
			_msgAvailable = true;
			lock.unlock();
		}
		_cv.notify_one();
		return RQ_ACCEPTED;
	}
	else{
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
	int clientLevel = _connections[rq->source].get()->_level;
	int requestLevel = static_cast<int>(permissionMap[rq->rqInfo.type]);
	if(clientLevel < requestLevel){
		delete rq;
		DLOG_F(ERROR, "Insufficient permission request: %i < %i", clientLevel, requestLevel);
		return RQ_INSUFFICIENT_PERMISSION;
	}

	std::shared_ptr<Message> message;
	switch(rq->rqInfo.type){
	case NOTIFY_DISCONNECTED:
		LOG_F(INFO, "Connection %i disconnected", rq->source);
		//delete _connections[rq->source];
		//_connections.erase(_connections.begin() + (rq->source));
		_connections[rq->source] = nullptr;
		delete rq;
		break;
	case SYSTEM_FUNCTION:
		//dest = SYSTEM_CONTROL;
		message = std::make_shared<ControllerMessage>(CLIENT, ControllerMessage::CLIENT_REQUEST, rq);
		break;
	case SCANNER_FUNCTION:
		//dest = SCANNER_SM;
		message = std::make_shared<ScannerMessage>(CLIENT, ScannerMessage::CLIENT_REQUEST, rq);
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
		message = std::make_shared<DemodMessage>(CLIENT, DemodMessage::CLIENT_REQUEST, rq);
		break;
	case GET_CONTEXT:
		message = _makeContextRequest(rq);
		break;
	default:
		delete rq;
	}

	if(message != nullptr)
		this->giveMessage(message);
	return rq->rqHandle;
}

void ServerManager::_handleMessage(std::shared_ptr<Message> message){
	assert(message->destination == SERVER_MAN);
	auto msg = std::dynamic_pointer_cast<ServerMessage>(message);
	int* level = nullptr;
	switch (msg->type) {
	case ServerMessage::CONTEXT_UPDATE:
		switch(message->source){
		case SCANNER_SM:
			_broadcastContextUpdate(*(reinterpret_cast<ScannerContext*>(msg->pData)));
			break;
		case DEMOD:
			_broadcastContextUpdate(*(reinterpret_cast<DemodContext*>(msg->pData)));
			break;
		default:
			break;
		}
		break;
	case ServerMessage::NOTIFY_ALL_CLIENTS:
	case ServerMessage::NOTIFY_USERS:
	case ServerMessage::NOTIFY_VIEWERS:
		_broadcastGeneralMessage(msg->type, *(reinterpret_cast<GeneralMessage*>(msg->pData)));
		break;
	case ServerMessage::STOP:
		disconnectClients();
		_run = false;
		break;
	case ServerMessage::SIGNAL_LEVEL:
		DLOG_F(7, "Broadcast siglevel update");
		level = reinterpret_cast<int*>(msg->pData);
		_broadcastSignalLevelUpdate(*level);
		delete level;
		break;
	default:
		break;
	}

}

void ServerManager::_addConnection(boost::shared_ptr<Connection> client){
	//TODO
	for(unsigned int i = 0; i < MAX_CONNECTIONS; ++i){
		if(_connections[i] == nullptr){
			LOG_F(1, "Initiating connection with %s", client->identifier().c_str());


			client.get()->_handle = i;
			client.get()->_serverManager = this;
			if(client.get()->connect()){
				_connections[i] = client;
				//_connections.assign(i, client);
				LOG_F(INFO, "Client %s connected with handle %i", client->identifier().c_str(), i);
			}
			else{
				LOG_F(1, "Connection attempt failed");
				//delete &client;
			}
			break;
		}
	}
}

template <class T>
void ServerManager::_broadcastContextUpdate(T& context){
	for(size_t i = 0; i < MAX_CONNECTIONS; i++)
		if(_connections[i] != nullptr)
			_connections[i]->contextUpdate(T(context));

	delete &context;
}

void ServerManager::_broadcastGeneralMessage(unsigned char group, GeneralMessage& message){
	unsigned char connectionLevel = 0;
	switch(group){
	case ServerMessage::NOTIFY_ALL_CLIENTS:
		connectionLevel = RECEIVE_ONLY;
		break;
	case ServerMessage::NOTIFY_USERS:
		connectionLevel = FULL_CONTROL;
		break;
	case ServerMessage::NOTIFY_VIEWERS:
		connectionLevel = VIEWER;
		break;
	default:
		connectionLevel = VIEWER;
	}

	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (_connections[i] != nullptr) {
			Connection* con = _connections[i].get();
			if (con->_level >= connectionLevel)
				con->handleSystemMessage(GeneralMessage(message));
		}
	}

	delete &message;
}

std::shared_ptr<Message> ServerManager::_makeContextRequest(ClientRequest* rq){
	CHECK_F(rq->rqInfo.type == GET_CONTEXT);

	switch(rq->rqInfo.subType){
	case SCANNER_CONTEXT:
		return std::make_shared<ScannerMessage>(CLIENT, ScannerMessage::CLIENT_REQUEST, rq);
		break;
	case DEMOD_CONTEXT:
		return std::make_shared<DemodMessage>(CLIENT, DemodMessage::CLIENT_REQUEST, rq);
		break;
	default:
		return nullptr;
	}
}

void ServerManager::_broadcastSignalLevelUpdate(int level) {
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (_connections[i] != nullptr) {
			Connection* con = _connections[i].get();
			con->handleSignalLevel(level);
		}
	}
}

