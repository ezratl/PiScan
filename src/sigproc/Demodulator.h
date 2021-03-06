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
#include "messages/context.h"
#include "sigproc_types.h"
#include "CubicSDR.h"
#include "DemodulatorMgr.h"
#include "SDRDeviceInfo.h"
#include "Configuration.h"
#include "synchronize.h"
#include "IntervalTimer.h"
#include "TunerManager.h"


namespace piscan {
namespace sigproc {

class Demodulator : public DemodInterface, public Synchronizable {
public:
	Demodulator();
	~Demodulator() {
	};

	void start();
	void stop();

	bool setFrequency(long long freq);
	bool setTunerFrequency(long long freq);
	float getSignalLevel();
	float getDecodedPL();
	unsigned int getDecodedDC();
	bool squelchThresholdMet();
	bool setModem(Modulation mode);
	void setSquelch(float level);
	float getSNR();
	int getSignalStrength();
	void setTunerGain(float gain);
	float getTunerGain();
	float getSquelch();
	void squelchBreak(bool mute);
	long long getTunerSampleRate();

private:
	Modulation _currentModem = NFM;
	float _squelchLevel = DEFAULT_SQUELCH;
	long long _currentFreq = 0;
	std::atomic<float> _gain;

	std::shared_ptr<CubicSDR> _cubic;
	DemodulatorMgr& _demodMgr;
	TunerManager _tunerManager;

	std::map<Modulation, DemodulatorInstancePtr> _demods;

	IntervalTimer _sigLevelRefresher;

	void _handleMessage(std::shared_ptr<DemodMessage> message);
	void _handleRequest(ClientRequest& request);
	void _contextUpdate();
};

}
}
#endif /* SIGPROC_DEMODULATOR_H_ */
