/*
 * Entry.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include <assert.h>
#include <stddef.h>

#include "scan_types.h"
#include "Entry.h"
#include "loguru.hpp"
#include "sigproc_types.h"

using ptree = boost::property_tree::ptree;

namespace piscan {
namespace scan {

bool DummyChannel::hasSignal(){
	if(!demod.setTunerFrequency(this->frequency))
		return false;

	return false;
}

bool FMChannel::hasSignal(void){
	demod.setModem(piscan::sigproc::NFM);

	if(!demod.setFrequency(this->frequency))
		return false;

	if(demod.squelchThresholdMet())
		return true;
	/*	LOG_F(6, "Signal checking: %lli", this->frequency);
	for(size_t squelchHits = 0; demod->squelchThresholdMet(); squelchHits++){
		if(squelchHits == SQUELCH_TRIGGER_HITS) return true;
		usleep(7000);
	}*/

	return false;
}

bool AMChannel::hasSignal(void){
	demod.setModem(piscan::sigproc::AM);

	if (!demod.setFrequency(this->frequency))
		return false;

	if (demod.squelchThresholdMet())
		return true;

	return false;
}

}
}
