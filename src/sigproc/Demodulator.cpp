/*
 * Demodulator.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#include <unistd.h>
#include <functional>

#include "PiScan.h"
#include "Demodulator.h"
#include "loguru.hpp"
#include "events.h"

#define INIT_FREQUENCY			100000000
#define NUM_RATES_DEFAULT	4
#define SIGLEVEL_REFRESH_INTERVAL	100 // milliseconds

namespace piscan {
namespace sigproc {

Demodulator::Demodulator() : _cubic(makeCubic()), _demodMgr(_cubic->getDemodMgr()), _tunerManager(_cubic) {};

void Demodulator::start(){
	piscan::config::DemodState& state = app::system::getState().getDemodState();
	_squelchLevel = state.squelch;
	_gain = state.gain;

	// TODO assertions will be used until a soft abort is made available
	CHECK_F(_cubic->OnInit());

	CHECK_F(_tunerManager.enumerateDevices());
	CHECK_F(_tunerManager.autoSelectTuner());
	CHECK_F(_tunerManager.startSelectedTuner());

	//sets sample rate for outputs - imported from cubic
    unsigned int desired_rates[NUM_RATES_DEFAULT] = { 48000, 44100, 96000, 192000 };

	auto outdevs = _demodMgr.getOutputDevices();

	CHECK_F(outdevs.size() > 0);

    for (auto mdevices_i = outdevs.begin(); mdevices_i != outdevs.end(); mdevices_i++) {
        unsigned int desired_rate = 0;
        unsigned int desired_rank = NUM_RATES_DEFAULT + 1;

        for (auto srate = mdevices_i->second.sampleRates.begin(); srate != mdevices_i->second.sampleRates.end();
             srate++) {
            for (unsigned int i = 0; i < NUM_RATES_DEFAULT; i++) {
                if (desired_rates[i] == (*srate)) {
                    if (desired_rank > i) {
                        desired_rank = i;
                        desired_rate = (*srate);
                    }
                }
            }
        }

        if (desired_rank > NUM_RATES_DEFAULT) {
            desired_rate = mdevices_i->second.sampleRates.back();
        }
        AudioThread::deviceSampleRate[mdevices_i->first] = desired_rate;
    }

    int audiodev = outdevs.begin()->first;

	LOG_F(INFO, "Creating modems");

	/* demodulator utilized by FMChannel-derived entries */
    DemodulatorInstancePtr newDemod = _demodMgr.newThread();
    newDemod->setLabel("NFM");
	newDemod->setDemodulatorType("NBFM");
	newDemod->setBandwidth(12500);
	newDemod->setGain(1.0);
	newDemod->setSquelchLevel(-100);
	newDemod->setMuted(true);
	newDemod->setOutputDevice(audiodev);
	newDemod->run();
	newDemod->setFrequency(INIT_FREQUENCY);
	_demods[NFM] = newDemod;
	LOG_F(INFO, "Added modem NFM");

	/*newDemod = _demodMgr.newThread();
	newDemod->setLabel("FM");
	newDemod->setDemodulatorType("NBFM");
	newDemod->setBandwidth(24000);
	newDemod->setGain(1.0);
	newDemod->setSquelchLevel(-100);
	newDemod->setMuted(true);
	newDemod->setOutputDevice(audiodev);
	newDemod->setFrequency(INIT_FREQUENCY);
	newDemod->run();
	_demods[FM] = newDemod;
	LOG_F(INFO, "Added modem FM");*/

	/* demodulator utilized by AMChannel-derived entries */
	newDemod = _demodMgr.newThread();
	newDemod->setLabel("AM");
	newDemod->setDemodulatorType("AM");
	newDemod->setBandwidth(12500);
	newDemod->setGain(1.0);
	newDemod->setSquelchLevel(-100);
	newDemod->setMuted(true);
	newDemod->setOutputDevice(audiodev);
	newDemod->setFrequency(INIT_FREQUENCY);
	newDemod->run();
	_demods[AM] = newDemod;
	LOG_F(INFO, "Added modem AM");

	//setModem(NFM);
	_demodMgr.setActiveDemodulator(_demods[NFM], false);

	//create signal level refresh timer
	std::function<void()> func([this](){
		int level = getSignalStrength();

		LOG_F(7, "Signal strength %i", level);
		//app::server::signalLevelUpdate(level);
		events::publish(std::make_shared<events::SignalLevelEvent>(level));
	});
	//_sigLevelRefresher = new IntervalTimer();
	_sigLevelRefresher.create(SIGLEVEL_REFRESH_INTERVAL, func);

	LOG_F(1, "Demodulator started");
	notifyReady();
}

void Demodulator::stop(){
	_sigLevelRefresher.stop();
	//delete _sigLevelRefresher;

	//_cubic->stopDevice(false, 2000);
	_tunerManager.stopSelectedTuner();
	_cubic->OnExit();
	
	piscan::config::DemodState& state = app::system::getState().getDemodState();
	state.gain = _gain;
	state.squelch = _squelchLevel;

	LOG_F(1, "Demodulator stopped");
	notifyDeinit();
}

bool Demodulator::setFrequency(long long freq) {
	/*if(freq == _demodMgr.getCurrentModem()->getFrequency()){
		DLOG_F(9, "Frequency already set");
		return true;
	}*/
	if(freq == _currentFreq) 
		return true;

	_demodMgr.getCurrentModem()->setFrequency(freq);

	//TODO account for bandwidth
	if(std::abs(_cubic->getFrequency() - freq) >= (_cubic->getSampleRate() / 2)){
        _cubic->setFrequency(freq);
        //also arbitrary
        //usleep(TUNER_RETUNE_TIME);
        std::this_thread::sleep_for(std::chrono::microseconds(app::system::getConfig().getDemodConfig().retuneDelay));
	}

	_demodMgr.getCurrentModem()->setFrequency(freq);
	//this is totally arbitrary
	//usleep(DEMOD_BUFFER_TIME);
	events::publish(std::make_shared<events::GenericNumberEvent>("demod_frequency_set", freq));
	std::this_thread::sleep_for(std::chrono::microseconds(app::system::getConfig().getDemodConfig().demodDelay));

	_currentFreq = freq;

	

	return true;
}

bool Demodulator::setTunerFrequency(long long freq){
    _cubic->setFrequency(freq);
	_demodMgr.getCurrentModem()->setFrequency(freq);
    //usleep(200000);
	events::publish(std::make_shared<events::GenericNumberEvent>("demod_frequency_set", freq));
	std::this_thread::sleep_for(std::chrono::microseconds(app::system::getConfig().getDemodConfig().retuneDelay));
	return true;
}

float Demodulator::getSignalLevel() {
	return _demodMgr.getActiveContextModem()->getSignalLevel();
}

float Demodulator::getDecodedPL() { return 0; }
unsigned int Demodulator::getDecodedDC() { return 0; }

bool Demodulator::squelchThresholdMet() {
	switch (app::system::getConfig().getDemodConfig().squelchType) {
	case SQUELCH_PCT:
		return (getSignalStrength() >= _squelchLevel);
	case SQUELCH_SNR:
		return (std::abs(
				_demodMgr.getActiveContextModem()->getSignalLevel()
						- _demodMgr.getActiveContextModem()->getSignalFloor())
				>= _squelchLevel);
	case SQUELCH_DBM:
	default:
		return (getSignalLevel() >= _squelchLevel); //dBm comparison
	}
}

bool Demodulator::setModem(Modulation mode) {
	if(_currentModem == mode)
		return true;

	_demods[mode]->setActive(true);
	_demodMgr.setActiveDemodulator(_demods[mode], false);
	_demods[_currentModem]->setActive(false);
	_currentModem = mode;
	return true;
}

void Demodulator::setSquelch(float level) {
	_squelchLevel = level;
	LOG_F(1, "Squelch set to %.1lf", level);
	_contextUpdate();
}

float Demodulator::getSNR() {
	float level = _demodMgr.getActiveContextModem()->getSignalLevel();
	float floor = _demodMgr.getActiveContextModem()->getSignalFloor();
	DRAW_LOG_F(7, "\t\t\tsiglevel %.1f\tfloor %.1f", level, floor);
	return (100+level)/(100+floor);
	//return (_demodMgr.getActiveContextModem()->getSignalFloor()/_demodMgr.getActiveContextModem()->getSignalLevel());
}

int Demodulator::getSignalStrength() { // uses signal level as a fraction between floor and 0dBm - unreliable
//	float fractional = getSNR() - 1;
//	int percent = 100*fractional;
//	if(percent >= 100)
//		return 100;

	float signal = _demodMgr.getActiveContextModem()->getSignalLevel();
	float floor = _demodMgr.getActiveContextModem()->getSignalFloor();
	float ceiling = 0.0;
	float range = ceiling - floor;

	int level = (100 * (signal - floor)) / range;
	if (level > 100)
		level = 100;

//	DRAW_LOG_F(7, "\t\t\tsigstrength %i", percent);
//	return percent;
	return level;
}

void Demodulator::_handleMessage(std::shared_ptr<DemodMessage> message){
	if(message->type == DemodMessage::OPEN_AUDIO){
		_demodMgr.getCurrentModem()->setMuted((bool) message->pData);
	}
}

void Demodulator::_handleRequest(ClientRequest& request){
	if(request.rqInfo.type == DEMOD_CONFIGURE){
		int* data = reinterpret_cast<int*>(request.pData);
		switch (request.rqInfo.subType) {
		case DEMOD_SET_SQUELCH:
			DCHECK_F(data != nullptr);
			_squelchLevel = *data;
			LOG_F(1, "Squelch set to %.1lf", _squelchLevel);
			delete data;
			break;
		case DEMOD_SET_GAIN:
			DCHECK_F(data != nullptr);
			
			if (*data == AUTO_GAIN){
				_cubic->setAGCMode(true);

				LOG_F(1, "Gain set to auto");
			}
			else{
				_cubic->setAGCMode(false);
				_cubic->setGain("TUNER", *data);
				LOG_F(1, "Gain set to %i", *data);
			}
			_gain = *data;
			delete data;
			break;
		default:
			break;
		}

		_contextUpdate();
	}
	else if(request.rqInfo.type == GET_CONTEXT){
		piscan::server::context::DemodContext* context = new piscan::server::context::DemodContext(_gain, _squelchLevel);
		request.connection->demodContextRequestCallback(request.rqHandle, context);
	}

	delete &request;
}

void Demodulator::_contextUpdate(){
	//app::server::demodContextUpdate(piscan::server::context::DemodContext(_gain, _squelchLevel));
	events::DemodStateEvent event;
	event.squelchState = _squelchLevel;
	event.tunerGainState = _gain;
	events::publish(std::make_shared<events::DemodStateEvent>(std::move(event)));
}

void Demodulator::setTunerGain(float gain){
	if (gain < 0)
	{
		_cubic->setAGCMode(true);
		_gain = AUTO_GAIN;
		LOG_F(1, "Gain set to auto");
	}
	else
	{
		_cubic->setAGCMode(false);
		_cubic->setGain("TUNER", gain);
		_gain = _cubic->getGain("TUNER");
		LOG_F(1, "Gain set to %.1lf", _cubic->getGain("TUNER"));
	}
	_contextUpdate();
}

float Demodulator::getTunerGain(){
	return _gain;
}

float Demodulator::getSquelch(){
	return _squelchLevel;
}

void Demodulator::squelchBreak(bool mute){
	//mute = !mute;
	mute ? _sigLevelRefresher.start() : _sigLevelRefresher.stop();

	_demodMgr.getCurrentModem()->setMuted(!mute);
}

long long Demodulator::getTunerSampleRate(){
	return _cubic->getSampleRate();
}

}
}
