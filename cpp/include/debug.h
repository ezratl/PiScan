/*
 * debug.h
 *
 *  Created on: Jun 11, 2018
 *      Author: kidsr
 */

#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

//int do_exit = 0;

int debug_init();
int debug_quit();
void general_info(char message[]);
void info(char tag[], char message[]);
void debug(char tag[], char message[]);
void error(char tag[], char message[]);
void error_report(char tag[], char source[], char message[]);

#endif /* SRC_DEBUG_H_ */
