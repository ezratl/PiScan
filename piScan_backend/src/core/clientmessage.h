/*
 * clientmessage.h
 *
 *  Created on: Mar 12, 2019
 *      Author: ezra
 */

#include <string>

#ifndef CORE_CLIENTMESSAGE_H_
#define CORE_CLIENTMESSAGE_H_

class ScannerContext {
public:
	enum ScannerState {
		OTHER_STATE,
		SCAN,
		HOLD,
		RECEIVE,
	};

	ScannerContext(){};

	ScannerContext(const ScannerContext& copy) :
			_state(copy._state), _systemTag(copy._systemTag), _entryTag(
					copy._entryTag), _frequency(copy._frequency), _modulation(
					copy._modulation), _entryIndex(copy._entryIndex) {
	};

	ScannerContext(ScannerState state, std::string sysTag, std::string entryTag,
			unsigned long freq, std::string mod, std::string index) :
			_state(state), _systemTag(sysTag), _entryTag(entryTag), _frequency(
					freq), _modulation(mod), _entryIndex(index) {
	};

	const ScannerState state() const { return _state; };
	const std::string& systemTag() const { return _systemTag; };
	const std::string& entryTag() const { return _entryTag; };
	const unsigned long frequency() const { return _frequency; };
	const std::string& modulation() const { return _modulation; };
	const std::string& entryIndex() const { return _entryIndex; };

	friend class ScannerSM;

private:
	ScannerState _state = OTHER_STATE;
	std::string _systemTag;
	std::string _entryTag;
	unsigned long _frequency = 0;
	std::string _modulation;
	std::string _entryIndex;
};

class GeneralMessage {
public:
	enum MessageType {
		INFO,
		WARNING,
		ERROR,
	};

	GeneralMessage(const GeneralMessage& copy) : _type(copy._type), _content(copy._content) {};

	GeneralMessage(MessageType type, std::string content) : _type(type), _content(content) {};

	const MessageType type() const { return _type; };
	const std::string& content() const { return _content; };

private:
	MessageType _type;
	std::string _content;
};

#endif /* CORE_CLIENTMESSAGE_H_ */
