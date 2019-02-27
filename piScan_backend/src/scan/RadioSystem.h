/*
 * RadioSysten.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef RADIOSYSTEM_H_
#define RADIOSYSTEM_H_

#include <stdint.h>
#include <string>

#include "Entry.h"

typedef enum {
	SYSTEM_ANALOG
} RadioSystemType;

class RadioSystem {
public:
	RadioSystem() {};
	virtual ~RadioSystem() {};

	virtual Entry* operator[](size_t pos) = 0;

	size_t size() { return _size; }
private:
	//const RadioSystemType type;
	const std::string tag;
	size_t _size = 0;
};

class AnalogSystem: public RadioSystem {
public:
	AnalogSystem() : RadioSystem() {};
	~AnalogSystem() {};

	Entry* operator[](size_t pos) { return nullptr; };
};

#endif /* RADIOSYSTEM_H_ */
