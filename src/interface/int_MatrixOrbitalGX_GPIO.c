/*
 * int_MatrixOrbitalGX_GPIO.c
 *
 *  Created on: Jun 10, 2018
 *      Author: kidsr
 */

/*
 *
 */

#include <stdlib.h>

#include "../interface.h"

//prototypes
INFO_COMPONENTS infocmp;
MODE_MENU mode_menu;
MODE_SCANNING mode_scanning;
MODE_SCAN_PAUSED mode_paused;



int MOGX_interface_thread_fn(){
	return 0;
}


INFO_COMPONENTS infocmp = {

};

MODE_MENU mode_menu = {

};

MODE_SCANNING mode_scanning = {

};

MODE_SCAN_PAUSED mode_paused = {

};

INTERFACE MatrixOrbital_with_GPIO = {
		.info_comp = &info,
		.menu_mode = &mode_menu,
		.scan_mode = &mode_scanning,
		.paused_mode = &mode_paused,
};

