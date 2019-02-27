/*
 * Demodulator.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#ifndef SIGPROC_DEMODULATOR_H_
#define SIGPROC_DEMODULATOR_H_

#include "messages.h"

class DemodInterface {
public:
	virtual ~DemodInterface() {};

	virtual bool setFrequency(unsigned long freq) = 0;
	virtual int getRssi() = 0;
	virtual float getDecodedPL() = 0;
	virtual unsigned int getDecodedDC() = 0;
	virtual bool squelchThresholdMet() = 0;
};

class Demodulator : public MessageReceiver, public DemodInterface {
public:
	Demodulator() {};
	~Demodulator() {};

private:
	void giveMessage(Message& message) {};
	bool setFrequency(unsigned long freq) { return false; };
	int getRssi() { return 0; };
	float getDecodedPL() { return 0; };
	unsigned int getDecodedDC() { return 0; };
	bool squelchThresholdMet() { return false; };
};

#endif /* SIGPROC_DEMODULATOR_H_ */
