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
	void	lockout(bool val) { _lockedOut = val; }
	virtual bool	hasSignal() = 0;
	virtual unsigned long freq() = 0;

private:
	std::string	_tag;
	bool	_lockedOut;
	bool	_scanDelay;

protected:
	static DemodInterface* demod;
	friend void setDemodulator(DemodInterface* demod);
};

class Channel: public Entry {
public:
	Channel(unsigned long freq, std::string tag, bool lo, bool del) : Entry(tag, lo, del), frequency(freq){}
	virtual ~Channel() {};
	virtual unsigned long freq() { return frequency; };
protected:
	const unsigned long frequency;
};

class FMChannel : public Channel {
public:
	FMChannel(unsigned long freq, std::string tag, bool lo, bool del) : Channel(freq, tag, lo, del){}
	~FMChannel() {};

	std::string modulation() {
		return "FM";
	}

	bool hasSignal();
};

class PLChannel: public FMChannel {
public:
	PLChannel(unsigned long freq, float tn, std::string tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), tone(tn) {
	}
	~PLChannel() {};

	//bool hasSignal();
protected:
	const float tone;
};

class DCChannel : public FMChannel {
public:
	DCChannel(unsigned long freq, unsigned int tn, std::string tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), code(tn) {
	}
	~DCChannel() {};

	//bool hasSignal();
protected:
	const unsigned int code;
};

class AMChannel : public Channel {
public:
	AMChannel(unsigned long freq, std::string tag, bool lo, bool del) : Channel(freq, tag, lo, del){}
	~AMChannel() {};

	bool hasSignal() { return false; };
};
}
#endif /*Channel_ */
