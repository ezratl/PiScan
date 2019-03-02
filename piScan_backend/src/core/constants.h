/*
 * constants.h
 *
 *  Created on: Feb 24, 2019
 *      Author: ezra
 */

#ifndef CORE_CONSTANTS_H_
#define CORE_CONSTANTS_H_

#ifndef NDEBUG
#define LOG_PATH	"log.txt"
#define CONFIG_PATH	"piscan.config"
#else
#define LOG_PATH	"~/piscan.log"
#define CONFIG_PATH	"~/piscan.config"
#endif

enum ConnectionLevel {
	RECEIVE_ONLY, VIEWER, FULL_CONTROL,
};

enum RequestType {
	NOTIFY_DISCONNECTED = 0,
	SYSTEM_FUNCTION,
	SCANNER_FUNCTION,
	DATABASE_RETRIEVE,
	DATABASE_MODIFY,
	CONFIG_RETRIEVE,
	CONFIG_MODIFY,
};

enum {
	SYSTEM_STOP,
};


#endif /* CORE_CONSTANTS_H_ */
