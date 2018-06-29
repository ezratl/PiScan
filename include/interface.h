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

enum interface_mode {
	MODE_INITIALIZING,
	MODE_QUIT,
	MODE_MENU,
	MODE_SCAN,
	MODE_PAUSED,

};
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
struct info_components {
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

struct mode_menu {
	void	(*init)(void);
	void	(*set_option_tree)(void*);
};

struct mode_scan {
	void	(*init)(void);
	void	(*set_system)(RADIO_SYSTEM*);
};

struct mode_paused {
	void	(*init)(void);
	void	(*set_entry)(ENTRY*);
};

typedef struct {
	//void	(*broadcast_event)(enum event);
	//int	(*output_event_handler)(void);
	int	(*input_event_listener)(void);
	int	(*init)(void);
	int	(*quit)(void);
	int	(*update)(void);
	pthread_t interface_service;
	struct info_components	info_comp;
	struct mode_menu	menu_mode;
	struct mode_scan	scan_mode;
	struct mode_paused	paused_mode;
}INTERFACE;

extern INTERFACE Console_only_no_IO;
extern INTERFACE Web_and_remote;
extern INTERFACE MatrixOrbital_with_GPIO;
extern INTERFACE HD44780_with_GPIO;
extern INTERFACE vLCD_with_virtualIO;
extern INTERFACE GUI_with_GPIO;
extern INTERFACE GUI_with_touchscreen;

#endif /* SRC_INTERFACE_INTERFACE_H_ */
