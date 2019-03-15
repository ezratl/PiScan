/*
 * connection.h
 *
 *  Created on: Mar 1, 2019
 *      Author: ezra
 */

#ifndef SERVER_CONNECTION_H_
#define SERVER_CONNECTION_H_

#include "constants.h"
#include "clientmessage.h"
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

class Connection : public RequestCallbackInterface, public MessageReceiver {
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
	virtual void contextUpdate(ScannerContext context) = 0;
	virtual void contextUpdate(DemodContext context) = 0;
	virtual void systemMessage(GeneralMessage message) = 0;

private:
	friend class ServerManager;
	friend class ClientRequest;

	ConnectionLevel _level;
	AudioReceive _audio;
	ServerInterface* _serverManager;
	int _handle;

	void scannerContextRequestCallback(int handle, void* data){
		assert(data != nullptr);
		ScannerContext* context = reinterpret_cast<ScannerContext*>(data);
		contextUpdate(ScannerContext(*context));
		delete context;
	}

	void demodContextRequestCallback(int handle, void* data){
		assert(data != nullptr);
		DemodContext* context = reinterpret_cast<DemodContext*>(data);
		contextUpdate(DemodContext(*context));
		delete context;
	}
protected:

	void notifyDisconnected();
	int issueRequest(ClientRequest::RequestParams params, void* data = nullptr);

	enum SystemFunction {
		STOP,
	};
	int systemFunction(SystemFunction function);

	enum ScannerFunction {
		SCAN,
		HOLD,
		MANUAL,
	};
	int scannerFunction(ScannerFunction function, uint32_t freq = 0);
	int setDemodSquelch(int level);
	int setDemodGain(int level);
	int getScannerContext();
	int getDemodContext();

};

#endif /* SERVER_CONNECTION_H_ */
