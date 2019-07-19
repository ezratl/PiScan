/*
 * Entry.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include <assert.h>
#include <stddef.h>

#include "Entry.h"

using namespace piscan;

DemodInterface* Entry::demod = nullptr;

bool FMChannel::hasSignal(void){
	assert(demod != nullptr);

	demod->setModem(NFM);

	if(!demod->setFrequency(this->frequency))
		return false;

	if(demod->squelchThresholdMet())
		return true;

	return false;
}
