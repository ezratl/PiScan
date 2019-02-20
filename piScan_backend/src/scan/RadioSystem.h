/*
 * RadioSysten.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef RADIOSYSTEM_H_
#define RADIOSYSTEM_H_

#include <stdint.h>

#include "Entry.h"

typedef enum {
	SYSTEM_ANALOG
} RadioSystemType;

class RadioSystem {
public:
	RadioSystem() = 0;
	virtual ~RadioSystem();

	virtual Entry& operator[](size_t pos) = 0;

	size_t size() { return _size; }
private:
	//const RadioSystemType type;
	const char tag[];
	size_t _size = 0;
};

class AnalogSystem: public RadioSystem {
public:
	AnalogSystem();
	virtual ~AnalogSystem();

	Entry& operator[](size_t pos);
};

#endif /* RADIOSYSTEM_H_ */
