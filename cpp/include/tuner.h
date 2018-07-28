/*
 * tuner.h
 *
 *  Created on: Jun 3, 2018
 *      Author: kidsr
 */

#ifndef SRC_TUNER_H_
#define SRC_TUNER_H_

#include <stdbool.h>

#define TUNER_DEMOD_FM			0x01
#define TUNER_DEMOD_AM			0x02
#define TUNER_DEMOD_FM_PL		0x04
#define TUNER_DEMOD_FM_DC		0x08
#define TUNER_DEMOD_P25P1		0x10
#define TUNER_DEMOD_P25P2		0x20
#define TUNER_DEMOD_DMR			0x40
#define TUNER_DEMOD_NXDN		0x80

typedef enum {
	TUNER_SUCCESS,
	TUNER_FAIL,
	TUNER_READY,
	TUNER_BUSY,
	TUNER_FREQUENCY_NOT_SUPPORTED,
	TUNER_DEMOD_NOT_SUPPORTED
} Tuner_Status_t;

typedef enum {
	TUNER_FM,
	TUNER_AM,
	TUNER_PL,
	TUNER_DC,
	TUNER_P25P1,
	TUNER_P25P2,
	TUNER_DMR,
	TUNER_NXDN,
} Tuner_Demodulator_t;

typedef struct {
	uint32_t			frequency;
	Tuner_Demodulator_t	demod;
	uint16_t			demodParamA;
	uint16_t			demodParamB;
	uint16_t			demodParamC;
} Tuner_Channel_t;

typedef struct {
	const uint16_t	supportedDemods;
	Tuner_Status_t	(*init)(void);
	Tuner_Status_t	(*tuneTo)(Tuner_Channel_t*);
	bool			(*hasSignal)(void);
} Tuner_t;

#ifdef __cplusplus
extern "C" {
#endif

extern Tuner_t MockTuner;
extern Tuner_t RTL_SDR;
extern Tuner_t BC560XLT;

const uint16_t const tuner_demod_lookup_table[] = {
		[TUNER_FM]		= TUNER_DEMOD_FM,
		[TUNER_AM]		= TUNER_DEMOD_AM,
		[TUNER_PL]		= TUNER_DEMOD_FM_PL | TUNER_DEMOD_FM,
		[TUNER_DC]		= TUNER_DEMOD_FM_DC | TUNER_DEMOD_FM,
		[TUNER_P25P1]	= TUNER_DEMOD_P25P2 | TUNER_DEMOD_P25P1,
		[TUNER_DMR]		= TUNER_DEMOD_DMR,
		[TUNER_NXDN]	= TUNER_DEMOD_NXDN,
};

#ifdef __cplusplus
}
#endif

#endif /* SRC_TUNER_H_ */
