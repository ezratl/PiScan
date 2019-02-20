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
	Message(unsigned char src, unsigned char dst, void* data = 0) : source(src), destination(dst), pData(data) {}
	const unsigned char source;
	const unsigned char destination;
	void* const pData;
};

/* system control target */
class ControllerMessage : public Message{
public:
	ControllerMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, SYSTEM_CONTROL, data), type(msgType) {}

	enum {
		NOTIFY_READY,
		NOTIFY_STOPPED,
		CLIENT_REQUEST,
	};

	const unsigned char type;
};

/* scanner state machine target */
class ScannerMessage : public Message {
public:
	ScannerMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, SCANNER_SM, data), type(msgType) {}
	const unsigned char type;
	enum {
		CLIENT_REQUEST,
		STATE_CHANGE,

		STOP = 0xFF
	};

	enum {
		STATE_SCAN,
		STATE_HOLD,
	};
};

/* demodulator target */
class DemodMessage : public Message {
public:
	DemodMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, DEMOD, data), type(msgType) {}
	const unsigned char type;
	enum {
		SET_SQUELCH = 0,

		STOP = 0xFF
	};
};

/* audio manager target */
class AudioMessage : public Message {
public:
	AudioMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, AUDIO_MAN, data), type(msgType) {}
	const unsigned char type;
	enum {
		ENABLE_OUTPUT = 0,
		DISABLE_OUTPUT,

		STOP = 0xFF
	};
};

/* connection manager target */
class ServerMessage : public Message {
public:
	ServerMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, SERVER_MAN, data), type(msgType) {}

	enum {
		CONTEXT_UPDATE,

		NOTIFY_ALL_CLIENTS,
		NOTIFY_USERS,
		NOTIFY_VIEWERS,

		STOP = 0xFF
	};

	const unsigned char type;
};

/* interface for all classes using ITC */
class MessageReceiver {
public:
	virtual ~MessageReceiver();
	virtual void giveMessage(Message& msg) = 0;
};

#endif /* MESSAGES_H_ */
