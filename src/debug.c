/*
 * debug.c
 *
 *  Created on: Jun 11, 2018
 *      Author: kidsr
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "debug.h"

#define LOGFILE_PATH	"./PiScan.log"
#define DEBUG_TAG	"Debug"

FILE *logfile;
int debug_messages_enabled = 1;
struct tm *cur_time;

static void print_time(){
	time_t ctime = time(NULL);
	cur_time = localtime(&ctime);
	printf("[%i:%i:%i]\t", cur_time->tm_hour, cur_time->tm_min, cur_time->tm_sec);
	if(logfile)
		fprintf(logfile, "[%i/%i/%i][%i:%i:%i]\t", cur_time->tm_mon + 1, cur_time->tm_mday, cur_time->tm_year,
			cur_time->tm_hour, cur_time->tm_min, cur_time->tm_sec);
}

int debug_init(int debug_enable){
	debug_messages_enabled = debug_enable;

	logfile = fopen(LOGFILE_PATH, "a");
	if(!logfile){
		error(DEBUG_TAG, "Could not open log file");
		return 0;
	}

	return 1;
}

int debug_quit(){
	fclose(logfile);
	return 1;
}

void general_info(char message[]){
	print_time();
	printf("%s\n", message);
	if (logfile)
		fprintf(logfile, "%s\n", message);
}

void info(char tag[], char message[]){
	print_time();
	printf("%s: %s\n", tag, message);
	if(logfile)
		fprintf(logfile, "%s: %s\n", tag, message);
}

void debug(char tag[], char message[]){
	if(debug_messages_enabled)
		info(tag, message);
}

void error(char tag[], char message[]){
	print_time();
	printf("ERROR:\t%s: %s\n", tag, message);
	if (logfile)
		fprintf(logfile, "ERROR:\t%s: %s\n", tag, message);
}

void error_report(char tag[], char source[], char message[]){
	error(tag, message);
	if (logfile)
		fprintf(logfile, "\tIn %s\n", source);
}
