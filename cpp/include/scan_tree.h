/*
 * scan_tree.h
 *
 *  Created on: Jun 2, 2018
 *      Author: kidsr
 */

#ifndef SCAN_TREE_H_
#define SCAN_TREE_H_

#include <stdint.h>
#include <time.h>

//#include "debug.h"

//#include "scan_tree.c"
#define SCAN_TREE_TAG	"Scan Tree"

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

enum modulation {
	FM = 0,
	NFM,
	AM,
	USB,
	LSB,
	RAW
};

enum squelch_mode {
	CSQ = 0,
	PL,
	DCS
};


typedef struct {
	char	*tag;
	uint32_t	freq;
	enum modulation	mode;
	enum squelch_mode squelch;
	union {
		int pl_tone;
		int dcs_tone;
	};
	int		delay_enabled;
	int		lockout;
	time_t	sigloss_start_time;
	//void	(*mode_demod)(struct demod_state*);
} Scan_Channel_t;

enum entry_type {
	ANALOG_CHANNEL = 0, TALKGROUP
};

typedef struct {
	//char	*tag;
	//int		lockout;
	enum entry_type	type;
	void	*entry_data;
} Scan_Entry_t;

typedef struct {
	char	*tag;
	enum system_type	type;
	int	channel_count;
	int		lockout;
	Scan_Channel_t	**channels;
} Scan_System_t;

typedef struct {
	char	*dir;
	char	*name;
	int		num_systems;
	Scan_System_t	*systems;
} Scan_Profile_t;

int scan_profile_load(void *psp);
int scan_profile_save(void *psp);

#endif /* SCAN_TREE_H_ */
