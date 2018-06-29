/*
 * test_scan_tree.c
 *
 *  Created on: Jun 2, 2018
 *      Author: Ezra Taimuty-Loomis
 */

/*
 * Program to test scan tree implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../src/scan_tree.h"


struct scan_profile generated_profile;
struct scan_profile loaded_profile;

char test_directory[] = "./profiles/";

void init_test_sys_1(struct system *sys){
	fprintf(stdout, "Initializing system 1\n");
	sys->tag = "Analog system 1";
	sys->type = ANALOG;
	sys->channel_count = 0;
	sys->channels = malloc(10 * sizeof(struct channel*));
	uint32_t freq = 470000000;
	for(int i = 0; i < 10; i++){
		sys->channels[i] = malloc(sizeof(struct channel));
		char num[3];
		sys->channels[i]->tag = malloc(16 * sizeof(char) + 1);
		memset(sys->channels[i]->tag, 0, 16 * sizeof(char) + 1);
		itoa(i, num, 10);
		strcat(sys->channels[i]->tag, "Channel ");
		strcat(sys->channels[i]->tag, num);
		//sys->channels[i]->tag = out;
		sys->channels[i]->freq = freq;
		sys->channels[i]->delay_enabled = i % 2;
		sys->channels[i]->mode = i % 3;
		sys->channel_count++;
		freq += 12500;
	}
}

void init_test_prf(struct scan_profile *sp){
	fprintf(stdout, "Initializing test profile\n");
	sp->name = "Test profile 1";
	struct system sys1;
	init_test_sys_1(&sys1);
	fprintf(stdout, "System 1 initialized\n");


	sp->num_systems = 1;
	sp->systems = malloc(1 * sizeof(struct system));
	sp->systems[0] = sys1;

}

void print_analog_sys(struct system *sys){
	fprintf(stdout, "System: %s\n\tType: %s\n\tChannels:\n", sys->tag, getSysType(sys->type));
	for(int i = 0; i < sys->channel_count; i++){
		fprintf(stdout, "\t\t%i: %s\n", i, sys->channels[i]->tag);
		fprintf(stdout, "\t\t\t%lfMHz\n\t\t\tMode: %s\n\t\t\tDelay: %i\n",
				(sys->channels[i]->freq/1000000.0),
				getMode(sys->channels[i]->mode),
				sys->channels[i]->delay_enabled);
	}
}

int main(void){


	init_test_prf(&generated_profile);
	fprintf(stdout, "Profile initialized\nPrinting systems\n");

	for(int i = 0; i < generated_profile.num_systems; i++){
		switch(generated_profile.systems[i].type){
		case P25:
			break;
		case DMR:
			break;
		case ANALOG:
		default:
			print_analog_sys(&generated_profile.systems[i]);
		}
	}

	fprintf(stdout, "Saving file\n");
	if(scan_profile_save(&generated_profile))
		fprintf(stdout, "Saved successfully\n");

	fprintf(stdout, "Loading scan file\n");
	scan_profile_load(&loaded_profile);

	system("pause");
	return 0;
}

