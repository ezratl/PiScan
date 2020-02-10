/*
 * constants.h
 *
 *  Created on: Feb 24, 2019
 *      Author: ezra
 */

#ifndef CORE_CONSTANTS_H_
#define CORE_CONSTANTS_H_

#define AUTO_GAIN	-10

namespace piscan {

#define TUNER_RETUNE_TIME	225000
#define DEMOD_BUFFER_TIME	10000

enum ConnectionLevel {
	RECEIVE_ONLY = 0, VIEWER, FULL_CONTROL,
};

enum RequestType {
	NOTIFY_DISCONNECTED = 0,
	SYSTEM_FUNCTION,
	SCANNER_FUNCTION,
	DATABASE_RETRIEVE,
	DATABASE_MODIFY,
	CONFIG_RETRIEVE,
	CONFIG_MODIFY,
	DEMOD_CONFIGURE,
	GET_CONTEXT,
};

/* system functions */
enum {
	SYSTEM_STOP,
};

/* scanner functions */
enum {
	SCANNER_GET_CONTEXT,
	SCANNER_STATE_SCAN,
	SCANNER_STATE_HOLD,
	SCANNER_STATE_MANUAL,
};

/* demod functions */
enum {
	DEMOD_SET_SQUELCH,
	DEMOD_SET_GAIN,
};

/* context requests */
enum {
	SCANNER_CONTEXT,
	DEMOD_CONTEXT,
};


void terminate();

}
#endif /* CORE_CONSTANTS_H_ */
