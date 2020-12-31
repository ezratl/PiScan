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

#include "connection_types.h"
#include "server_types.h"
#include "request.h"
#include "messages/context.h"
#include "constants.h"
#include "messages.h"

#define HANDLE_NULL	-1

namespace piscan {
class TestClient;
}

namespace piscan::server::connection {

class Connection : public piscan::RequestCallbackInterface, public piscan::MessageReceiver {
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
	virtual void contextUpdate(const piscan::server::context::ScannerContext context) = 0;
	virtual void contextUpdate(const piscan::server::context::DemodContext context) = 0;
	virtual void handleSystemMessage(const piscan::server::context::GeneralMessage message) = 0;
	virtual void handleSystemInfo(const piscan::server::context::SystemInfo info) = 0;
	virtual void handleSignalLevel(const int level) = 0;

	virtual const std::string identifier() = 0;

private:
	friend class piscan::ServerManager;
	friend class piscan::ClientRequest;
	friend class piscan::TestClient;

	ConnectionLevel _level;
	AudioReceive _audio;
	ServerInterface* _serverManager;
	int _handle;

	void scannerContextRequestCallback(int handle, void* data){
		assert(data != nullptr);
		piscan::server::context::ScannerContext* context = reinterpret_cast<piscan::server::context::ScannerContext*>(data);
		contextUpdate(piscan::server::context::ScannerContext(*context));
		delete context;
	}

	void demodContextRequestCallback(int handle, void* data){
		assert(data != nullptr);
		piscan::server::context::DemodContext* context = reinterpret_cast<piscan::server::context::DemodContext*>(data);
		contextUpdate(piscan::server::context::DemodContext(*context));
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

}
#endif /* SERVER_CONNECTION_H_ */
