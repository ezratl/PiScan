/*
 * messages/context.h
 *
 *  Created on: Mar 12, 2019
 *      Author: ezra
 */
#ifndef COMMON_CLIENTMESSAGE_H_
#define COMMON_CLIENTMESSAGE_H_

#include <string>
#include <vector>

#include "scan_types.h"

using namespace std;
namespace piscan {
namespace server {
namespace context {

struct SystemInfo {
	string version;
	int buildNumber;
	pair<int, int> squelchRange;
	vector<string> supportedModulations;
};

struct ScannerContext {
	enum ScannerState {
		OTHER_STATE,
		SCAN,
		HOLD,
		RECEIVE,
	};

	ScannerContext(){};

	ScannerContext(const ScannerContext& copy);

	ScannerContext(ScannerState state, std::string sysTag, std::string entryTag,
			long long freq, std::string mod, std::string index);

	ScannerContext(ScannerState state, piscan::scan::Entry& entry);

	void clearFields(){
		systemTag.clear();
		entryTag.clear();
		frequency = 0;
		modulation.clear();
		entryIndex.clear();
	};

	ScannerState state = OTHER_STATE;
	std::string systemTag;
	std::string entryTag;
	long long frequency = 0;
	std::string modulation;
	std::string entryIndex;
	int delayMS = 0;
	bool lockout = false;
	
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
}
}
#endif /* COMMON_CLIENTMESSAGE_H_ */
