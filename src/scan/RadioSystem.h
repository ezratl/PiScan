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
#include <vector>

#include "Entry.h"
#include "loguru.hpp"

namespace piscan {

typedef enum {
	SYSTEM_ANALOG
} RadioSystemType;

class RadioSystem {
public:
	RadioSystem(std::string tag, bool lo) : _tag(tag), _lockout(lo) {};
	virtual ~RadioSystem() {};

	virtual std::shared_ptr<Entry> operator[](size_t pos) = 0;

	virtual size_t size() { return numEntries; }

	std::string tag() { return _tag; };

	virtual void addEntry(std::shared_ptr<Entry> entry) = 0;
private:
	//const RadioSystemType type;
	const std::string _tag;

	bool _lockout;
protected:
	size_t numEntries = 0;
};

class AnalogSystem: public RadioSystem {
public:
	AnalogSystem(std::string tag, bool lo) : RadioSystem(tag, lo) {};
	~AnalogSystem() {};

	virtual std::shared_ptr<Entry> operator[](size_t pos) { return entries[pos]; };

	/*virtual size_t size() {
		return entries.size();
	}*/

	virtual void addEntry(std::shared_ptr<Entry> entry){
		if(entry != nullptr){
			entries.push_back(entry);
			numEntries++;
		}
	}

protected:
	std::vector<std::shared_ptr<Entry>> entries;
};
}
#endif /* RADIOSYSTEM_H_ */
