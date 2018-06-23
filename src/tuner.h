/*
 * tuner.h
 *
 *  Created on: Jun 3, 2018
 *      Author: kidsr
 */

#ifndef SRC_TUNER_H_
#define SRC_TUNER_H_

#include <pthread.h>

//#include "../../rtl-sdr/src/rtl_fm.c"
#include "../../rtl-sdr/include/rtl-sdr.h"
#include "scan_tree.h"

//pthread_t tuner_thread;

int tune_to(const CHANNEL *chan);
int read_rssi();

int rtl_fm_thread_func();

#endif /* SRC_TUNER_H_ */
