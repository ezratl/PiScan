/*
 * messages.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "atomicops.h"
#include "concurrentqueue.h"
#include "readerwriterqueue.h"

enum {
	SYSTEM_CONTROL,
	SCANNER_SM,
	DEMOD,
	SERVER_MAN,
	AUDIO_MAN,
	CLIENT,
};

/* basic interthread message structure */
class Message {
public:
	Message(unsigned char src, unsigned char dst, void* data) : source(src), destination(dst), pData(data) {}
	const unsigned char source;
	const unsigned char destination;
	void* const pData;
};

class ControllerMessage : public Message{

};

class ScannerMessage : public Message {
public:
	ScannerMessage(unsigned char src, unsigned char msgType, void* data) :
		Message(src, SCANNER_SM, data), type(msgType) {}
	const unsigned char type;
	enum {
		CLIENT_REQUEST,

		STOP = 0xFF
	};

	enum {
		STATE_SCAN,
		STATE_HOLD,
	};
};

class DemodMessage : public Message {

};

class AudioMessage : public Message {
public:
	AudioMessage(unsigned char src, unsigned char msgType, void* data) :
		Message(src, AUDIO_MAN, data), type(msgType) {}
	const unsigned char type;
	enum {
		ENABLE_OUTPUT = 0,
		DISABLE_OUTPUT,

		STOP = 0xFF
	};
};

class ServerMessage : public Message {
public:
	enum {
		CONTEXT_UPDATE,

		NOTIFY_ALL_CLIENTS,
		NOTIFY_USERS,
		NOTIFY_VIEWERS,

		STOP = 0xFF
	};
};

/* interface for all classes using ITC */
class MessageReceiver {
public:
	virtual ~MessageReceiver();
	virtual void giveMessage(Message msg) = 0;
};

#endif /* MESSAGES_H_ */
