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

#include "interface.h"
#include "interface/graphic_lcd/display.h"
#include "interface/graphic_lcd.h"

DISPLAY *disp = &disp_MatrixOrbitalGX;

int MOGX_GPIO_update();

/* general layout vars */
int dynamic_region_bounds[2][2] = {
	{0, 0},
	{200, 53}
};

RECTANGLE layout_softbutton_horiz_divider = {

};
TEXT_LINE layout_softbutton_1 = {

};
RECTANGLE layout_softbutton_divider_12 = {

};
TEXT_LINE layout_softbutton_2 = {

};
RECTANGLE layout_softbutton_divider_23 = {

};
TEXT_LINE layout_softbutton_3 = {

};
RECTANGLE layout_softbutton_divider_34 = {

};
TEXT_LINE layout_softbutton_4 = {

};
RECTANGLE layout_softbutton_divider_45 = {

};
TEXT_LINE layout_softbutton_5 = {

};
void MOGX_GPIO_draw_static_layout(){
	draw_rect(disp, &layout_softbutton_horiz_divider);
	draw_rect(disp, &layout_softbutton_divider_12);
	draw_rect(disp, &layout_softbutton_divider_23);
	draw_rect(disp, &layout_softbutton_divider_34);
	draw_rect(disp, &layout_softbutton_divider_45);
}

/* menu mode vars */
TEXT_LINE menu_line_1 = {

};
RECTANGLE menu_line_1_highlight = {

};
TEXT_LINE menu_line_2 = {

};
RECTANGLE menu_line_2_highlight = {

};
TEXT_LINE menu_line_3 = {

};
RECTANGLE menu_line_3_highlight = {

};
TEXT_LINE menu_line_4 = {

};
RECTANGLE menu_line_4_highlight = {

};

/* scan mode vars */
RADIO_SYSTEM *cur_sys;
TEXT_LINE scan_system_line = {

};
TEXT_LINE scan_info_line = {

};
void MOGX_GPIO_scan_init() {
	clear_region(disp, dynamic_region_bounds[0][0], dynamic_region_bounds[0][1], dynamic_region_bounds[1][0], dynamic_region_bounds[1][1], 0);
	draw_text(disp, &scan_info_line, "Scanning...");
};
void MOGX_GPIO_scan_set_system(RADIO_SYSTEM *sys) {
	cur_sys = sys;
	draw_text(disp, &scan_system_line, sys->tag);
	MOGX_GPIO_update();
}

/* paused mode vars */
TEXT_LINE paused_tag_line = {

};
TEXT_LINE paused_system_line = {

};
TEXT_LINE paused_freq_line = {

};
BAR paused_signal_indicator = {
		.minval = MIN_DISPLAY_RSSI,
		.maxval = MAX_DISPLAY_RSSI,

};
void MOGX_GPIO_paused_init(){
	clear_region(disp, dynamic_region_bounds[0][0], dynamic_region_bounds[0][1], dynamic_region_bounds[1][0], dynamic_region_bounds[1][1], 0);
}
void MOGX_GPIO_paused_set_entry(ENTRY *entry){
	CHANNEL *chan;
	switch(entry->type){
	case ANALOG:
	default:
		chan = (CHANNEL*) entry;
		draw_text(disp, &paused_tag_line, chan->tag);
		draw_text(disp, &paused_system_line, cur_sys->tag);
	}
}

int MOGX_GPIO_init(){
	return 0;
}

int MOGX_GPIO_quit(){
	return 0;
}

int MOGX_GPIO_update(){
	if(disp->update()){
		error(INTERFACE_TAG, "Display update");
		return 1;
	}
	return 0;
}

int MOGX_GPIO_input_listener(){
	return 0;
}

INTERFACE MatrixOrbital_with_GPIO = {
		//.broadcast_event = &interface_broadcast_output_event,
		.input_event_listener = &MOGX_GPIO_input_listener,
		//.output_event_listener = &interface_event_listener_thread,
		.init = &MOGX_GPIO_init,
		.update = &MOGX_GPIO_update,
		.quit = &MOGX_GPIO_quit,
		.info_comp = {

		},
		.menu_mode = {

		},
		.scan_mode = {

		},
		.paused_mode = {

		},
};

