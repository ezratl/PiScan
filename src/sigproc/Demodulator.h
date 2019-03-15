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
#include "request.h"
#include "clientmessage.h"

#define DEFAULT_SQUELCH		70.0

class DemodInterface {
public:
	virtual ~DemodInterface() {};

	virtual bool setFrequency(uint32_t freq) = 0;
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
	uint32_t _currentFreq = 0;
	float _gain = AUTO_GAIN;

	void giveMessage(Message& message);
	bool setFrequency(uint32_t freq);
	float getRssi();
	float getDecodedPL();
	unsigned int getDecodedDC();
	bool squelchThresholdMet();

	void _handleMessage(DemodMessage& message);
	void _handleRequest(ClientRequest& request);
	void _contextUpdate();
};

#endif /* SIGPROC_DEMODULATOR_H_ */
