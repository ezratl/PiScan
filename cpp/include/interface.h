/*
 * interface.h
 *
 *  Created on: Jun 10, 2018
 *      Author: kidsr
 */

#ifndef SRC_INTERFACE_INTERFACE_H_
#define SRC_INTERFACE_INTERFACE_H_

#include <stdlib.h>
#include <pthread.h>

#include "debug.h"
#include "event.h"
#include "scan_tree.h"

#define INTERFACE_TAG	"Interface"
#define INTERFACE_SVC_TAG	"Interface Service"

#define MAXIMUM_INTERFACES	4
#define MIN_DISPLAY_RSSI	-75
#define MAX_DISPLAY_RSSI	0

typedef enum _interface_state {
	INTERFACE_STATE_NONE,
	INTERFACE_STATE_INITIALIZING,
	INTERFACE_STATE_QUIT,
	INTERFACE_STATE_ERROR,
	INTERFACE_STATE_MENU,
	INTERFACE_STATE_SCAN,
	INTERFACE_STATE_PAUSED,
	INTERFACE_STATE_STEALTH,

}INTERFACE_STATE;
/*
enum interface_output_event {
	EVENT_QUIT,
	EVENT_SET_MODE_MENU,
	EVENT_SET_MODE_SCAN,
	EVENT_SET_MODE_PAUSED,
	EVENT_UPDATE_INFO,
	EVENT_UPDATE_DISPLAY,

};

enum interface_input_event {
	EVENT_BUTTON_PRESSED,
	EVENT_SERIAL_DATA_AVAILABLE,

};
*/
struct info_elements {
	void	(*init)(void);
	void	(*update_info)(void);
	int	current_rssi;
	int	current_freq;
	int	notification_status;
	int	sys_update_status;
	int	internet_status;
	char	*ip_address;
	int	bluetooth_status;
};

struct state_menu {
	void	(*init)(void);
	void	(*set_option_tree)(void*);
};

struct state_scan {
	void	(*init)(void);
	void	(*set_system)(RADIO_SYSTEM*);
};

struct state_paused {
	void	(*init)(void);
	void	(*set_entry)(ENTRY*);
};

struct state_stealth {
	void	(*init)(void);
};

typedef struct {
	INTERFACE_STATE		state;
	int	(*input_event_listener)(void);
	int	(*init)(void);
	int	(*quit)(void);
	int	(*update)(void);
	pthread_t interface_service;
	struct info_elements	info_elmnt;
	struct state_menu	menu_state;
	struct state_scan	scan_state;
	struct state_paused	paused_state;
	struct state_stealth	stealth_state;
}INTERFACE;

typedef struct {
	INTERFACE	*interface;
	void	*data;
}INTERFACE_EVENT_DATA;

extern INTERFACE Console_only_no_IO;
extern INTERFACE Web_and_remote;
extern INTERFACE MatrixOrbital_with_GPIO;
extern INTERFACE HD44780_with_GPIO;
extern INTERFACE vLCD_with_virtualIO;
extern INTERFACE GUI_with_GPIO;
extern INTERFACE GUI_with_touchscreen;

/* Generic input handler prototypes */
// Global event triggers
void input_request_shutdown();
void input_request_restart();
void input_reload_config();
void input_reload_scan_profile();
// Scanner event triggers
void input_start_scan();
void input_scan_pause();
void input_scan_resume();
void input_lockout_entry();
// Interface event triggers
void input_open_menu(/* context data request */);


#endif /* SRC_INTERFACE_INTERFACE_H_ */
