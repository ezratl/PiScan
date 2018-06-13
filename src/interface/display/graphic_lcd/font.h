/*
 * font.h
 *
 *  Created on: Jun 9, 2018
 *      Author: kidsr
 */

#ifndef SRC_DISPLAY_GRAPHIC_LCD_FONT_H_
#define SRC_DISPLAY_GRAPHIC_LCD_FONT_H_

#include <stdlib.h>
#include <stdint.h>

//typedef struct fontcharinfo FONT_CHAR_INFO;
//typedef struct fontinfo FONT_INFO;
typedef struct fontcharinfo {
	int width;
	int loc;
} FONT_CHAR_INFO;

typedef struct fontinfo {
	const int	height;
	const char	start;
	const char	end;
	const FONT_CHAR_INFO	char_descriptors[255];
	const uint8_t	char_bitmaps[];
} FONT_INFO;

//#include "font_lucida8pt.h"
extern const FONT_INFO lucidaConsole_8ptFontInfo;

//#include "font_lucida16pt.h"
extern const FONT_INFO lucidaConsole_16ptFontInfo;

extern const FONT_INFO lucidaConsole_7ptFontInfo;

#endif /* SRC_DISPLAY_GRAPHIC_LCD_FONT_H_ */
