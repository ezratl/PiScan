/*
 * Entry.h
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#ifndef SCAN_ENTRY_H_
#define SCAN_ENTRY_H_

#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <atomic>

#include "scan_types.h"
#include "sigproc_types.h"
#include "PiScan.h"

#define TAG_LENGTH	20

#define SQUELCH_TRIGGER_HITS	25

using ptree = boost::property_tree::ptree;

namespace piscan {
namespace scan {

/* base class for all types of scanner entries */
class Entry {
public:
	enum LockoutType {
		LOCKOUT_NONE,
		LOCKOUT_PERSIST,
		LOCKOUT_SESSION,
		LOCKOUT_TIMER,
	};

	Entry(std::string tag, bool lo, int del) : _tag(tag), _scanDelayMS(del), demod(piscan::app::demod::getDemodInstance()) {
		_lockout = (lo) ? LOCKOUT_PERSIST : LOCKOUT_NONE;
		propertyTree.put(database::entry::tag_key, tag);
		propertyTree.put(database::entry::lockout_key, lo);
		propertyTree.put(database::entry::delay_key, del);
	};
	virtual ~Entry() {};

	std::string	tag() { return _tag; }
	virtual std::string	modulation() = 0;
	bool	isLockedOut() { return _lockout.load() != LOCKOUT_NONE; }
	double	delay() { return _scanDelayMS/1000.0; }
	int		delayMS() { return _scanDelayMS; }
	void	lockout(bool val = true) {
		//_lockedOut = val;
		_lockout = (val) ? LOCKOUT_PERSIST : LOCKOUT_NONE;
		propertyTree.put(database::entry::lockout_key, val);
	}
	virtual bool	hasSignal() = 0;
	virtual long long freq() = 0;
	virtual bool	isDummy() { return false; }
	virtual std::string key() = 0;

	size_t getSysIndex() { return _sysIndex; };
	void setSysIndex(size_t index) { _sysIndex = index; };
	size_t getEntryIndex() { return _entryIndex; };
	void setEntryIndex(size_t index) { _entryIndex = index; };

	ptree getPropertyTree() { return propertyTree; };

	RadioSystemPtr parent() { return _parent; };

private:
	std::string	_tag;
	std::atomic<LockoutType>	_lockout;
	int	_scanDelayMS;
	size_t _sysIndex = 0;
	size_t _entryIndex = 0;
	RadioSystemPtr _parent;
protected:
	piscan::sigproc::DemodInterface& demod;

	ptree propertyTree;
};

/* base class for scanner entries defined with a tunable frequency */
class Channel: public Entry {
public:
	Channel(long freq, std::string tag, bool lo, int del) : Entry(tag, lo, del), frequency(freq){
		propertyTree.put(database::entry::freq_key, freq);
	}
	virtual ~Channel() {};
	virtual long long freq() { return frequency; };
	virtual std::string key() { return "F" + std::to_string(frequency); };
protected:
	const long long frequency;

};

/* used for setting the tuner's center frequency */
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

/* base class for analog and digital channels with FM modulation, and for channels with CSQ squelch */
class FMChannel : public Channel {
public:
	FMChannel(long long freq, std::string tag, bool lo, int del) : Channel(freq, tag, lo, del){
		propertyTree.put(database::entry::channel::type_key, database::entry::channel::type_fm);
	}
	~FMChannel() {};

	virtual std::string modulation() {
		return "NFM";
	}

	virtual bool hasSignal();
	virtual std::string key() { return Channel::key() + "FM"; };
};

/* for analog FM channels utilizing CTCSS tones */
class PLChannel: public FMChannel {
public:
	PLChannel(long long freq, std::string tn, std::string tag, bool lo, int del) :
			FMChannel(freq, tag, lo, del), s_tone(tn) {
		try{
			tone = std::stof(tn);
		} catch (std::exception& e) {
			tone = 0;
		}
		propertyTree.put(database::entry::channel::type_key, database::entry::channel::type_pl);
		propertyTree.put(database::entry::tone_key, tn);
	}
	~PLChannel() {};

	//bool hasSignal();

	virtual std::string modulation() {
		return "PL" + s_tone;
	}

	virtual std::string key() { return FMChannel::key() + "PL" + s_tone; };
protected:
	std::string s_tone;
	float tone;
};

/* for analog FM channels uitlizing DCS squelch */
class DCChannel : public FMChannel {
public:
	DCChannel(long long freq, std::string tn, std::string tag, bool lo, int del) :
			FMChannel(freq, tag, lo, del), s_code(tn) {
		propertyTree.put(database::entry::channel::type_key, database::entry::channel::type_dc);
		propertyTree.put(database::entry::code_key, tn);
	}
	~DCChannel() {};

	//bool hasSignal();

	virtual std::string modulation() {
		return "DC" + s_code;
	}

	virtual std::string key() { return FMChannel::key() + "DC" + s_code; };
protected:
	const std::string s_code;
};

/* for analog AM channels, does not include SSB, DSB or CW modes */
class AMChannel : public Channel {
public:
	AMChannel(long long freq, std::string tag, bool lo, int del) : Channel(freq, tag, lo, del){
		propertyTree.put(database::entry::channel::type_key, database::entry::channel::type_am);
	}
	~AMChannel() {};

	virtual bool hasSignal();

	virtual std::string modulation() { return "AM"; }
	virtual std::string key() { return Channel::key() + "AM"; };
};


}
}
#endif /*Channel_ */
