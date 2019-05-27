/*
 * Tuner.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef DRIVERS_TUNER_H_
#define DRIVERS_TUNER_H_

#include <stdint.h>
#include <unistd.h>

#include "rtl_fm.h"
#include "loguru.hpp"

#define RTL_DEMOD_WAIT	50000
#define RTL_GAIN	-100

namespace piscan {

enum TunerStatus {
	TUNER_ERROR = -1,
	TUNER_SUCCESS,
	TUNER_BUSY,
	TUNER_UNSUPPORTED,
};

typedef enum {
	MODE_FM,
	MODE_AM
} DemodMode;

typedef struct {
	uint32_t	value;
	DemodMode	mode;
	uint32_t	bandwidth;
	int8_t		gain;
} Frequency;

class RtlFmTuner;

class Tuner {
public:
	Tuner() {};
	virtual ~Tuner() {};

	virtual TunerStatus init() { return TUNER_UNSUPPORTED; };
	virtual TunerStatus stop() { return TUNER_UNSUPPORTED; };
	virtual TunerStatus setFrequency(uint32_t freq) { return TUNER_UNSUPPORTED; };
	virtual TunerStatus setSquelchLevel(int8_t newLevel) { return TUNER_UNSUPPORTED; };
	virtual TunerStatus setGain(int level) { return TUNER_UNSUPPORTED; };
	virtual float rssi() { return 0; };
};

class RtlFmTuner : public Tuner {
public:
	RtlFmTuner() {};
	~RtlFmTuner() {};

	TunerStatus init() {
		LOG_F(2, "Starting rtl_fm");
		if(rtl_fm_init(nullptr, 0, 12000, RTL_GAIN))
			return TUNER_ERROR;
		return TUNER_SUCCESS;
	}

	TunerStatus stop() {
		LOG_F(2, "Stopping rtl_fm");
		if(rtl_fm_deinit())
			return TUNER_ERROR;
		return TUNER_SUCCESS;
	}

	TunerStatus setFrequency(uint32_t freq){
		if(freq >= RTL_MIN_FREQ && freq <= RTL_MAX_FREQ){
			if(rtl_fm_setfreq(freq))
				return TUNER_ERROR;
			usleep(RTL_DEMOD_WAIT);
			return TUNER_SUCCESS;
		}
		else
			return TUNER_UNSUPPORTED;
	}

	TunerStatus setGain(int level) {
		rtl_fm_set_gain(level * 10);
		return TUNER_SUCCESS;
	}

	float rssi() { return rtl_fm_get_rssi(); }
private:
};
}
#endif /* DRIVERS_TUNER_H_ */
