/*
 * test_RtlFm.cpp
 *
 *  Created on: Feb 27, 2019
 *      Author: ezra
 */

#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <unistd.h>


#include "rtl_fm.h"

using namespace std;

int main(int argc, char** argv){
	cerr << "Starting rtl_fm test\n";

	timespec time1, time2;

	timespec_get(&time1, TIME_UTC);
	if(rtl_fm_init((void*)0, (size_t)0, 12000))
		return 1;
	timespec_get(&time2, TIME_UTC);
	fprintf(stderr, "%li\n", (time2.tv_nsec - time1.tv_nsec)/1000);

	//usleep(3000000);
	timespec_get(&time1, TIME_UTC);
	while(rtl_fm_setfreq(162550000));
	timespec_get(&time2, TIME_UTC);
	fprintf(stderr, "%li\n", (time2.tv_nsec - time1.tv_nsec)/1000);

	usleep(3000000);
	timespec_get(&time1, TIME_UTC);
	while(rtl_fm_setfreq(472087500));
	timespec_get(&time2, TIME_UTC);
	fprintf(stderr, "%li\n", (time2.tv_nsec - time1.tv_nsec)/1000);

	//usleep(10000);
	timespec_get(&time1, TIME_UTC);
	while(rtl_fm_setfreq(453150000));
	timespec_get(&time2, TIME_UTC);
	fprintf(stderr, "%li\n", (time2.tv_nsec - time1.tv_nsec)/1000);

	int freq = 0;
	while(1){
		cin >> freq;
		if(freq == 0)
			break;
		else
			while(rtl_fm_setfreq(freq));
	}

	rtl_fm_deinit();

	return 0;
}


