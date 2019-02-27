/*
 * event.c
 *
 *  Created on: Jun 17, 2018
 *      Author: kidsr
 */

#include <stdlib.h>
#include <time.h>

#include "event.h"
#include "debug.h"

#define EVENT_QUEUE_SIZE	64

/* Events to be put into a circular FIFO queue */
static EVENT event_queue[EVENT_QUEUE_SIZE];
static int queue_length = 0;
static int queue_start = 0;

void event_set_default_handlers(){
	evt_global_handler = &global_event_handler;
	//evt_scanner_handler = &scanner_event_handler;
	evt_interface_handler = &interface_event_handler;
	//evt_audio_handler = &audio_event_handler;
}

/* Pushes event to the queue */
int event_broadcast(EVENT_ID evid, void *data){
	EVENT event = {evid, data};

	if(queue_length >= EVENT_QUEUE_SIZE){
		error(EVT_HANDLER_TAG, "Event queue full!");
		return 1;
	}

	int index = (queue_start + queue_length) % EVENT_QUEUE_SIZE;
	event_queue[index] = event;
	++queue_length;

	return 0;
}

void global_event_listener_thread(void){
	info(EVT_HANDLER_TAG, "Starting event handler");

	int run = 1;
	struct timespec time, t2;
	time.tv_nsec = 500000;
	time.tv_sec = 0;

	/* Event listener loop */
	while(run){
		if(queue_length > 0){
			//debug(EVT_HANDLER_TAG, "Event queued");
			EVENT event = event_queue[queue_start];
			if(event.id >= GLOBAL_EVT && event.id < MAX_EVENT_INDEX){
				/* Global events */
				if(event.id < SCANNER_EVT){
					if(event.id == GLOBAL_SHUTDOWN || event.id == GLOBAL_RESTART_PROGRAM)
						run = 0;
					evt_global_handler(&event);
				}
				/* Scanner events */
				else if(event.id < INTERFACE_EVT){
					debug(EVT_HANDLER_TAG, "Scanner event");
				}
				/* Interface events */
				else if(event.id < AUDIO_EVT){
					debug(EVT_HANDLER_TAG, "Interface event");
				}
				/* Audio events */
				else{
					debug(EVT_HANDLER_TAG, "Audio event");
				}
			}
			else
				debug(EVT_HANDLER_TAG, "Unknown event code");

			//event_queue[queue_start] = 0;
			queue_start = (queue_start + 1) % EVENT_QUEUE_SIZE;
			--queue_length;
		}
		else
			nanosleep(&time, &t2);
	}
}

void global_event_handler(EVENT *event){
	debug(EVT_HANDLER_TAG, "Global event");
	switch(event->id){
	case GLOBAL_READY:
		break;
	case GLOBAL_SHUTDOWN:
		debug(EVT_HANDLER_TAG, "Shutdown");
		break;
	case GLOBAL_RESTART_PROGRAM:
		break;
	case GLOBAL_RELOAD_CONFIG:
		break;
	case GLOBAL_RELOAD_SCAN_PROFILE:
		break;
	case GLOBAL_CRITICAL_ERROR:
		break;
	case GLOBAL_WARNING:
		break;
	case GLOBAL_INFO:
		break;
	default:
		break;
	}
}
