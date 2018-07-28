/*
 * interface.c
 *
 *  Created on: Jun 13, 2018
 *      Author: kidsr
 */

#include <stdlib.h>

#include "interface.h"

int output_event_queued, input_event_queued;
//INTERFACE_STATE state;
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

void interface_event_handler(EVENT *event) {
	INTERFACE_EVENT_DATA *context = event->data;

	switch (event->id) {
	if (context->interface->state != INTERFACE_STATE_STEALTH) {
		case GLOBAL_SHUTDOWN:
		context->interface->state = INTERFACE_STATE_QUIT;
		break;

		case INTERFACE_SET_STATE_MENU:
		if (context->interface->state != INTERFACE_STATE_MENU)
			context->interface->menu_state.init();
		context->interface->menu_state.set_option_tree(context->data);
		context->interface->state = INTERFACE_STATE_MENU;
		break;

		case INTERFACE_SET_STATE_SCAN:
		if (context->interface->state != INTERFACE_STATE_SCAN)
			context->interface->scan_state.init();
		context->interface->scan_state.set_system(context->data);
		context->interface->state = INTERFACE_STATE_SCAN;
		break;

		case INTERFACE_SET_STATE_PAUSED:
		if (context->interface->state != INTERFACE_STATE_PAUSED)
			context->interface->paused_state.init();
		context->interface->paused_state.set_entry(context->data);
		context->interface->state = INTERFACE_STATE_PAUSED;
		break;

		case INTERFACE_SET_STATE_STEALTH:
		context->interface->stealth_state.init();
		context->interface->state = INTERFACE_STATE_STEALTH;
		break;
	}
case INTERFACE_EXIT_STATE_STEALTH:
	context->interface->state = INTERFACE_STATE_NONE;
	break;

case INTERFACE_UPDATE_INFO:
	context->interface->info_elmnt.init();

	break;

case INTERFACE_UPDATE_DISPLAY:
	context->interface->update();
	break;

default:
	debug(INTERFACE_SVC_TAG, "Unknown event code");
	}

}

