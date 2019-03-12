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

#include "messages.h"
#include "request.h"
#include "connection.h"
#include "BackendServer.h"

class Connection;

class ServerManager : public MessageReceiver, public ServerInterface {
public:
	ServerManager(MessageReceiver& central);
	~ServerManager() {
		for(unsigned int i = 0; i < _servers.size(); i++)
			delete _servers[i];
		_queueThread.join(); };

	void start();
	void allowConnections();
	void disconnectClients();
	void giveMessage(Message& message);
protected:

private:
	MessageReceiver& _centralQueue;
	moodycamel::ConcurrentQueue<Message*> _queue;
	moodycamel::ReaderWriterQueue<Connection*> _connectionQueue;
	int _activeConnections;
	std::vector<Connection*> _connections;
	std::vector<BackendServer*> _servers;
	std::thread _queueThread;
	std::mutex _msgMutex;
	std::condition_variable _cv;
	bool _msgAvailable = false;
	bool _allowConnections = false;
	bool _run = false;


	void _queueThreadFunc(void);
	void _handleMessage(Message& message);
	void _addConnection(Connection& client);
	int requestConnection(void* client);
	int giveRequest(void* request);
	void _broadcastConnections();

};

#endif /* SERVER_SERVERMANAGER_H_ */
