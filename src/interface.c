/*
 * interface.c
 *
 *  Created on: Jun 13, 2018
 *      Author: kidsr
 */

#include <stdlib.h>

#include "interface.h"

int output_event_queued, input_event_queued;
enum interface_mode mode;
//enum interface_output_event event;
INTERFACE *iface;

/*void interface_broadcast_output_event(enum interface_output_event evt){
	output_event_queued = 1;
	//event = evt;
}*/

int init_interface_service(INTERFACE *iface_p){
	if(!iface_p)
		return 1;

	iface = iface_p;
	//if(!iface->broadcast_event)
		//iface->broadcast_event = &interface_broadcast_output_event;

	//if(!iface->output_event_listener)
		//iface->output_event_listener = &interface_event_listener_thread;

	info(INTERFACE_TAG, "Interface service initialized");
	return 0;
}

void output_event_handler(EVENT *event){
	//info(INTERFACE_SVC_TAG, "Starting service");

	//while(mode != MODE_QUIT){
		//if(output_event_queued){
			switch(event->id){
			case GLOBAL_SHUTDOWN:
				mode = MODE_QUIT;
				break;

			case OUTPUT_SET_MODE_MENU:
				if(mode != MODE_MENU)
					iface->menu_mode.init();
				iface->menu_mode.set_option_tree(NULL); //TODO specify parameter
				mode = MODE_MENU;
				break;

			case OUTPUT_SET_MODE_SCAN:
				if(mode != MODE_SCAN)
					iface->scan_mode.init();
				iface->scan_mode.set_system(NULL); //TODO specify parameter
				mode = MODE_SCAN;
				break;

			case OUTPUT_SET_MODE_PAUSED:
				if(mode != MODE_PAUSED)
					iface->paused_mode.init();
				iface->paused_mode.set_entry(NULL); //TODO specify parameter
				mode = MODE_PAUSED;
				break;

			case OUTPUT_UPDATE_INFO:
				iface->info_comp.init();

				break;

			case OUTPUT_UPDATE_DISPLAY:
				iface->update();
				break;

			default:
				debug(INTERFACE_SVC_TAG, "Unknown event code");
			}

			//output_event_queued = 0;
		//}
		//else
			//sleep(1);
	//}

	//info(INTERFACE_SVC_TAG, "Stopping service");
	//service stop procedure

	//return 0;
}
