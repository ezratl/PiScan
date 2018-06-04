/*
 * scanner.h
 *
 *  Created on: Jun 3, 2018
 *      Author: kidsr
 */

#ifndef SRC_SCANNER_H_
#define SRC_SCANNER_H_

void analog_scan(const struct system *sys, struct tuning_interface *tuner);
void p25_scan(struct system *sys);
void dmr_scan(struct system *sys);

#endif /* SRC_SCANNER_H_ */
