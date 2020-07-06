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

#include "connection/connection_types.h"
#include "server_types.h"
#include "SocketServer.h"
#include "BackendServer.h"
#include "synchronize.h"
#include "AudioStreamServer.h"
#include "messages/context.h"
#include "messages.h"
#include "request.h"


#define MAX_CONNECTIONS	5

namespace piscan {

using ConnectionPtr = server::connection::ConnectionPtr;

class ServerManager : public MessageReceiver, public server::ServerInterface, public Synchronizable {
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
	moodycamel::ReaderWriterQueue<ConnectionPtr> _connectionQueue;
	int _activeConnections;
	//std::vector<boost::shared_ptr<Connection>> _connections;
	ConnectionPtr _connections[MAX_CONNECTIONS];
	std::vector<server::BackendServer*> _servers;
	std::thread _queueThread;
	std::mutex _msgMutex;
	std::condition_variable _cv;
	bool _msgAvailable = false;
	bool _allowConnections = false;
	bool _run = false;

	server::DebugServer* _debugServer = nullptr;
	server::SocketServer* _sockServer = nullptr;
	server::AudioStreamServer* _audioServer = nullptr;

	void _queueThreadFunc(void);
	void _handleMessage(std::shared_ptr<Message> message);
	void _addConnection(ConnectionPtr client);
	int requestConnection(ConnectionPtr client);
	int giveRequest(void* request);

	template <class T>
	void _broadcastContextUpdate(T& context);

	void _broadcastGeneralMessage(unsigned char group, piscan::server::context::GeneralMessage& message);
	std::shared_ptr<Message> _makeContextRequest(ClientRequest* rq);

	void _broadcastSignalLevelUpdate(int level);

};
}
#endif /* SERVER_SERVERMANAGER_H_ */
