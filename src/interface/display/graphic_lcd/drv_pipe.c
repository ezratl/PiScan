/*
 * drv_pipe.c
 *
 *  Created on: Jun 9, 2018
 *      Author: kidsr
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
//#include <io.h>
#include <stdlib.h>
#include <errno.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "interface/graphic_lcd/display.h"

static DISPLAY *disp;
static const char fifo_name[] = "./graphic_lcd.fifo";
static FILE *outfile;

static int update(){
	fwrite(disp->output_buffer, sizeof(char), disp->height * disp->width, outfile);
	return 1;
}

int drv_pipe_list(){
	return 0;
}

int drv_pipe_init(void *pdisp){
	disp = (DISPLAY*) pdisp;
	disp->output_buffer = malloc(disp->height * disp->width * sizeof(char));
	memset(disp->output_buffer, 0, disp->height * disp->width * sizeof(char));
	if(!disp->output_buffer)
		return 0;

	//mkfifo(&fifo_name);
	outfile = fopen(fifo_name, "w");
	if(outfile == NULL)
		return 0;

	disp->update = &update;
	return 1;
}

int drv_pipe_quit(){
	free(disp->output_buffer);
	fclose(outfile);
	return 1;
}

DISPLAY pipe_fifo = { 0, 0, {255, 255, 255}, &drv_pipe_init, &drv_pipe_quit, &update };
