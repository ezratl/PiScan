/*
 * BackendServer.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef BACKENDSERVER_H_
#define BACKENDSERVER_H_

#include "messages.h"
#include "ServerManager.h"

class BackendServer : public MessageReceiver {
public:
	BackendServer(ServerInterface& host) : _host(host) {}
	virtual ~BackendServer();

	virtual void start() = 0;
	virtual void stop() = 0;

private:
	ServerInterface& _host;
};

#endif /* BACKENDSERVER_H_ */
