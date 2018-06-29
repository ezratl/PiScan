/*
 * scanner.h
 *
 *  Created on: Jun 3, 2018
 *      Author: kidsr
 */

#ifndef SRC_SCANNER_H_
#define SRC_SCANNER_H_

#include <pthread.h>

#include "interface.h"
//#include "tuner.h"

#define SCANNER_TAG	"Scanner"
#define SCANNER_SVC_TAG	"Scanner Service"

pthread_t *scanner_service;

int scanner_init(void *p_profile);
int scanner_thread_fn();


void analog_scan(const struct system *sys);
void p25_scan(struct system *sys);
void dmr_scan(struct system *sys);

#endif /* SRC_SCANNER_H_ */
