/*
 * connection.h
 *
 *  Created on: Mar 1, 2019
 *      Author: ezra
 */

#ifndef SERVER_CONNECTION_H_
#define SERVER_CONNECTION_H_

#include "constants.h"
#include "messages.h"
#include "request.h"

#define HANDLE_NULL	-1


class ServerInterface {
public:
	virtual ~ServerInterface() {};

	enum RequestResponse {
		RQ_ACCEPTED,
		RQ_DENIED,
		RQ_INSUFFICIENT_PERMISSION,
		RQ_INVALID_HANDLE,
	};

	virtual int requestConnection(void* client) = 0;
	virtual int giveRequest(void* request) = 0;
};

class Connection : public MessageReceiver {
public:


	enum AudioReceive {
		AUDIO_NONE,
		AUDIO_RECEIVE,
	};

	Connection(ConnectionLevel lvl, AudioReceive aud) :
		_level(lvl), _audio(aud), _serverManager(nullptr), _handle(HANDLE_NULL) {}
	virtual ~Connection() {};

	virtual void giveMessage(Message& message) = 0;
	virtual bool connect() = 0;
	virtual void disconnect() = 0;

private:
	friend class ServerManager;
	friend class ClientRequest;

	//static ConnectionLevel permissionMap[];
	ConnectionLevel _level;
	AudioReceive _audio;
	ServerInterface* _serverManager;
	int _handle;
protected:


	void notifyDisconnected() {
		ClientRequest::RequestParams params = {.type = NOTIFY_DISCONNECTED};
		issueRequest(params);
	}

	int issueRequest(ClientRequest::RequestParams params, void* data = nullptr, void (*_callback)(int, void*) = nullptr){
		ClientRequest* rq = new ClientRequest(_handle, params, data, _callback);
		return _serverManager->giveRequest(rq);
	}

	enum SystemFunction {
		STOP,
	};
	int systemFunction(SystemFunction function){
		ClientRequest::RequestParams params = { .type = SYSTEM_FUNCTION };
		switch(function){
		case STOP:
			params.subType = SYSTEM_STOP;
			break;
		default:
			return -1;
		}
		return issueRequest(params);
	}

	enum ScannerFunction {
		SCAN,
		HOLD,
		MANUAL,
	};
	int scannerFunction(ScannerFunction function, uint32_t freq = 0){
		ClientRequest::RequestParams params = { .type = SCANNER_FUNCTION };
		switch (function) {
		case SCAN:
			params.subType = SCANNER_STATE_SCAN;
			break;
		case HOLD:
			params.subType = SCANNER_STATE_HOLD;
			break;
		case MANUAL:
			params.subType = SCANNER_STATE_MANUAL;
			return issueRequest(params, new uint32_t(freq));
			break;
		default:
			return -1;
		}
		return issueRequest(params);
	}

	int setDemodSquelch(int level){
		ClientRequest::RequestParams params = { .type = DEMOD_CONFIGURE, .subType = DEMOD_SET_SQUELCH };
		return issueRequest(params, new int(level));
	}

	int setDemodGain(int level){
		ClientRequest::RequestParams params = { .type = DEMOD_CONFIGURE, .subType = DEMOD_SET_GAIN };
		return issueRequest(params, new int(level));
	}
};

#endif /* SERVER_CONNECTION_H_ */
