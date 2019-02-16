/*
 * BackendServer.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef BACKENDSERVER_H_
#define BACKENDSERVER_H_

#include "messages.h"

class BackendServer : public MessageReceiver {
public:
	BackendServer();
	virtual ~BackendServer();
};

#endif /* BACKENDSERVER_H_ */
