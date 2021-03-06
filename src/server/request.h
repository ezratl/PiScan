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

namespace piscan {

class ServerManager;
class ClientRequest;

class RequestCallbackInterface {
public:
	virtual ~RequestCallbackInterface() {};
	virtual void scannerContextRequestCallback(int handle, void* data) = 0;
	virtual void demodContextRequestCallback(int handle, void* data) = 0;

};

class ClientRequest : public Message {
public:

	struct RequestParams {
		RequestType type;
		int subType;
	};


	ClientRequest(unsigned char handle, RequestParams info, void* data, RequestCallbackInterface* source = 0) :
		Message(handle, 0, data), rqInfo(info), connection(source) {}
	~ClientRequest() {};


	int rqHandle = 0;
	RequestParams rqInfo;

	RequestCallbackInterface* const connection;
private:


	friend class ServerManager;
};

}
#endif /* SERVER_REQUEST_H_ */
