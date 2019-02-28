/*
 * Tuner.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef DRIVERS_TUNER_H_
#define DRIVERS_TUNER_H_

#include <stdint.h>

typedef enum {
	TUNER_ERROR = -1,
	TUNER_SUCCESS,
	TUNER_BUSY,
	TUNER_UNSUPPORTED,
} TunerStatus;

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

class Tuner {
public:
	Tuner();
	virtual ~Tuner();

	TunerStatus setFrequency(Frequency* newFreq);
	TunerStatus setSquelchLevel(int8_t newLevel);
};

class RtlTuner : public Tuner {
public:

private:
};

#endif /* DRIVERS_TUNER_H_ */
