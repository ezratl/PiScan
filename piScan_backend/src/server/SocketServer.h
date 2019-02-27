/*
 * DbusServer.h
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#ifndef SERVER_SOCKETSERVER_H_
#define SERVER_SOCKETSERVER_H_

#include "BackendServer.h"
#include "ServerManager.h"

class SocketConnection : public Connection {
public:
	~SocketConnection() {};
private:

};

/* listener for socket connections */
class SocketServer: public BackendServer {
public:
	SocketServer(ServerInterface& host) : BackendServer(host) {};
	~SocketServer() {};
};

#endif /* SERVER_SOCKETSERVER_H_ */
