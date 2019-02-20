/*
 * Entry.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include <assert.h>
#include <stddef.h>

#include "Entry.h"

bool FMChannel::hasSignal(void){
	assert(demod != NULL);

	if(!demod.setFrequency(this->frequency))
		return false;

	if(demod.squelchThresholdMet())
		return true;

	return false;
}
