/*
 * constants.h
 *
 *  Created on: Feb 24, 2019
 *      Author: ezra
 */

#ifndef CORE_CONSTANTS_H_
#define CORE_CONSTANTS_H_

#ifndef NDEBUG
#define LOG_VERBOSITY	7
#define LOG_PATH		"log.txt"
#define CONFIG_PATH		"piscan.config"
#else
#define LOG_VERBOSITY	0
#define LOG_PATH		"~/piscan.log"
#define CONFIG_PATH		"~/piscan.config"
#endif

#define DATABASE_PATH	"../data"

#define AUTO_GAIN	-10

#define DEFAULT_TCP_PORT	1234
#define MAX_TCP_CONNECTIONS	5

namespace piscan {

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
}
#endif /* CORE_CONSTANTS_H_ */
