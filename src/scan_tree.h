/*
 * scan_tree.h
 *
 *  Created on: Jun 2, 2018
 *      Author: kidsr
 */

#ifndef SCAN_TREE_H_
#define SCAN_TREE_H_

#include <time.h>

//#include "scan_tree.c"
#define MAX_TAG_LENGTH	32
#define MAX_FILENAME_LENGTH	MAX_TAG_LENGTH + 4
#define SP_FILE_EXT	".sp"
#define SYS_FILE_EXT	".scn"
#define CHAN_FILE_EXT	".chan"

enum system_type {
	ANALOG = 0,
	P25,
	DMR
};

const char *getSysType(enum system_type type);

enum modulation {
	FM = 0,
	NFM,
	AM,
	USB,
	LSB,
	RAW
};

const char *getMode(enum modulation mode);

typedef struct channel {
	char	*tag;
	uint32_t	freq;
	enum modulation	mode;
	int		delay_enabled;
	time_t	sigloss_start_time;
	//void	(*mode_demod)(struct demod_state*);
} channel_t;

typedef struct entry {
	char	*tag;
} entry_t;

typedef struct system {
	char	*tag;
	enum system_type	type;
	int	channel_count;
	channel_t	**channels;
	void	(*scan)(struct system*);
} system_t;

struct scan_profile {
	char	*name;
	struct system	*systems;
	int		num_systems;
};

int scan_profile_load(struct scan_profile *sp, char dir[]);
int scan_profile_save(struct scan_profile *sp, char dir[]);

#endif /* SCAN_TREE_H_ */
