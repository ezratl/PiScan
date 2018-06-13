/*
 * piscan.c
 *
 *  Created on: May 30, 2018
 *      Author: Ezra Taimuty-Loomis
 */

/* includes */
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

//#include <librtlsdr.h>
//TODO revise or replace this dependency
#include "../../rtl-sdr/src/rtl_fm.c"

#include "debug.h"
#include "scanner.h"
#include "scan_tree.h"
#include "tuner.h"
#include "interface.h"

/* defines */


/* global variable declarations */


/* function declarations */


/* main function */
int main(void){
	debug_init(1);
	general_info("Starting PiScan");

	return 0;
}
