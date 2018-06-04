/*
 * scan_tree.c
 *
 *  Created on: May 31, 2018
 *      Author: Ezra Taimuty-Loomis
 */

/*
 * Definitions for the scan tree data structure
 */

/* Includes */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <errno.h>

//#include "../../rtl-sdr/src/rtl_fm.c"
#include "scan_tree.h"

/* Definitions */


const char *getSysType(enum system_type type){
	switch(type){
	case ANALOG: return "Analog";
	case P25: return "P25";
	case DMR: return "DMR";
	default: return "";
	}
}



const char *getMode(enum modulation mode){
	switch(mode){
	case FM: return "FM";
	case NFM: return "NFM";
	case AM: return "AM";
	case USB: return "USB";
	case LSB: return "LSB";
	case RAW: return "RAW";
	default: return "";
	}
}



//typedef struct channel_fm {
//	char 	mode[] = "FM";
//	struct channel	chan;
//	void	(*mode_demod)(struct demod_state*) = &fm_demod;
//	//chan.mode_demod = &fm_demod;
//};
//
//typedef struct channel_nfm {
//	char	mode[] = "FM";
//	struct channel chan;
//	void	(*mode_demod)(struct demod_state*) = &fm_demod;
//};
//
//typedef struct channel_am {
//	char	mode[] = "AM";
//	struct channel chan;
//	void	(*mode_demod)(struct demod_state*) = &am_demod;
//};



/* Methods */
int system_save(struct system *sys, char prf_dir[]){
	char *sys_path = malloc(sizeof(*prf_dir) + MAX_TAG_LENGTH + 2);
	memset(sys_path, 0, sizeof(*prf_dir) + MAX_TAG_LENGTH + 2);
	strcat(sys_path, prf_dir);
	strcat(sys_path, "/");
	strcat(sys_path, sys->tag);

	mkdir(sys_path);

	char *sys_filename = malloc(sizeof(*sys_path) + sizeof(SYS_FILE_EXT) + 1);
	memset(sys_filename, 0, sizeof(*sys_path) + sizeof(SYS_FILE_EXT) + 1);
	strcat(sys_filename, sys_path);
	strcat(sys_filename, SYS_FILE_EXT);

	fprintf(stdout, "\tOpening \"%s\"", sys_filename);
	FILE *outfile = fopen(sys_filename, "w");
	if (outfile == NULL) {
		fprintf(stderr, "File open error\n");
		return 0;
	}

	fwrite(sys, sizeof(struct system), 1, outfile);

	fclose(outfile);
	free(sys_path);
	free(sys_filename);
	return 0;
}

int scan_profile_load(struct scan_profile *sp, char dir[]){
	return 0;
}

int scan_profile_save(struct scan_profile *sp, char dir[]) {
	int err = mkdir(dir);
	if (err != 0) {
		fprintf(stderr, "Could not make directory \"%s\": code %i\n", dir, err);
		if (err == EEXIST)
			fprintf(stderr, "\tDirectory already exists\n");
		//else return 0;
	}

	char *sp_path = malloc(sizeof(*dir) + MAX_TAG_LENGTH + 1);
	memset(sp_path, 0, sizeof(*dir) + MAX_TAG_LENGTH + 1);
	strcat(sp_path, dir);
	strcat(sp_path, sp->name);

	err = mkdir(sp_path);
	if (err != 0) {
		fprintf(stderr, "Could not make directory \"%s\": code %i\n", sp_path, err);
		if (err == EEXIST)
			fprintf(stderr, "\tDirectory already exists\n");
		//else return 0;
	}

	char *sp_filename = malloc(sizeof(*sp_path) + sizeof(SP_FILE_EXT) + 1);
	memset(sp_filename, 0, sizeof(*sp_path) + sizeof(SP_FILE_EXT) + 1);
	strcat(sp_filename, sp_path);

	strcat(sp_filename, SP_FILE_EXT);
	fprintf(stdout, "Opening file \"%s\"\n", sp_filename);
	FILE *outfile = fopen(sp_filename, "w");
	if (outfile == NULL) {
		fprintf(stderr, "File open error\n");
		return 0;
	}

	err = fwrite(sp, sizeof(struct scan_profile), 1, outfile);

	for(int i = 0; i < sp->num_systems; i++)
		system_save(&sp->systems[i], sp_path);

	fclose(outfile);
	free(sp_path);
	free(sp_filename);
	return err;
}

