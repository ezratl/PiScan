/*
 * event.h
 *
 *  Created on: Jun 17, 2018
 *      Author: kidsr
 */

#ifndef SRC_EVENT_H_
#define SRC_EVENT_H_

#define EVT_HANDLER_TAG	"Event handler"

#define GLOBAL_EVENT_INDEX	0
#define SCANNER_EVENT_INDEX	100 + GLOBAL_EVENT_INDEX
#define OUTPUT_EVENT_INDEX	100 + SCANNER_EVENT_INDEX
#define INPUT_EVENT_INDEX	100 + OUTPUT_EVENT_INDEX
#define AUDIO_EVENT_INDEX	100 + INPUT_EVENT_INDEX

#define MAX_EVENT_INDEX		100 + AUDIO_EVENT_INDEX

typedef enum {
	/* Global events */
	GLOBAL_EVT = GLOBAL_EVENT_INDEX,
	GLOBAL_READY,
	GLOBAL_SHUTDOWN,
	GLOBAL_RESTART_PROGRAM,
	GLOBAL_RELOAD_CONFIG,
	GLOBAL_RELOAD_SCAN_PROFILE,
	GLOBAL_CRITICAL_ERROR,
	GLOBAL_WARNING,
	GLOBAL_INFO,

	/* Scanner events */
	SCANNER_EVT = SCANNER_EVENT_INDEX,
	SCANNER_SCAN,
	SCANNER_PAUSE,
	SCANNER_RESUME,

	/* Output events */
	OUTPUT_EVT = OUTPUT_EVENT_INDEX,
	OUTPUT_SET_MODE_MENU,
	OUTPUT_SET_MODE_SCAN,
	OUTPUT_SET_MODE_PAUSED,
	OUTPUT_UPDATE_INFO,
	OUTPUT_UPDATE_DISPLAY,

	/* Input events */
	INPUT_EVT = INPUT_EVENT_INDEX,
	INPUT_BUTTON_PRESSED,
	INPUT_SERIAL_DATA_AVAILABLE,

	/* Audio events */
	AUDIO_EVT = AUDIO_EVENT_INDEX,
	AUDIO_OPEN_SQUELCH,
	AUDIO_CLOSE_SQUELCH,
	AUDIO_BEEP,


}EVENT_ID;

typedef struct {
	EVENT_ID	id;
	void	*data;
}EVENT;

int push_event(EVENT_ID evid, void *data);
void global_event_listener_thread(void);

void global_event_handler(EVENT *event);
extern void scanner_event_handler(EVENT *event);
extern void output_event_handler(EVENT *event);
extern void input_event_handler(EVENT *event);
extern void audio_event_handler(EVENT *event);


#endif /* SRC_EVENT_H_ */
