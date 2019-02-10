/*
 * DbusServer.h
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#ifndef SERVER_SOCKETSERVER_H_
#define SERVER_SOCKETSERVER_H_

#include "BackendServer.h"

class SocketServer: public BackendServer {
public:
	SocketServer();
	virtual ~SocketServer();
};

#endif /* SERVER_SOCKETSERVER_H_ */
