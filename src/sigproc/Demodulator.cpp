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

#define DEFAULT_SDR_SAMPLE_RATE	2048000
#define INIT_FREQUENCY			100000000
#define NUM_RATES_DEFAULT	4
#define SIGLEVEL_REFRESH_INTERVAL	100 // milliseconds

namespace piscan {
namespace sigproc {

Demodulator::Demodulator(MessageReceiver& central) : _centralQueue(central), _cubic(makeCubic()), _demodMgr(_cubic->getDemodMgr()) {};

void Demodulator::start(){
	piscan::config::DemodState& state = app::getConfig().getDemodState();
	_squelchLevel = state.squelch;
	_gain = state.gain;

	CHECK_F(_cubic->OnInit());

	while(_cubic->areDevicesEnumerating());

	std::vector<SDRDeviceInfo*>* devs = _cubic->getDevices();

	CHECK_F(devs->size() > 0, "No SDR devices are available");

	//TODO config file for this
	LOG_F(INFO, "Auto-selecting SDR device");
	size_t i;
	for(i = 0; i < devs->size();){
		if(devs->at(i)->getDriver() != "audio" && devs->at(i)->isAvailable())
			break;

		i++;
	}

	CHECK_F(i < devs->size(), "No valid or available devices found");

	auto dev = devs->at(i);

	CHECK_F(dev->getDriver() != "audio");

	LOG_F(INFO, "Auto selected: %s", dev->getName().c_str());

	LOG_F(INFO, "Auto selecting sample rate");
	std::vector<long> srates = dev->getSampleRates(SOAPY_SDR_RX, 0);
	long srate = 0;
	for(i = 0; i < srates.size() - 1; i++){
		if(srates[i] <= DEFAULT_SDR_SAMPLE_RATE && srates[i+1] > DEFAULT_SDR_SAMPLE_RATE){
			break;
		}
	}

	srate = srates[i];
	LOG_F(INFO, "Setting device sample rate to %li", srate);

	//_cubic->setGain(name, gain_in);

	_cubic->setDevice(dev, 2000);
	_cubic->setSampleRate(srate);


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
		app::signalLevelUpdate(level);
	});
	//_sigLevelRefresher = new IntervalTimer();
	_sigLevelRefresher.create(SIGLEVEL_REFRESH_INTERVAL, func);

	//auto message = std::make_shared<ControllerMessage>(DEMOD, ControllerMessage::NOTIFY_READY);
	//_centralQueue.giveMessage(message);
	LOG_F(1, "Demodulator started");
	notifyReady();
}

void Demodulator::stop(){
	_sigLevelRefresher.stop();
	//delete _sigLevelRefresher;

	_cubic->stopDevice(false, 2000);
	_cubic->OnExit();
	
	piscan::config::DemodState& state = app::getConfig().getDemodState();
	state.gain = _gain;
	state.squelch = _squelchLevel;

	//auto message = std::make_shared<ControllerMessage>(DEMOD, ControllerMessage::NOTIFY_STOPPED);
	//_centralQueue.giveMessage(message);
	LOG_F(1, "Demodulator stopped");
	notifyDeinit();
}

bool Demodulator::setFrequency(long long freq) {
	/*if(freq == _demodMgr.getCurrentModem()->getFrequency()){
		DLOG_F(9, "Frequency already set");
		return true;
	}*/

	_demodMgr.getCurrentModem()->setFrequency(freq);

	//TODO account for bandwidth
	if(std::abs(_cubic->getFrequency() - freq) >= (_cubic->getSampleRate() / 2)){
        _cubic->setFrequency(freq);
        //also arbitrary
        //usleep(TUNER_RETUNE_TIME);
        std::this_thread::sleep_for(std::chrono::microseconds(app::getConfig().getDemodConfig().retuneDelay));
	}

	_demodMgr.getCurrentModem()->setFrequency(freq);
	//this is totally arbitrary
	//usleep(DEMOD_BUFFER_TIME);
	std::this_thread::sleep_for(std::chrono::microseconds(app::getConfig().getDemodConfig().demodDelay));

	_currentFreq = freq;



	return true;
}

bool Demodulator::setTunerFrequency(long long freq){
    _cubic->setFrequency(freq);
	_demodMgr.getCurrentModem()->setFrequency(freq);
    //usleep(200000);
	std::this_thread::sleep_for(std::chrono::microseconds(app::getConfig().getDemodConfig().retuneDelay));
	return true;
}

float Demodulator::getSignalLevel() {
	return _demodMgr.getActiveContextModem()->getSignalLevel();
}

float Demodulator::getDecodedPL() { return 0; }
unsigned int Demodulator::getDecodedDC() { return 0; }

bool Demodulator::squelchThresholdMet() {
	switch (app::getConfig().getDemodConfig().squelchType) {
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

void Demodulator::giveMessage(std::shared_ptr<Message> message){
	if(message->source == CLIENT) {
		_handleRequest(*(static_cast<ClientRequest*>(message->pData)));

	}
	else
		_handleMessage(std::dynamic_pointer_cast<DemodMessage>(message));
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
	//piscan::server::context::DemodContext* context = new piscan::server::context::DemodContext(_gain, _squelchLevel);
	//_centralQueue.giveMessage(std::make_shared<ServerMessage>(DEMOD, ServerMessage::CONTEXT_UPDATE, context));
	app::demodContextUpdate(piscan::server::context::DemodContext(_gain, _squelchLevel));
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
