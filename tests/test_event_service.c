/*
 * test_event_service.c
 *
 *  Created on: Jun 17, 2018
 *      Author: kidsr
 */

#include <stdlib.h>
#include <pthread.h>

#include "../src/debug.h"
#include "../src/event.h"

#define TEST_TAG "Event test"

int main(void){
	info(TEST_TAG, "Starting test");

	pthread_t event_thread;

	info(TEST_TAG, "Pushing pre-thread events");
	push_event(OUTPUT_EVT, NULL);
	push_event(SCANNER_EVT, NULL);
	push_event(GLOBAL_READY, NULL);

	info(TEST_TAG, "Creating thread");
	pthread_create(&event_thread, NULL, &global_event_listener_thread, NULL);
	//system("pause");

	info(TEST_TAG, "Pushing thread-time events");
	push_event(-1, NULL);
	push_event(INPUT_EVT, NULL);
	push_event(AUDIO_EVT, NULL);

	//system("pause");
	//push_event(GLOBAL_SHUTDOWN, NULL);

	//pthread_join(event_thread, NULL);

	//system("pause");

	info(TEST_TAG, "Overflowing queue");
	for(int i = 0; i < 70; ++i)
		push_event(0, NULL);

	info(TEST_TAG, "Wait");
	system("pause");
	//pthread_t event_thread_2;
	//pthread_create(&event_thread_2, NULL, global_event_listener_thread, NULL);

	push_event(GLOBAL_SHUTDOWN, NULL);

	pthread_join(event_thread, NULL);

	system("pause");
	return 0;
}
