/*
 * test_graphic_lcd.h
 *
 *  Created on: Jun 9, 2018
 *      Author: kidsr
 */

/*
 * Graphic LCD library test program
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interface/graphic_lcd.h"
#include "interface/graphic_lcd/display.h"
#include "interface/graphic_lcd/font.h"


int main(){
	DISPLAY disp = pipe_fifo;
	disp.height = 64;
	disp.width = 240;

	if(!disp.init(&disp))
		printf("FIFO open error!\n");
	else
		printf("FIFO opened\n");

	RECTANGLE rect = { 14, 14, 20, 122, 1 };
	RECTANGLE rect_2 = { 220, 10, 20, 10, 1 };
	RECTANGLE rect_3 = { 150, 15, 8, 50, 0};
	BAR bar = { 150, 15, 8, 50, 0, 100, DOWN };
	TEXT_LINE text = { .startX = 15, .startY = 15, .width = 120, .spacing = 1, .font =lucida16pt, LAYOUT_RIGHT};
	TEXT_LINE text_2 = { 0, 0, 239, 2, lucida7pt, LAYOUT_LEFT };
	TEXT_LINE text_3 = { 0, 40, 239, 1, lucida8pt, LAYOUT_CENTER };
	TEXT_LINE text_4 = { 0, 50, 120, 1, lucida7pt, LAYOUT_LEFT };

	draw_rect(&disp, &rect);
	draw_text(&disp, &text, "hello world!");

	draw_text(&disp, &text_2, "abcdefghijklmnopqrstuvwxyz<>,.'\"");
	draw_text(&disp, &text_3, "`1234567890-=~!@#$%^&*()_+[]{}|\\/?><.,");
	draw_text(&disp, &text_4, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	draw_rect(&disp, &rect_3);
	draw_bar(&disp, &bar, 75);

	//disp.update();


	//system("pause");
	//disp.update();

	//system("pause");
	draw_rect(&disp, &rect_2);
	disp.update();
	system("pause");
	disp.quit();

	return 0;
}


