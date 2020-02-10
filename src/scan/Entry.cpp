/*
 * Entry.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include <assert.h>
#include <stddef.h>

#include "Entry.h"
#include "loguru.hpp"

using namespace piscan;

DemodInterface* Entry::demod = nullptr;

bool DummyChannel::hasSignal(){
	if(!demod->setTunerFrequency(this->frequency))
		return false;

	return false;
}

bool FMChannel::hasSignal(void){
	assert(demod != nullptr);

	demod->setModem(NFM);

	if(!demod->setFrequency(this->frequency))
		return false;

	if(demod->squelchThresholdMet())
		return true;
	/*	LOG_F(6, "Signal checking: %lli", this->frequency);
	for(size_t squelchHits = 0; demod->squelchThresholdMet(); squelchHits++){
		if(squelchHits == SQUELCH_TRIGGER_HITS) return true;
		usleep(7000);
	}*/

	return false;
}

bool AMChannel::hasSignal(void){
	assert(demod != nullptr);

	demod->setModem(AM);

	if (!demod->setFrequency(this->frequency))
		return false;

	if (demod->squelchThresholdMet())
		return true;

	return false;
}
