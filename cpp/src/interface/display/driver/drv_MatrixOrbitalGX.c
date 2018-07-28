/* $Id: drv_MatrixOrbitalGX.c 975 2009-02-27 18:50:20Z abbas $
 * $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/drv_MatrixOrbitalGX.c $
 *
 * driver for Matrix Orbital GX Series Graphic(240x64) displays from matrixorbital.com
 *
 * Copyright (C) 2009 Abbas Kosan <abbaskosan@gmail.com>
 * Copyright (C) 2005, 2006, 2007 The LCD4Linux Team 
 * <lcd4linux-devel@users.sourceforge.net>
 *
 * This file is part of LCD4Linux.
 *
 * LCD4Linux is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LCD4Linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*
 * MODIFIED for use in PiScan
 */

/* 
 *
 * exported fuctions:
 *
 * struct DRIVER drv_MatrixOrbitalGX
 *
 */

//#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
//#include <termios.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <sys/time.h>

//#include <libusb-1.0/libusb.h>
#include "debug.h"
#include "libusb-compat-0.1/libusb/usb.h"
#include "interface/graphic_lcd/display.h"
//#include "usb.h"
/*
#include "debug.h"
#include "cfg.h"
#include "qprintf.h"
#include "udelay.h"
#include "plugin.h"
#include "widget.h"
#include "widget_text.h"
#include "widget_icon.h"
#include "widget_bar.h"
#include "drv.h"

#include "drv_generic_graphic.h"
*/
#define MOGX_TAG	"Matrix Orbital driver"

#define MatrixOrbitalGX_VENDOR  	0x1b3d
#define MatrixOrbitalGX_DEVICE_1  	0x000a
#define MatrixOrbitalGX_DEVICE_2  	0x000b
#define MatrixOrbitalGX_DEVICE_3  	0x000c

/********Matrix Orbital GX Series*********/
#define INTERFACE_ 			0
#define BULK_OUT_ENDPOINT 		0x05
#define BULK_IN_ENDPOINT 		0x82

#define SCREEN_H			64
#define SCREEN_W			240
#define SCREEN_SIZE			(SCREEN_H * SCREEN_W)
/*****************************************/

/*#if 1
#define DEBUG(x) debug("%s(): %s", __FUNCTION__, x);
#else
#define DEBUG(x)
#endif*/


//static char Name[] = "MatrixOrbitalGX";
static uint8_t *MOGX_framebuffer;

/* used to display white text on blue background or inverse */
static unsigned char invert = 0x00;
static unsigned char backlight_RGB = 0;

static usb_dev_handle *lcd_dev;

/*
int mygetch(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
*/

/****************************************/
/***  hardware dependant functions    ***/
/****************************************/

static int drv_MOGX_open(void)
{
    struct usb_bus *busses, *bus;
    struct usb_device *dev;
    char driver[1024];
    char product[1024];
    char manufacturer[1024];
    char serialnumber[1024];
    int ret;

    lcd_dev = NULL;

    debug(MOGX_TAG, "Scanning for Matrix Orbital GX Series LCD...");

    usb_set_debug(0);

    usb_init();
    usb_find_busses();
    usb_find_devices();
    busses = usb_get_busses();

    for (bus = busses; bus; bus = bus->next) {
	for (dev = bus->devices; dev; dev = dev->next) {
	    if ((dev->descriptor.idVendor == MatrixOrbitalGX_VENDOR) &&
		((dev->descriptor.idProduct == MatrixOrbitalGX_DEVICE_1) ||
		 (dev->descriptor.idProduct == MatrixOrbitalGX_DEVICE_2) ||
		 (dev->descriptor.idProduct == MatrixOrbitalGX_DEVICE_3))) {

		/* At the moment, I have information for only this LCD */
		if (dev->descriptor.idProduct == MatrixOrbitalGX_DEVICE_2)
		    backlight_RGB = 0;

		debug(MOGX_TAG, "Found Matrix Orbital GX Series LCD on bus %s device %s"/*, bus->dirname, dev->filename*/);

		lcd_dev = usb_open(dev);

		ret = usb_get_driver_np(lcd_dev, 0, driver, sizeof(driver));

		if (ret == 0) {
		    debug(MOGX_TAG, "Interface 0 already claimed by '%s'"/*, driver*/);
		    debug(MOGX_TAG, "Attempting to detach driver...");
		    if (usb_detach_kernel_driver_np(lcd_dev, 0) < 0) {
			error(MOGX_TAG, "%s: usb_detach_kernel_driver_np() failed!");
			return -1;
		    }
		}

		usb_set_configuration(lcd_dev, 1);
		usleep(100);

		if (usb_claim_interface(lcd_dev, 0) < 0) {
		    error(MOGX_TAG, "usb_claim_interface() failed!");
		    return -1;
		}

		usb_set_altinterface(lcd_dev, 0);

		usb_get_string_simple(lcd_dev, dev->descriptor.iProduct, product, sizeof(product));
		usb_get_string_simple(lcd_dev, dev->descriptor.iManufacturer, manufacturer, sizeof(manufacturer));
		usb_get_string_simple(lcd_dev, dev->descriptor.iSerialNumber, serialnumber, sizeof(serialnumber));

		//info("%s: Manufacturer='%s' Product='%s' SerialNumber='%s'", Name, manufacturer, product, serialnumber);

		return 0;
	    }
	}
    }
    error(MOGX_TAG, "Could not find a Matrix Orbital GX Series LCD");
    return -1;
}

static int drv_MOGX_send(const unsigned char *data, const unsigned int size)
{
    int __attribute__ ((unused)) ret;

    //unsigned char rcv_buffer[64] = "";

    ret = usb_bulk_write(lcd_dev, BULK_OUT_ENDPOINT, (char *) data, size, 1000);

    //info("%s written %d bytes\n", __FUNCTION__, ret);

    //ret = usb_bulk_read(lcd_dev, BULK_IN_ENDPOINT, (char *) rcv_buffer,64,1000);

    //printf("\nReply : ");
    //for (i=0;i<ret;i++)
    //printf("%3x",rcv_buffer[i]);
    return ret;
}

static int drv_MOGX_close(void)
{
    /* close whatever port you've opened */
    usb_release_interface(lcd_dev, 0);
    usb_close(lcd_dev);

    return 0;
}

/* Send framebuffer to lcd */
static int drv_MOGX_update_lcd(void)
{
    unsigned char cmd[3852] = { 0x38, 0x46, 0x42, 0x46, 0x50, 0x00, 0x00, 0x07, 0x80, 0xff, 0xff };
    /*
       index    : index of pixel_byte in cmd[]
       bit              : pixel of each pixel_byte
       x                : index of pixel in framebuffer
       pixel_byte       : each 8 pixel in framebuffer = 1 byte
     */
    int index, bit, x = 0;
    unsigned int cmd_length = 0;
    unsigned char pixel_byte;

    //info("In %s\n", __FUNCTION__);

    for (index = 0; index < (SCREEN_SIZE / 8); index++) {
	pixel_byte = 0x00;
	for (bit = 7; bit >= 0; bit--) {
	    if (MOGX_framebuffer[x] ^ invert)
		pixel_byte |= (1 << bit);
	    else
		pixel_byte &= ~(1 << bit);
	    x++;
	}

	if (pixel_byte == 0xc0) {
	    cmd[11 + cmd_length] = 0xdb;
	    cmd_length++;
	    cmd[11 + cmd_length] = 0xdc;
	    cmd_length++;
	} else if (pixel_byte == 0xdb) {
	    cmd[11 + cmd_length] = 0xdb;
	    cmd_length++;
	    cmd[11 + cmd_length] = 0xdd;
	    cmd_length++;
	} else {
	    cmd[11 + cmd_length] = pixel_byte;
	    cmd_length++;
	}
    }
    /* finish command */
    cmd[11 + cmd_length] = 0xc0;

    //info("In %s - %s \n", __FUNCTION__, cmd_img);
    /* send command which includes framebuffer */
    return drv_MOGX_send(cmd, cmd_length + 12);
}

/*static void drv_MOGX_blit(const int row, const int col, const int height, const int width)
{
    int r, c;

    for (r = row; r < row + height; r++) {
	for (c = col; c < col + width; c++)
	    MOGX_framebuffer[r * SCREEN_W + c] = drv_generic_graphic_black(r, c);
    }
    drv_MOGX_update_lcd();
}*/

void drv_MOGX_clear(void)
{
    //info("In %s\n", __FUNCTION__);
    memset(MOGX_framebuffer, 0x00, SCREEN_SIZE);
    /* clear the screen */
    drv_MOGX_update_lcd();
}

/*	TODO :	I am not sure for contrast function (command) 
		Don't try to adjust contrast until contrast function is checked and fixed
*/
/*
static int drv_MOGX_contrast(int contrast)
{
    unsigned char cmd[11] = {0x18,0x4c,0x43,0x53,0x43,0x00,0x00,0x00,0x01,0x00,0xc0};

    // adjust limits according to the display
    if (contrast < 0)
	contrast = 0;
    if (contrast > 255)
	contrast = 255;

    // send contrast command
    cmd[9] = contrast;
    drv_MOGX_send(cmd, 11);

    return contrast;
}
*/

/* backlight function used in plugin */
/*static int drv_MOGX_backlight(int backlight)
{
    unsigned char cmd[13] = { 0x18, 0x4c, 0x43, 0x53, 0x48, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xc0 };

    if (backlight < 0)
	backlight = 0;
    if (backlight >= 255)
	backlight = 255;

    cmd[10] = backlight;
    drv_MOGX_send(cmd, 13);

    return backlight;
}*/

/* backlightRGB function used in plugin */
static int drv_MOGX_backlightRGB(int backlight_R, int backlight_G, int backlight_B)
{
    /*
       TODO :   Function should be tested for three color LCD
     */
    //unsigned char cmd1[11] = {0x18,0x4c,0x43,0x53,0x42,0x00,0x00,0x00,0x01,0xff,0xc0};
    unsigned char cmd2[13] = { 0x18, 0x4c, 0x43, 0x53, 0x48, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xc0 };

    if (backlight_R < 0)
	backlight_R = 0;
    if (backlight_R >= 255)
	backlight_R = 255;

    if (backlight_G < 0)
	backlight_G = 0;
    if (backlight_G >= 255)
	backlight_G = 255;

    if (backlight_B < 0)
	backlight_B = 0;
    if (backlight_B >= 255)
	backlight_B = 255;

    //cmd1[9] = backlight;
    cmd2[9] = backlight_R;
    cmd2[10] = backlight_G;
    cmd2[11] = backlight_B;
    //drv_MOGX_send(cmd1, 11);
    drv_MOGX_send(cmd2, 13);

    return backlight_R + backlight_G + backlight_B;
}

/* start graphic display */
static int drv_MOGX_start(DISPLAY *pdisp)
{
    //char *s;
    //int value1, value2, value3;

    /* read display size from config */
    /*s = cfg_get(section, "Size", NULL);
    if (s == NULL || *s == '\0') {
	error("%s: no '%s.Size' entry from %s", Name, section, cfg_source());
	return -1;
    }*/

    /*DROWS = -1;
    DCOLS = -1;
    if (sscanf(s, "%dx%d", &DCOLS, &DROWS) != 2 || DCOLS < 1 || DROWS < 1) {
	error("%s: bad %s.Size '%s' from %s", Name, section, s, cfg_source());
	return -1;
    }*/

    /*s = cfg_get(section, "Font", "6x8");
    if (s == NULL || *s == '\0') {
	error("%s: no '%s.Font' entry from %s", Name, section, cfg_source());
	return -1;
    }*/

    /*XRES = -1;
    YRES = -1;
    if (sscanf(s, "%dx%d", &XRES, &YRES) != 2 || XRES < 1 || YRES < 1) {
	error("%s: bad %s.Font '%s' from %s", Name, section, s, cfg_source());
	return -1;
    }*/

    /* Fixme: provider other fonts someday... */
    /*if (XRES != 6 && YRES != 8) {
	error("%s: bad Font '%s' from %s (only 6x8 at the moment)", Name, s, cfg_source());
	return -1;
    }*/

    //if (cfg_number(section, "Invert", 0, 0, 1, &value1) > 0)
	if (pdisp->invert > 0) {
	    info(MOGX_TAG, "Display is inverted");
	    invert = 0x01;
	}

    /* open communication with the display */
    if (drv_MOGX_open() < 0) {
	return -1;
    }

    /* Init framebuffer buffer */
    MOGX_framebuffer = (uint8_t *) malloc(SCREEN_SIZE * sizeof(uint8_t));
    if (!MOGX_framebuffer) {
	error(MOGX_TAG, "framebuffer could not be allocated: malloc() failed");
	return -1;
    }

    memset(MOGX_framebuffer, 0x00, SCREEN_SIZE);
    //info("%s framebuffer zeroed", __FUNCTION__);

/*	TODO :	I am not sure for contrast function (command) 
		Don't try to adjust contrast until contrast function is checked and fixed
*/
/*
    if (cfg_number(section, "Contrast", 0, 0, 255, &value1) > 0) {
	info("%s: Setting contrast to %d", Name, value1);
	drv_MOGX_contrast(value1);
    }
*/
    /* if lcd has three color backlight call the backlightRGB function */
    /*if (backlight_RGB) {
	if ((cfg_number(section, "Backlight_R", 0, 0, 255, &value1) > 0) &&
	    (cfg_number(section, "Backlight_G", 0, 0, 255, &value2) > 0) &&
	    (cfg_number(section, "Backlight_B", 0, 0, 255, &value3) > 0)) {
	    info("%s: Setting backlight to %d,%d,%d (RGB)", Name, value1, value2, value3);
	    drv_MOGX_backlightRGB(value1, value2, value3);
	}
    } else {
	if ((cfg_number(section, "Backlight", 0, 0, 255, &value1) > 0)) {
	    info("%s: Setting backlight to %d", Name, value1);
	    drv_MOGX_backlight(value1);
	}
    }*/
    drv_MOGX_backlightRGB(pdisp->backlight[0], pdisp->backlight[1], pdisp->backlight[2]);

    //info("In %s\n", __FUNCTION__);

    return 0;
}


/****************************************/
/***            plugins               ***/
/****************************************/
/*	TODO :	I am not sure for contrast function (command) 
		Don't try to adjust contrast until contrast function is checked and fixed
*/
/*
static void plugin_contrast(RESULT * result, RESULT * arg1)
{
    double contrast;

    contrast = drv_MOGX_contrast(R2N(arg1));
    SetResult(&result, R_NUMBER, &contrast);
}
*/
/*
static void plugin_backlight(RESULT * result, RESULT * arg1)
{
    double backlight;

    backlight = drv_MOGX_backlight(R2N(arg1));
    SetResult(&result, R_NUMBER, &backlight);
}

static void plugin_backlightRGB(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3)
{
    double backlight;

    backlight = drv_MOGX_backlightRGB(R2N(arg1), R2N(arg2), R2N(arg3));
    SetResult(&result, R_NUMBER, &backlight);
}
*/
/****************************************/
/***        widget callbacks          ***/
/****************************************/


/* using drv_generic_text_draw(W) */
/* using drv_generic_text_icon_draw(W) */
/* using drv_generic_text_bar_draw(W) */
/* using drv_generic_gpio_draw(W) */


/****************************************/
/***        exported functions        ***/
/****************************************/


/* list models */
int drv_MOGX_list(void)
{
    printf("Matrix Orbital GX Series driver");
    return 0;
}

/* initialize driver & display */
int drv_MOGX_init(void *ptr)
{
	DISPLAY *pdisp = ptr;
    int ret;

    //info("%s: %s", Name, "$Rev: 2$");
    //info("Matrix Orbital GX Series LCD initialization\n");

    /* real worker functions */
//    drv_generic_graphic_real_blit = drv_MOGX_blit;

    /* start display */
    if ((ret = drv_MOGX_start(pdisp)) != 0)
	return ret;

    /* initialize generic graphic driver */
/*    if ((ret = drv_generic_graphic_init(section, Name)) != 0)
	return ret;
*/
/*    if (!quiet) {
	char buffer[40];
	qprintf(buffer, sizeof(buffer), "%s %dx%d", Name, DCOLS, DROWS);
	if (drv_generic_graphic_greet(buffer, "http://www.matrixorbital.com")) {
	    sleep(3);
	    drv_generic_graphic_clear();
	}
    }
*/
    /* register plugins */
    /*  TODO :  I am not sure for contrast function (command) 
       Don't try to adjust contrast until contrast function is checked and fixed
     */
    //AddFunction("LCD::contrast", 1, plugin_contrast);
/*    if (backlight_RGB)
	AddFunction("LCD::backlightRGB", 3, plugin_backlightRGB);
    else
	AddFunction("LCD::backlight", 1, plugin_backlight);
*/
    //info("In %s\n", __FUNCTION__);

    memset(MOGX_framebuffer, 0x00, SCREEN_SIZE);
    //DEBUG("zeroed");

    pdisp->output_buffer = MOGX_framebuffer;

    return 0;
}



/* close driver & display */
int drv_MOGX_quit(void)
{
    info(MOGX_TAG, "shutting down.");

    /* clear display */
    drv_MOGX_clear();

    //drv_generic_graphic_quit();

    //debug("closing connection");
    drv_MOGX_close();

    if (MOGX_framebuffer) {
	free(MOGX_framebuffer);
    }

    return (0);
}

/* use this one for a graphic display */
/*DRIVER drv_MatrixOrbitalGX = {
    .name = Name,
    .list = drv_MOGX_list,
    .init = drv_MOGX_init,
    .quit = drv_MOGX_quit,
};
*/

DISPLAY disp_MatrixOrbitalGX = {
		.height = SCREEN_H,
		.width = SCREEN_W,
		.init = &drv_MOGX_init,
		.update = &drv_MOGX_update_lcd,
		.quit = &drv_MOGX_quit
};
