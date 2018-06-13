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
#include "scan_tree.h"

#define INTERFACE_TAG	"Interface"
#define INTERFACE_SVC_TAG	"Interface Service"

typedef struct {
	void	(*init)(void);
}INFO_COMPONENTS;

typedef struct {
	void	(*init)(void);
}MODE_MENU;

typedef struct {
	void	(*init)(void);
	void	(*set_system)(RADIO_SYSTEM*);
}MODE_SCANNING;

typedef struct {
	void	(*init)(void);
	void	(*set_entry)(ENTRY*);
}MODE_SCAN_PAUSED;

typedef struct {
	int	(*thread_func)(void);
	void	(*init)(void);
	void	(*quit)(void);
	pthread_t *interface_service;
	INFO_COMPONENTS	*info_comp;
	MODE_MENU	*menu_mode;
	MODE_SCANNING	*scan_mode;
	MODE_SCAN_PAUSED	*paused_mode;
}INTERFACE;

extern INTERFACE Console_only_no_IO;
extern INTERFACE Web_and_remote;
extern INTERFACE MatrixOrbital_with_GPIO;
extern INTERFACE HD44780_with_GPIO;
extern INTERFACE vLCD_with_virtualIO;
extern INTERFACE GUI_with_GPIO;
extern INTERFACE GUI_with_touchscreen;

#endif /* SRC_INTERFACE_INTERFACE_H_ */
