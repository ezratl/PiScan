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
#include "loguru.hpp"

#define MESSAGE_RECEIVERS	5

namespace piscan {

enum {
	SYSTEM_CONTROL = 0,
	SCANNER_SM,
	DEMOD,
	SERVER_MAN,
	AUDIO_MAN,

	CLIENT = 254,
	ALL = 255
};

static int activeMessages = 0;

/* basic interthread message structure */
struct Message {
	Message(unsigned char src, unsigned char dst, void* data = 0) : source(src), destination(dst), pData(data) {
		DLOG_F(7, "Message create | dst:%d | src:%d", destination, source);
		activeMessages++; }
	virtual ~Message() {
		DLOG_F(7, "Message delete | dst:%d | src:%d", destination, source);
		activeMessages--; };
	const unsigned char source;
	const unsigned char destination;
	void* const pData;
};

/* system control target */
struct ControllerMessage : public Message{
	ControllerMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, SYSTEM_CONTROL, data), type(msgType) {
	}
	~ControllerMessage() {};
	enum {
		NOTIFY_READY,
		NOTIFY_STOPPED,
		CLIENT_REQUEST,
	};

	const unsigned char type;
};

/* scanner state machine target */
struct ScannerMessage : public Message {
	ScannerMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, SCANNER_SM, data), type(msgType) {}
	~ScannerMessage() {};
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
struct DemodMessage : public Message {
	DemodMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, DEMOD, data), type(msgType) {}
	~DemodMessage() {};
	const unsigned char type;
	enum {
		CLIENT_REQUEST = 0,
		SET_SQUELCH,
		SET_GAIN,
		OPEN_AUDIO,

		STOP = 0xFF
	};
};

/* audio manager target */
struct AudioMessage : public Message {
	AudioMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, AUDIO_MAN, data), type(msgType) {}
	~AudioMessage() {};
	const unsigned char type;
	enum {
		ENABLE_OUTPUT = 0,
		DISABLE_OUTPUT,

		STOP = 0xFF
	};
};

/* connection manager target */
struct ServerMessage : public Message {
	ServerMessage(unsigned char src, unsigned char msgType, void* data = 0) :
		Message(src, SERVER_MAN, data), type(msgType) {}
	~ServerMessage() {};
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
	virtual ~MessageReceiver() {};
	virtual void giveMessage(std::shared_ptr<Message> message) = 0;
};
}
#endif /* MESSAGES_H_ */
