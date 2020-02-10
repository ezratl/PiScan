/*
 * connection.h
 *
 *  Created on: Mar 1, 2019
 *      Author: ezra
 */

#ifndef SERVER_CONNECTION_H_
#define SERVER_CONNECTION_H_

#include <memory>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "constants.h"
#include "clientmessage.h"
#include "messages.h"
#include "request.h"

#define HANDLE_NULL	-1

namespace piscan {

class Connection;

class ServerInterface {
public:
	virtual ~ServerInterface() {};

	enum RequestResponse {
		RQ_ACCEPTED,
		RQ_DENIED,
		RQ_INSUFFICIENT_PERMISSION,
		RQ_INVALID_HANDLE,
	};

	virtual int requestConnection(boost::shared_ptr<Connection> client) = 0;
	virtual int giveRequest(void* request) = 0;
};

class Connection : public RequestCallbackInterface, public MessageReceiver {
public:


	enum AudioReceive {
		AUDIO_NONE,
		AUDIO_RECEIVE,
	};

	Connection(ConnectionLevel lvl, AudioReceive aud = AUDIO_NONE) :
		_level(lvl), _audio(aud), _serverManager(nullptr), _handle(HANDLE_NULL) {}
	virtual ~Connection() {};

	virtual void giveMessage(std::shared_ptr<Message> message) = 0;
	virtual bool connect() = 0;
	virtual void disconnect() = 0;
	virtual void contextUpdate(const ScannerContext context) = 0;
	virtual void contextUpdate(const DemodContext context) = 0;
	virtual void handleSystemMessage(const GeneralMessage message) = 0;
	virtual void handleSystemInfo(const SystemInfo info) = 0;

	virtual const std::string identifier() = 0;

private:
	friend class ServerManager;
	friend class ClientRequest;
	friend class TestClient;

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

	enum Modulation {
		FM,
		AM,
	};

	//int scannerFunction(ScannerFunction function, uint32_t freq = 0);
	int scanStart();
	int scanHold();
	int scanHoldEntry(std::vector<int> index);
	int scanManualEntry(long freq, std::string mode = "FM");
	int setDemodSquelch(int level);
	int setDemodGain(float level);
	int getScannerContext();
	int getDemodContext();
	int getSystemInfo();

};

typedef boost::shared_ptr<Connection> ConnectionPtr;

}
#endif /* SERVER_CONNECTION_H_ */
