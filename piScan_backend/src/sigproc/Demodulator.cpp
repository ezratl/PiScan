/*
 * Demodulator.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#include "Demodulator.h"
#include "loguru.hpp"

void Demodulator::start(){
	if(_tuner.init() != TUNER_SUCCESS){
		LOG_F(ERROR, "Error starting tuner!");
	}
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

bool Demodulator::setFrequency(unsigned long freq) {
	if(_tuner.setFrequency(freq) == TunerStatus::TUNER_SUCCESS)
		return true;
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
