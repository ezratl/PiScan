/*
 * Entry.h
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#ifndef SCAN_ENTRY_H_
#define SCAN_ENTRY_H_

#define TAG_LENGTH	20

class Entry {
public:
	virtual Entry();
	virtual ~Entry();

	char*	getTag();
	virtual char*	getModulation();
	virtual char*	getIdentity();
	bool	isLockedOut();
	void	lockout();
	virtual bool	hasSignal();

protected:
	char	tag[TAG_LENGTH];
	bool	lockout;
	bool	scanDelay;
};

class Channel: public Entry {
public:
	virtual Channel();
	virtual ~Channel();
protected:
	const unsigned long freq;
};

class FMChannel : public Channel {

};

class PLChannel: public FMChannel {
protected:
	const float tone;
};

class DCChannel : public FMChannel {

};

class AMChannel : public Channel {

};

#endif /*Channel_ */
