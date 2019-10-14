/*
 * Entry.h
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#ifndef SCAN_ENTRY_H_
#define SCAN_ENTRY_H_

#include <boost/property_tree/ptree.hpp>

#include "Demodulator.h"

#define TAG_LENGTH	20

#define FM_CHANNEL_HASH		"fmc"
#define AM_CHANNEL_HASH		"amc"
#define PL_CHANNEL_HASH		"plc"
#define DC_CHANNEL_HASH		"dcc"

#define CHAN_TYPE_KEY	"chantype"
#define TAG_KEY			"tag"
#define LOCKOUT_KEY		"lockout"
#define DELAY_KEY		"delay"
#define FREQ_KEY		"freq"
#define TONE_KEY		"tone"
#define CODE_KEY		"code"

using namespace boost::property_tree;

namespace piscan {

class Entry {
public:
	Entry(std::string tag, bool lo, int del) : _tag(tag), _lockedOut(lo), _scanDelay(del) {
		propertyTree.put(TAG_KEY, tag);
		propertyTree.put(LOCKOUT_KEY, lo);
		propertyTree.put(DELAY_KEY, del);
	};
	virtual ~Entry() {};

	std::string	tag() { return _tag; }
	virtual std::string	modulation() = 0;
	bool	isLockedOut() { return _lockedOut; }
	double	delay() { return _scanDelay; }
	void	lockout(bool val = true) {
		_lockedOut = val;
		propertyTree.put(LOCKOUT_KEY, val);
	}
	virtual bool	hasSignal() = 0;
	virtual long long freq() = 0;
	virtual bool	isDummy() { return false; }

	size_t getSysIndex() { return _sysIndex; };
	void setSysIndex(size_t index) { _sysIndex = index; };
	size_t getEntryIndex() { return _entryIndex; };
	void setEntryIndex(size_t index) { _entryIndex = index; };

	ptree getPropertyTree() { return propertyTree; };

private:
	std::string	_tag;
	bool	_lockedOut;
	double	_scanDelay;
	size_t _sysIndex = 0;
	size_t _entryIndex = 0;
protected:
	static DemodInterface* demod;
	friend void setDemodulator(DemodInterface* demod);

	ptree propertyTree;
};

typedef std::shared_ptr<Entry> EntryPtr;

class Channel: public Entry {
public:
	Channel(long freq, std::string tag, bool lo, int del) : Entry(tag, lo, del), frequency(freq){
		propertyTree.put(FREQ_KEY, freq);
	}
	virtual ~Channel() {};
	virtual long long freq() { return frequency; };
protected:
	const long long frequency;

};

class DummyChannel: public Channel {
public:
	DummyChannel(long long freq) : Channel(freq, "", false, 0){

	}
	~DummyChannel(){};

	std::string modulation() { return ""; };

	bool isDummy() { return true; }

	bool hasSignal();
	ptree getPropertyTree() { return ptree(); };
};

class FMChannel : public Channel {
public:
	FMChannel(long long freq, std::string tag, bool lo, int del) : Channel(freq, tag, lo, del){
		propertyTree.put(CHAN_TYPE_KEY, FM_CHANNEL_HASH);
	}
	~FMChannel() {};

	virtual std::string modulation() {
		return "NFM";
	}

	virtual bool hasSignal();
};

class PLChannel: public FMChannel {
public:
	PLChannel(long long freq, float tn, std::string tag, bool lo, int del) :
			FMChannel(freq, tag, lo, del), tone(tn) {
		propertyTree.put(CHAN_TYPE_KEY, PL_CHANNEL_HASH);
		propertyTree.put(TONE_KEY, tn);
	}
	~PLChannel() {};

	//bool hasSignal();
protected:
	const float tone;
};

class DCChannel : public FMChannel {
public:
	DCChannel(long long freq, unsigned int tn, std::string tag, bool lo, int del) :
			FMChannel(freq, tag, lo, del), code(tn) {
		propertyTree.put(CHAN_TYPE_KEY, DC_CHANNEL_HASH);
		propertyTree.put(CODE_KEY, tn);
	}
	~DCChannel() {};

	//bool hasSignal();
protected:
	const unsigned int code;
};

class AMChannel : public Channel {
public:
	AMChannel(long long freq, std::string tag, bool lo, int del) : Channel(freq, tag, lo, del){
		propertyTree.put(CHAN_TYPE_KEY, AM_CHANNEL_HASH);
	}
	~AMChannel() {};

	virtual bool hasSignal() { return false; };

	virtual std::string modulation() { return "AM"; }
};
}
#endif /*Channel_ */
