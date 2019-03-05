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

class Entry {
public:
	Entry(std::string tag, bool lo, bool del);
	virtual ~Entry() {};

	std::string*	getTag() { return &tag; }
	virtual std::string	getModulation() = 0;
	virtual std::string*	getIdentity() = 0;
	bool	isLockedOut() { return lockedOut; }
	bool	useDelay() { return scanDelay; }
	void	lockout(bool val) { lockedOut = val; }
	virtual bool	hasSignal() = 0;

protected:
	std::string	tag;
	bool	lockedOut;
	bool	scanDelay;

	static DemodInterface* demod;
	friend void setDemodulator(DemodInterface* demod);
};

class Channel: public Entry {
public:
	Channel(unsigned long freq, char* tag, bool lo, bool del) : Entry(tag, lo, del), frequency(freq){}
	virtual ~Channel() {};
protected:
	const unsigned long frequency;
};

class FMChannel : public Channel {
public:
	FMChannel(unsigned long freq, char* tag, bool lo, bool del) : Channel(freq, tag, lo, del){}
	~FMChannel() {};

	std::string getModulation() {
		return "FM";
	}

	bool hasSignal();
};

class PLChannel: public FMChannel {
public:
	PLChannel(unsigned long freq, float tn, char* tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), tone(tn) {
	}
	~PLChannel() {};

	//bool hasSignal();
protected:
	const float tone;
};

class DCChannel : public FMChannel {
public:
	DCChannel(unsigned long freq, unsigned int tn, char* tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), code(tn) {
	}
	~DCChannel() {};

	//bool hasSignal();
protected:
	const unsigned int code;
};

class AMChannel : public Channel {
public:
	AMChannel(unsigned long freq, char* tag, bool lo, bool del) : Channel(freq, tag, lo, del){}
	~AMChannel() {};

	bool hasSignal() { return false; };
};

#endif /*Channel_ */
