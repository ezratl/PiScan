/*
 * graphic_lcd.c
 *
 *  Created on: Jun 3, 2018
 *      Author: kidsr
 */

/*
 * TODO proper commenting
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "../graphic_lcd.h"
#include "font.h"
//#include "font_lucida16pt.h"
//#include "font_lucida8pt.h"
#include "display.h"

/* Clears the frame buffer */
void clear_frame(void *pdisp){
	DISPLAY *disp = (DISPLAY*) pdisp;
	memset(disp, 0, sizeof(disp->output_buffer));
}

/* Draw a single point */
void draw_point(void *pdisp, int x, int y){
	DISPLAY *disp = (DISPLAY*) pdisp;
	if(x >= 0 && x < disp->width && y >= 0 && y < disp->height)
		disp->output_buffer[x + (y * disp->width)] = 1;
}

/* Recursively draw characters into frame buffer */
static void add_char(DISPLAY *disp, struct text_line* line, const char text[],
		int index, int x_offset, const FONT_INFO *font) {
	int next_offset = x_offset;
	int c = text[index];

	if (c >= font->start && c <= font->end) {
		c -= font->start;
		const int *c_height = &font->height;
		const int *c_width = &font->char_descriptors[c].width;
		const int *bmap_index = &font->char_descriptors[c].loc;
		int bmap_row_ind = *bmap_index;
		int x_start = line->startX + x_offset;
		int y_start = line->startY;

		uint8_t bitmask;
		uint8_t bit;
		int buffer_index;

		/* Iterate through rows of the font bitmap */
		int x, y;
		for (y = 0; y < *c_height; ++y) {

			bitmask = 0b10000000;
			for (x = 0;
					x < *c_width && x_offset + x < line->width
							&& x_start + x < disp->width; ) {

				/* Skip row with binary 0 */
				if (!font->char_bitmaps[bmap_row_ind + x / 8]) {
					x += 8;
					continue;
				}
				buffer_index = (x_start + x) + ((y_start + y) * disp->width);

				bit = font->char_bitmaps[bmap_row_ind + x / 8] & bitmask;

				if(bit)
					disp->output_buffer[buffer_index] = 1;

				if ((x + 1) % 8 > 0)
					bitmask = bitmask >> 1;
				else
					bitmask = 0b10000000;

				++x;
			}
			bmap_row_ind += (int) ceil(*c_width / 8.0);
		}
		next_offset += *c_width + line->spacing;
	}

	/* Recursive call to the next character */
	if (index >= strlen(text) - 1|| next_offset > (line->startX + line->width)
			|| next_offset > disp->width)
		return;
	else
		add_char(disp, line, text, index + 1, next_offset, font);
}

/* Recursively look up width of the string to be drawn */
static int string_px_width(const char text[], int index, int spacing, const FONT_INFO *font){
	int width = 0;
	int c = text[index];
	if(c >= font->start && c <= font->end){
		c -= font->start;
		width = font->char_descriptors[c].width;
	}
	if(index < strlen(text) - 1)
		return width + spacing + string_px_width(text, index + 1, spacing, font);
	else
		return width;
}

/* Draws text on the given display's frame buffer */
void draw_text(void *pdisp, struct text_line* line, const char text[]){
	const FONT_INFO *font;
	switch(line->font){
	case lucida16pt:
		font = &lucidaConsole_16ptFontInfo;
		break;
	case lucida8pt:
		font = &lucidaConsole_8ptFontInfo;
		break;
	case lucida7pt:
	default:
		font = &lucidaConsole_7ptFontInfo;
	}
	DISPLAY *disp = (DISPLAY*) pdisp;

	/* Look up string width */
	int x_offset;
	if(line->layout == LAYOUT_CENTER)
		x_offset = (line->width - string_px_width(text, 0, line->spacing, font)) / 2;
	else if(line->layout == LAYOUT_RIGHT)
		x_offset = line->width - string_px_width(text, 0, line->spacing, font);
	else
		x_offset = 0;

	if(x_offset < 0)
		x_offset = 0;

	/* Begin recursive call stack */
	add_char(disp, line, text, 0, x_offset, font);
}

/* Draws a rectangle on the given display's frame buffer */
void draw_rect(void *pdisp, struct rectangle* rect){
	DISPLAY *disp = (DISPLAY*) pdisp;
	if(rect->fill){
		for(int y = rect->startY; y < (rect->startY + rect->height); ++y){
			for(int x = rect->startX; x < (rect->startX + rect->width); ++x){
				disp->output_buffer[x + (y * disp->width)] = 1;
			}
		}
		return;
	}
	//left + right sides
	for(int y = rect->startY; y < (rect->startY + rect->height); ++y){
		disp->output_buffer[rect->startX + (y * disp->width)] = 1;
		disp->output_buffer[rect->startX + rect->width - 1 + (y * disp->width)] = 1;
	}

	//top + bottom sides
	for(int x = rect->startX; x < (rect->startX + rect->width); ++x){
		disp->output_buffer[x + (rect->startY * disp->width)] = 1;
		disp->output_buffer[x + ((rect->startY + rect->height - 1) * disp->width)] = 1;
	}

}

/* Draws a level bar on the given display's frame buffer */
void draw_bar(void *pdisp, struct bar* bar, const int val){
	// FIXME fix whatever the hell is wrong with this
	DISPLAY *disp = (DISPLAY*) pdisp;
	if(val < bar->minval)
		return;

	int percent;
	if (val > bar->maxval)
		percent = 100;
	else
		percent = (100 * val) / (bar->maxval - bar->minval);

	int size;

	switch (bar->orientation) {
	case UP:
		size = bar->height * percent / 100;
		for (int x = bar->startX;
				x < bar->startX + bar->width || x >= disp->width; ++x) {
			for (int y = bar->startY + bar->height - 1;
					y >= bar->startY + bar->height - size; --y) {
				disp->output_buffer[x + (y * bar->width)] = 1;
			}
		}
		break;
	case DOWN:
		size = bar->height * percent / 100;
		for (int x = bar->startX;
				x < bar->startX + bar->width || x >= disp->width; ++x) {
			for (int y = bar->startY; y < bar->startY + size; ++y) {
				disp->output_buffer[x + (y * bar->width)] = 1;
			}
		}
		break;
	case RIGHT:
		size = bar->width * percent / 100;
		for (int y = bar->startY;
				y < bar->startY + bar->height || y >= disp->height; ++y) {
			for (int x = bar->startX; x < bar->startX + size; ++x) {
				disp->output_buffer[x + (y * bar->width)] = 1;
			}
		}
		break;
	case LEFT:
		size = bar->width * percent / 100;
		for (int y = bar->startY;
				y < bar->startY + bar->height || y >= disp->height; ++y) {
			for (int x = bar->startX + bar->width - 1;
					x >= bar->startX + bar->width - size; --x) {
				disp->output_buffer[x + (y * bar->width)] = 1;
			}
		}
		break;
	}
}

