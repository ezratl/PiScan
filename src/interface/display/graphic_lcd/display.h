/*
 * display.h
 *
 *  Created on: Jun 9, 2018
 *      Author: kidsr
 */
#include <stdint.h>

#include "../../../debug.h"

#define DISPLAY_TAG	"Display driver"

#ifndef SRC_DISPLAY_GRAPHIC_LCD_DISPLAY_H_
#define SRC_DISPLAY_GRAPHIC_LCD_DISPLAY_H_

typedef struct display {
	int height;
	int width;
	uint8_t backlight[3];
	int	(*init)(void*);
	int (*quit)(void);
	int (*update)(void);
	unsigned char *output_buffer;
}DISPLAY;

DISPLAY pipe_fifo;

#endif /* SRC_DISPLAY_GRAPHIC_LCD_DISPLAY_H_ */
