/*
 * Demodulator.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#ifndef SIGPROC_DEMODULATOR_H_
#define SIGPROC_DEMODULATOR_H_

#include "messages.h"
#include "Tuner.h"

#define DEFAULT_SQUELCH		65.0

class DemodInterface {
public:
	virtual ~DemodInterface() {};

	virtual bool setFrequency(unsigned long freq) = 0;
	virtual float getRssi() = 0;
	virtual float getDecodedPL() = 0;
	virtual unsigned int getDecodedDC() = 0;
	virtual bool squelchThresholdMet() = 0;
};

class Demodulator : public MessageReceiver, public DemodInterface {
public:
	Demodulator(MessageReceiver& central) : _centralQueue(central), _tuner(*(new RtlFmTuner())) {
		//_tuner = *(new RtlFmTuner());
	};
	~Demodulator() {
		delete &_tuner;
	};

	void start();
	void stop();

private:
	MessageReceiver& _centralQueue;
	Tuner& _tuner;
	float _squelchLevel = DEFAULT_SQUELCH;

	void giveMessage(Message& message) {};
	bool setFrequency(unsigned long freq);
	float getRssi();
	float getDecodedPL();
	unsigned int getDecodedDC();
	bool squelchThresholdMet();
};

#endif /* SIGPROC_DEMODULATOR_H_ */
