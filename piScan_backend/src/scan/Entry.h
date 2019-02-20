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
	virtual Entry(char* tag, bool lo, bool del);
	virtual ~Entry();

	char*	getTag() { return &tag[0]; }
	virtual char*	getModulation() = 0;
	virtual char*	getIdentity() = 0;
	bool	isLockedOut() { return lockedOut; }
	void	lockout(bool val) : lockedOut(val){}
	virtual bool	hasSignal() = 0;

protected:
	char	tag[TAG_LENGTH];
	bool	lockedOut;
	bool	scanDelay;

	static DemodInterface& demod;
};

class Channel: public Entry {
public:
	virtual Channel(unsigned long freq, char* tag, bool lo, bool del) : frequency(freq){}
	virtual ~Channel();
protected:
	const unsigned long frequency;
};

class FMChannel : public Channel {
public:
	FMChannel(unsigned long freq, char* tag, bool lo, bool del) : Channel(freq, tag, lo, del){}

	char* getModulation() {
		return "FM";
	}

	bool hasSignal();
};

class PLChannel: public FMChannel {
public:
	PLChannel(unsigned long freq, float tn, char* tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), tone(tn) {
	}

	//bool hasSignal();
protected:
	const float tone;
};

class DCChannel : public FMChannel {
public:
	DCChannel(unsigned long freq, unsigned int tn, char* tag, bool lo, bool del) :
			FMChannel(freq, tag, lo, del), code(tn) {
	}

	//bool hasSignal();
protected:
	const unsigned int code;
};

class AMChannel : public Channel {
public:
	AMChannel(unsigned long freq, char* tag, bool lo, bool del) : Channel(freq, tag, lo, del){}

	bool hasSignal();
};

#endif /*Channel_ */
