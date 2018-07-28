/*
 * event.h
 *
 *  Created on: Jun 17, 2018
 *      Author: kidsr
 */

#ifndef SRC_EVENT_H_
#define SRC_EVENT_H_

#define EVT_HANDLER_TAG	"Event handler"

#define GLOBAL_EVENT_INDEX		0
#define SCANNER_EVENT_INDEX		100 + GLOBAL_EVENT_INDEX
#define INTERFACE_EVENT_INDEX	100 + SCANNER_EVENT_INDEX
#define AUDIO_EVENT_INDEX		100 + INTERFACE_EVENT_INDEX
#define MAX_EVENT_INDEX			100 + AUDIO_EVENT_INDEX

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
	SCANNER_LOCKOUT_ENTRY,
	SCANNER_SET_PATTERN,

	/* Interface events */
	INTERFACE_EVT = INTERFACE_EVENT_INDEX,
	INTERFACE_SET_STATE_MENU,
	INTERFACE_SET_STATE_SCAN,
	INTERFACE_SET_STATE_PAUSED,
	INTERFACE_SET_STATE_STEALTH,
	INTERFACE_EXIT_STATE_STEALTH,
	INTERFACE_UPDATE_INFO,
	INTERFACE_UPDATE_DISPLAY,
	INTERFACE_SET_SYSTEM,
	INTERFACE_SET_ENTRY,

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

void (*evt_global_handler)(EVENT*);
void (*evt_scanner_handler)(EVENT*);
void (*evt_interface_handler)(EVENT*);
void (*evt_audio_handler)(EVENT*);

void event_set_default_handlers();
int event_broadcast(EVENT_ID evid, void *data);
void global_event_listener_thread(void);

void global_event_handler(EVENT *event);
extern void scanner_event_handler(EVENT *event);
extern void interface_event_handler(EVENT *event);
extern void audio_event_handler(EVENT *event);


#endif /* SRC_EVENT_H_ */
