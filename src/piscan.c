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
#include "event.h"
#include "scanner.h"
#include "scan_tree.h"
#include "tuner.h"
#include "interface.h"

/* defines */
#define TAG	"PiScan Main"

/* global variable declarations */
pthread_t event_handler_thread;
//pthread_t interface_display_thread;
//pthread_t interface_input_thread;
pthread_t audio_thread;
pthread_t tuner_thread;
pthread_t scanner_thread;

INTERFACE primary_interface;

struct scan_profile sp;

/* function declarations */
int config_load(){
	return 0;
}

int config_save(){
	return 0;
}


/* main function */
int main(void){
	//load config

	debug_init(1);
	general_info("Starting PiScan");

	//load scan profile
	pthread_t scan_profile_load_thread;
	pthread_create(&scan_profile_load_thread, NULL, &scan_profile_load, &sp);

	//start event handler
	info(TAG, "Starting event handler");
	pthread_create(&event_handler_thread, NULL, global_event_listener_thread, NULL);

	//start interface
	info(TAG, "Starting interface");
	primary_interface.init();

	//start audio
	info(TAG, "Starting audio");


	//start tuner
	info(TAG, "Starting tuner");
	pthread_create(&tuner_thread, NULL, &rtl_fm_thread_func, NULL);

	//start scanner
	pthread_join(scan_profile_load_thread, NULL);
	info(TAG, "Starting scanner");
	scanner_init(&sp);


	return 0;
}
