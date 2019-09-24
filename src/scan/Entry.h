/*
 * Entry.h
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#ifndef SCAN_ENTRY_H_
#define SCAN_ENTRY_H_

#include "Demodulator.h"

#define TAG_LENGTH	20

namespace piscan {

class Entry {
public:
	Entry(std::string tag, bool lo, bool del) : _tag(tag), _lockedOut(lo), _scanDelay(del){};
	virtual ~Entry() {};

	std::string	tag() { return _tag; }
	virtual std::string	modulation() = 0;
	bool	isLockedOut() { return _lockedOut; }
	bool	useDelay() { return _scanDelay; }
	void	lockout(bool val = true) { _lockedOut = val; }
	virtual bool	hasSignal() = 0;
	virtual long freq() = 0;

	size_t getSysIndex() { return _sysIndex; };
	void setSysIndex(size_t index) { _sysIndex = index; };
	size_t getEntryIndex() { return _entryIndex; };
	void setEntryIndex(size_t index) { _entryIndex = index; };

private:
	std::string	_tag;
	bool	_lockedOut;
	bool	_scanDelay;
	size_t _sysIndex = 0;
	size_t _entryIndex = 0;
protected:
	static DemodInterface* demod;
	friend void setDemodulator(DemodInterface* demod);
};

class Channel: public Entry {
public:
	Channel(long freq, std::string tag, bool lo, bool del) : Entry(tag, lo, del), frequency(freq){}
	virtual ~Channel() {};
	virtual long freq() { return frequency; };
protected:
	const long frequency;

};

class DummyChannel: public Channel {
public:
	DummyChannel(long freq) : Channel(freq, "", false, false){

	}
	~DummyChannel(){};

	std::string modulation() { return ""; };

	bool hasSignal();
};

class FMChannel : public Channel {
public:
	FMChannel(long freq, std::string tag, bool lo, bool del) : Channel(freq, tag, lo, del){}
	~FMChannel() {};

	std::string modulation() {
		return "NFM";
	}

	bool hasSignal();
};

class PLChannel: public FMChannel {
public:
	PLChannel(long freq, float tn, std::string tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), tone(tn) {
	}
	~PLChannel() {};

	//bool hasSignal();
protected:
	const float tone;
};

class DCChannel : public FMChannel {
public:
	DCChannel(long freq, unsigned int tn, std::string tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), code(tn) {
	}
	~DCChannel() {};

	//bool hasSignal();
protected:
	const unsigned int code;
};

class AMChannel : public Channel {
public:
	AMChannel(long freq, std::string tag, bool lo, bool del) : Channel(freq, tag, lo, del){}
	~AMChannel() {};

	bool hasSignal() { return false; };
};
}
#endif /*Channel_ */
