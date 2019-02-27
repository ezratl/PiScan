/*
 * graphic_lcd.h
 *
 *  Created on: Jun 6, 2018
 *      Author: kidsr
 */

/*
 * todo proper commenting
 */

#ifndef SRC_DISPLAY_GRAPHIC_LCD_GRAPHIC_LCD_H_
#define SRC_DISPLAY_GRAPHIC_LCD_GRAPHIC_LCD_H_

#include <stdlib.h>
#include "graphic_lcd/font.h"
//#include "display.h"

enum layout {
	LAYOUT_LEFT = 0, LAYOUT_CENTER, LAYOUT_RIGHT
};

enum font_name {
	lucida8pt = 0, lucida16pt, lucida7pt
};

enum orientation {
	UP = 0, RIGHT, DOWN, LEFT
};

typedef struct text_line {
	int	startX;
	int	startY;
	int	width;
	int spacing;
	int invert;
	enum font_name font;
	enum layout layout;
}TEXT_LINE;

typedef struct rectangle {
	int startX;
	int startY;
	int height;
	int width;
	int fill;
}RECTANGLE;

typedef struct bar {
	int startX;
	int startY;
	int height;
	int width;
	int minval;
	int maxval;
	enum orientation orientation;
}BAR;

typedef struct point {
	int	X;
	int Y;
	int isBlack;
}POINT;

typedef struct icon {
	int	startX;
	int startY;
	int invert;
	char	filepath[];
}ICON;

void clear_frame(void *pdisp);
void clear_region(void *pdisp, int startX, int startY, int endX, int endY, int fill);
void draw_point(void *pdisp, int x, int y);
void draw_text(void *pdisp, struct text_line* line, const char text[]);
void draw_rect(void *pdisp, struct rectangle* rect);
void draw_bar(void *pdisp, struct bar* bar, const int val);

#endif /* SRC_DISPLAY_GRAPHIC_LCD_GRAPHIC_LCD_H_ */
