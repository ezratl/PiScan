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
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

void analog_scan(const RADIO_SYSTEM *sys){
	for(int cur = 0; cur < sys->channel_count; cur++) {
		// TODO code for updating interface
		fprintf(stderr, "Scanning %s\n", sys->tag);

		// TODO thread sync
		//tuner->dongle->freq = sys->channels[cur]->freq;

		switch(sys->channels[cur]->mode){
		case AM:
			//tuner->demod->mode_demod = &am_demod;
			break;
		case NFM:
		case FM:
		default:
			//tuner->demod->mode_demod = &fm_demod;
			break;
		}

		//while(!tuner->tuned);

		//if(tuner->demod->)

	}
}

void p25_scan(RADIO_SYSTEM *sys){
	// TODO for future implementation
}

void dmr_scan(RADIO_SYSTEM *sys){
	// TODO for future implementation
}

