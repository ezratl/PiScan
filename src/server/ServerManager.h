/*
 * ServerManager.h
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#ifndef SERVER_SERVERMANAGER_H_
#define SERVER_SERVERMANAGER_H_

#include <condition_variable>
#include <thread>
#include <vector>
#include <boost/asio.hpp>

#include "messages.h"
#include "request.h"
#include "connection.h"
#include "clientmessage.h"
#include "SocketServer.h"
#include "DebugServer.h"
#include "BackendServer.h"
#include "synchronize.h"


#define MAX_CONNECTIONS	5

namespace piscan {

class Connection;
class SocketServer;

class ServerManager : public MessageReceiver, public ServerInterface, public Synchronizable {
public:
	ServerManager(boost::asio::io_service& io_service, MessageReceiver& central);
	~ServerManager() {
		for(unsigned int i = 0; i < _servers.size(); i++)
			delete _servers[i];
		_queueThread.join(); };

	void start(bool useDebugServer, bool spawnLocalClient);
	void stop();
	void allowConnections();
	void disconnectClients();
	void giveMessage(std::shared_ptr<Message> message);
protected:

private:
	boost::asio::io_service& _io_service;
	MessageReceiver& _centralQueue;
	moodycamel::ConcurrentQueue<std::shared_ptr<Message>> _queue;
	moodycamel::ReaderWriterQueue<boost::shared_ptr<Connection>> _connectionQueue;
	int _activeConnections;
	//std::vector<boost::shared_ptr<Connection>> _connections;
	boost::shared_ptr<Connection> _connections[MAX_CONNECTIONS];
	std::vector<BackendServer*> _servers;
	std::thread _queueThread;
	std::mutex _msgMutex;
	std::condition_variable _cv;
	bool _msgAvailable = false;
	bool _allowConnections = false;
	bool _run = false;

	DebugServer* _debugServer = nullptr;
	SocketServer* _sockServer = nullptr;

	void _queueThreadFunc(void);
	void _handleMessage(std::shared_ptr<Message> message);
	void _addConnection(boost::shared_ptr<Connection> client);
	int requestConnection(boost::shared_ptr<Connection> client);
	int giveRequest(void* request);

	template <class T>
	void _broadcastContextUpdate(T& context);

	void _broadcastGeneralMessage(unsigned char group, GeneralMessage& message);
	std::shared_ptr<Message> _makeContextRequest(ClientRequest* rq);

};
}
#endif /* SERVER_SERVERMANAGER_H_ */
