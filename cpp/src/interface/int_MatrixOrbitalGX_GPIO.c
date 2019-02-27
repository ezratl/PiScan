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

#define BUTTONS_WIDTH	46
#define BUTTONS_HEIGHT	10
#define BUTTONS_YPOS	54
#define BUTTON1_XPOS	1
#define BUTTON2_XPOS	50
#define BUTTON3_XPOS	98
#define BUTTON4_XPOS	146
#define BUTTON5_XPOS	194

struct button_function {
	void	(*on_pressed)(void);
	int useIcon;
	const char	label[];
};

struct button {
	TEXT_LINE	label_component;
	ICON	icon_component;
	RECTANGLE	background;
	POINT	lcorner;
	POINT	rcorner;
	struct button_function	*function;
};

struct interrupt_handlers {
	void	(*button1)(void);
	void	(*button2)(void);
	void	(*button3)(void);
	void	(*button4)(void);
	void	(*button5)(void);
	void	(*leftknob_up)(void);
	void	(*leftknob_down)(void);
	void	(*leftknob_button)(void);
	void	(*rightknob_up)(void);
	void	(*rightknob_down)(void);
	void	(*rightknob_button)(void);
};

DISPLAY *disp = &disp_MatrixOrbitalGX;



struct interrupt_handlers state_init_handlers;
struct interrupt_handlers state_scan_handlers;
struct interrupt_handlers state_paused_handlers;

int MOGX_GPIO_update();

/* general layout vars */
int dynamic_region_bounds[2][2] = {
	{0, 0},
	{200, 53}
};


void MOGX_GPIO_draw_static_layout(){

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
	disp->init(&disp);

	return 0;
}

int MOGX_GPIO_quit(){
	disp->quit();

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
};

