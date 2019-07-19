/*
 * Demodulator.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#ifndef SIGPROC_DEMODULATOR_H_
#define SIGPROC_DEMODULATOR_H_

#include "messages.h"
#include "request.h"
#include "clientmessage.h"
#include "CubicSDR.h"
#include "DemodulatorMgr.h"
#include "SDRDeviceInfo.h"

#define DEFAULT_SQUELCH		-60

namespace piscan {

enum Modulation {
	NFM,
	FM,
	AM
};

class DemodInterface {
public:
	virtual ~DemodInterface() {};

	virtual bool setFrequency(uint32_t freq) = 0;
	virtual float getSignalLevel() = 0;
	virtual float getDecodedPL() = 0;
	virtual unsigned int getDecodedDC() = 0;
	virtual bool squelchThresholdMet() = 0;
	virtual bool setModem(Modulation mode) = 0;
	virtual void setSquelch(float level) = 0;
	virtual float getSNR() = 0;
	virtual int getSignalStrength() = 0;
};

class Demodulator : public MessageReceiver, public DemodInterface {
public:
	Demodulator(MessageReceiver& central);
	~Demodulator() {
	};

	void start();
	void stop();

private:
	MessageReceiver& _centralQueue;
	Modulation _currentModem = NFM;
	float _squelchLevel = DEFAULT_SQUELCH;
	uint32_t _currentFreq = 0;
	float _gain = AUTO_GAIN;

	std::shared_ptr<CubicSDR> _cubic;
	DemodulatorMgr& _demodMgr;

	std::map<Modulation, DemodulatorInstancePtr> _demods;

	void giveMessage(std::shared_ptr<Message> message);
	bool setFrequency(uint32_t freq);
	float getSignalLevel();
	float getDecodedPL();
	unsigned int getDecodedDC();
	bool squelchThresholdMet();
	bool setModem(Modulation mode);
	void setSquelch(float level);
	float getSNR();
	int getSignalStrength();

	void _handleMessage(std::shared_ptr<DemodMessage> message);
	void _handleRequest(ClientRequest& request);
	void _contextUpdate();
};

}
#endif /* SIGPROC_DEMODULATOR_H_ */
