/*
 * ServerManager.h
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#ifndef SERVER_SERVERMANAGER_H_
#define SERVER_SERVERMANAGER_H_

#include "messages.h"

class Connection;
class ClientRequest;
class ServerInterface;
class ServerManager;

class Connection : public MessageReceiver {
public:
	enum ConnectionLevel {
		RECEIVE_ONLY,
		VIEWER,
		FULL_CONTROL,
	};

	enum AudioReceive {
		AUDIO_NONE,
		AUDIO_RECEIVE,
	};

	Connection(ConnectionLevel lvl, AudioReceive aud);

	virtual void giveMessage(Message message) = 0;
	void disconnect();

private:
	friend class ServerManager;
	int _handle;
	ConnectionLevel _level;
	AudioReceive _audio;
	ServerInterface _serverManager;

	void _notifyDisconnected();
};

class ClientRequest : public Message {
public:
	enum {
		REQUEST,
		NOTIFY_DISCONNECTED,
	};

	enum RequestType {
		SYSTEM_FUNCTION,
		SCANNER_FUNCTION,
		DATABASE_RETRIEVE,
		DATABASE_MODIFY,
		CONFIG_RETRIEVE,
		CONFIG_MODIFY,
	};

	ClientRequest(Connection client, unsigned char dst);

private:
	static const Connection::ConnectionLevel permissionMap = {
			[SYSTEM_FUNCTION] = Connection::ConnectionLevel::FULL_CONTROL,
			[SCANNER_FUNCTION] = Connection::ConnectionLevel::FULL_CONTROL,
			[DATABASE_RETRIEVE] = Connection::ConnectionLevel::VIEWER,
			[DATABASE_MODIFY] = Connection::ConnectionLevel::FULL_CONTROL,
			[CONFIG_RETRIEVE] = Connection::ConnectionLevel::VIEWER,
			[CONFIG_MODIFY] = Connection::ConnectionLevel::FULL_CONTROL,
	};

	friend class ServerManager;
	Connection src;
	RequestType requestType;
	void (*_callback)(void*);
};

class ServerInterface {
public:
	ServerInterface() = 0;
	virtual ~ServerInterface();

	enum RequestResponse {
		RQ_ACCEPTED,
		RQ_DENIED,
		RQ_INSUFFICIENT_PERMISSION,
		RQ_INVALID_HANDLE,
	};

	virtual RequestResponse requestConnection(Connection::Connection client) = 0;
	virtual RequestResponse giveRequest(ClientRequest request) = 0;
};

class ServerManager : public MessageReceiver, ServerInterface {
public:
	ServerManager(MessageReceiver central);

	void start();
	void allowConnections();
	void disconnectClients();
	void giveMessage(Message message);
protected:

private:
	moodycamel::ConcurrentQueue _queue;
	int _activeConnections;
	Connection _connections[];
	MessageReceiver _centralQueue;

	void _queueThreadFunc(void);
	void _handleMessage(Message message);
	void _addConnection(Connection::Connection client);
	RequestResponse requestConnection(Connection::Connection client);
	RequestResponse giveRequest(ClientRequest request);

};

#endif /* SERVER_SERVERMANAGER_H_ */
