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
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>

#include "scandefs.h"
#include "Entry.h"
#include "loguru.hpp"

#define ANALOG_SYSTEM_HASH	"analog"

#define SYS_TYPE_KEY		"systype"
#define CHANNELS_KEY		"channels"

using namespace boost::property_tree;

namespace piscan {

typedef enum {
	SYSTEM_ANALOG
} RadioSystemType;

class RadioSystem {
public:
	RadioSystem(std::string tag, bool lo) : _tag(tag), _lockout(lo) {};
	virtual ~RadioSystem() {};

	virtual EntryPtr operator[](size_t pos) = 0;

	virtual size_t size() { return numEntries; }

	std::string tag() { return _tag; };

	virtual void addEntry(EntryPtr entry) = 0;

	virtual ptree getPropertyTree() = 0;

	bool lockedOut() { return _lockout; };
	void lockout(bool val = true) { _lockout = val; };

	size_t getSysIndex() { return _sysIndex; };
	void setSysIndex(size_t index) { _sysIndex = index; };
private:
	//const RadioSystemType type;
	const std::string _tag;

	bool _lockout;
	size_t _sysIndex = 0;
protected:
	size_t numEntries = 0;

	static std::unordered_map<std::string, std::function<EntryPtr(ptree&)>> makeEntry;
	static EntryPtr makeFMChannel(ptree& pt);
	static EntryPtr makeAMChannel(ptree& pt);
	static EntryPtr makePLChannel(ptree& pt);
	static EntryPtr makeDCChannel(ptree& pt);
};

class AnalogSystem: public RadioSystem {
public:
	AnalogSystem(std::string tag, bool lo) : RadioSystem(tag, lo) {};
	AnalogSystem(ptree pt, size_t index);
	~AnalogSystem() {};

	virtual EntryPtr operator[](size_t pos) { return entries[pos]; };

	/*virtual size_t size() {
		return entries.size();
	}*/

	virtual void addEntry(EntryPtr entry){
		if(entry != nullptr){
			entry->setEntryIndex(numEntries);
			entry->setSysIndex(getSysIndex());
			entries.push_back(entry);
			numEntries++;
		}
	}

	ptree getPropertyTree();
protected:
	std::vector<EntryPtr> entries;
};

}
#endif /* RADIOSYSTEM_H_ */
