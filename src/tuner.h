/*
 * tuner.h
 *
 *  Created on: Jun 3, 2018
 *      Author: kidsr
 */

#ifndef SRC_TUNER_H_
#define SRC_TUNER_H_

#include <pthread.h>

#include "../../rtl-sdr/src/rtl_fm.c"
#include "../../rtl-sdr/include/rtl-sdr.h"

typedef struct tuning_interface {
	int 	ready = 0;
	int		tuned = 0;
	struct dongle_state *dongle;
	struct demod_state *demod;
	struct controller_state *controller;
	struct output_state *output;
	pthread_t *tuner_service;
}tuner_t;

#endif /* SRC_TUNER_H_ */
