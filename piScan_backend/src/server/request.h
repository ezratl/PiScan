/*
 * request.h
 *
 *  Created on: Mar 1, 2019
 *      Author: ezra
 */

#ifndef SERVER_REQUEST_H_
#define SERVER_REQUEST_H_

#include "messages.h"
#include "constants.h"

class ServerManager;

class ClientRequest : public Message {
public:
	enum {
		REQUEST,
		NOTIFY_DISCONNECTED,
	};


	struct RequestParams {
		RequestType type;
		int subType;
	};


	ClientRequest(unsigned char handle, RequestParams info, void (*callback)(void*) = 0) :
		Message(handle, 0), rqInfo(info), _callback(callback) {}
	~ClientRequest() {};


	int rqHandle = 0;
	RequestParams rqInfo;
	void (*_callback)(void*);

private:


	friend class ServerManager;
};


#endif /* SERVER_REQUEST_H_ */
