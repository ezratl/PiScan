/*
 * clientmessage.h
 *
 *  Created on: Mar 12, 2019
 *      Author: ezra
 */

#include <string>

#ifndef CORE_CLIENTMESSAGE_H_
#define CORE_CLIENTMESSAGE_H_

namespace piscan {

struct ScannerContext {
	enum ScannerState {
		OTHER_STATE,
		SCAN,
		HOLD,
		RECEIVE,
	};

	ScannerContext(){};

	ScannerContext(const ScannerContext& copy) :
			state(copy.state), systemTag(copy.systemTag), entryTag(
					copy.entryTag), frequency(copy.frequency), modulation(
					copy.modulation), entryIndex(copy.entryIndex) {
	};

	ScannerContext(ScannerState state, std::string sysTag, std::string entryTag,
			unsigned long freq, std::string mod, std::string index) :
			state(state), systemTag(sysTag), entryTag(entryTag), frequency(
					freq), modulation(mod), entryIndex(index) {
	};

	ScannerState state = OTHER_STATE;
	std::string systemTag;
	std::string entryTag;
	unsigned long frequency = 0;
	std::string modulation;
	std::string entryIndex;
};

struct GeneralMessage {
	enum MessageType {
		INFO,
		WARNING,
		ERROR,
	};

	GeneralMessage(const GeneralMessage& copy) : type(copy.type), content(copy.content) {};

	GeneralMessage(MessageType type, std::string content) : type(type), content(content) {};

	MessageType type;
	std::string content;
};

struct DemodContext {
	DemodContext(const DemodContext& copy) : gain(copy.gain), squelch(copy.squelch) {};
	DemodContext(float gain, int squelch) : gain(gain), squelch(squelch) {};

	float gain;
	int squelch;
};

}

#endif /* CORE_CLIENTMESSAGE_H_ */
