/*
 * RadioSysten.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef RADIOSYSTEM_H_
#define RADIOSYSTEM_H_

typedef enum {
	SYSTEM_ANALOG
} RadioSystemType;

class RadioSystem {
public:
	RadioSystem() = 0;
	virtual ~RadioSystem();

	const RadioSystemType type;
	char* name;
};

class AnalogSystem: public RadioSystem {
public:
	AnalogSystem();
	virtual ~AnalogSystem();
};

#endif /* RADIOSYSTEM_H_ */
