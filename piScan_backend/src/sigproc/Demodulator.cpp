/*
 * Demodulator.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#include "Demodulator.h"
#include "loguru.hpp"

void Demodulator::start(){
	DCHECK_F(_tuner.init() == TUNER_SUCCESS);

	Message* message = new ControllerMessage(DEMOD, ControllerMessage::NOTIFY_READY);
	_centralQueue.giveMessage(*message);
	LOG_F(1, "Demodulator started");
}

void Demodulator::stop(){
	if(_tuner.stop() != TUNER_SUCCESS){
		LOG_F(ERROR, "Error stopping tuner!");
	}
	Message* message = new ControllerMessage(DEMOD, ControllerMessage::NOTIFY_STOPPED);
	_centralQueue.giveMessage(*message);
	LOG_F(1, "Demodulator stopped");
}

bool Demodulator::setFrequency(uint32_t freq) {
	if(freq == _currentFreq){
		DLOG_F(9, "Frequency already set");
		return true;
	}
	if(_tuner.setFrequency(freq) == TunerStatus::TUNER_SUCCESS){
		_currentFreq = freq;
		return true;
	}

	LOG_F(ERROR, "Tuning error");
	return false;
}

float Demodulator::getRssi() {
	return _tuner.rssi();
}

float Demodulator::getDecodedPL() { return 0; }
unsigned int Demodulator::getDecodedDC() { return 0; }

bool Demodulator::squelchThresholdMet() {
	return (getRssi() >= _squelchLevel);
}

void Demodulator::giveMessage(Message& message){
	if(message.source == CLIENT) {
		_handleRequest(*(static_cast<ClientRequest*>(message.pData)));
		delete &message;
	}
	else
		_handleMessage(dynamic_cast<DemodMessage&>(message));
}

void Demodulator::_handleMessage(DemodMessage& message){
	delete &message;
}

void Demodulator::_handleRequest(ClientRequest& request){
	DCHECK_F(request.rqInfo.type == DEMOD_CONFIGURE);
	int* data = reinterpret_cast<int*>(request.pData);
	switch(request.rqInfo.subType){
	case DEMOD_SET_SQUELCH:
		DCHECK_F(data != nullptr);
		_squelchLevel = *data;
		LOG_F(1, "Squelch set to %.1lf", _squelchLevel);
		delete data;
		break;
	case DEMOD_SET_GAIN:
		DCHECK_F(data != nullptr);
		_tuner.setGain(*data);
		if(*data == AUTO_GAIN)
			LOG_F(1, "Gain set to auto");
		else
			LOG_F(1, "Gain set to %i", *data);
		delete data;
		break;
	default:
		break;
	}

	delete &request;
}
