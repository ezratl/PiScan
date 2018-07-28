/*
 * scanner.c
 *
 *  Created on: May 31, 2018
 *      Author: Ezra Taimuty-Loomis
 */

/*
 * Implementation of scanning procedures for each system type
 */

/* Includes */
#include <stdlib.h>

//#include "../../rtl-sdr/src/rtl_fm.c"
//#include "../../rtl-sdr/include/rtl-sdr.h"

#include "scan_tree.h"
#include "scanner.h"
#include "debug.h"
#include "tuner.h"

/* Constants */
#define SCAN_DELAY_TIMEOUT	2000 //2000ms timeout

/* Global variables */


/* Methods */
/*void broadcast_scan_event(enum scan_event){

}*/

void analog_scan(const Scan_System_t *sys){
	for(int cur = 0; cur < sys->channel_count; cur++) {


	}
}

void p25_scan(Scan_System_t *sys){
	// TODO for future implementation
}

void dmr_scan(Scan_System_t *sys){
	// TODO for future implementation
}

void system_scan(Scan_System_t* sys) {
	if(!sys->lockout){
		switch(sys->type){

		}
	}
}

void scanner_run(){

}
