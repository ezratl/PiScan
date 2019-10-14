/*
 * RadioSysten.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#include "RadioSystem.h"

using namespace piscan;
using namespace boost::property_tree;

std::unordered_map<std::string, std::function<EntryPtr(ptree&)>> RadioSystem::makeEntry = {
		{ FM_CHANNEL_HASH, makeFMChannel },
		{ AM_CHANNEL_HASH, makeAMChannel },
		{ PL_CHANNEL_HASH, makePLChannel },
		{ DC_CHANNEL_HASH, makeDCChannel }
};

EntryPtr RadioSystem::makeFMChannel(ptree& pt) {
	std::string tag = pt.get(TAG_KEY, "");
	long long freq = pt.get(FREQ_KEY, 0);
	bool lockout = pt.get(LOCKOUT_KEY, false);
	int delay = pt.get(DELAY_KEY, 0.0);
	if(freq == 0)
		return nullptr;

	return std::make_shared<FMChannel>(freq, tag, lockout, delay);
}

EntryPtr RadioSystem::makeAMChannel(ptree& pt) {
	std::string tag = pt.get(TAG_KEY, "");
	long long freq = pt.get(FREQ_KEY, 0);
	bool lockout = pt.get(LOCKOUT_KEY, false);
	int delay = pt.get(DELAY_KEY, 0.0);
	if(freq == 0)
		return nullptr;

	return std::make_shared<AMChannel>(freq, tag, lockout, delay);
}

EntryPtr RadioSystem::makePLChannel(ptree& pt) {
	std::string tag = pt.get(TAG_KEY, "");
	long long freq = pt.get(FREQ_KEY, 0);
	bool lockout = pt.get(LOCKOUT_KEY, false);
	int delay = pt.get(DELAY_KEY, 0.0);
	if(freq == 0)
		return nullptr;

	float tone = pt.get(TONE_KEY, 0.0);
	return std::make_shared<PLChannel>(freq, tone, tag, lockout, delay);
}

EntryPtr RadioSystem::makeDCChannel(ptree& pt) {
	std::string tag = pt.get(TAG_KEY, "");
	long long freq = pt.get(FREQ_KEY, 0);
	bool lockout = pt.get(LOCKOUT_KEY, false);
	int delay = pt.get(DELAY_KEY, 0.0);
	if(freq == 0)
		return nullptr;

	unsigned int code = pt.get(CODE_KEY, 0);
	return std::make_shared<DCChannel>(freq, code, tag, lockout, delay);
}

AnalogSystem::AnalogSystem(ptree pt, size_t index) : RadioSystem(pt.get(TAG_KEY, ""), pt.get(LOCKOUT_KEY, false)) {
	setSysIndex(index);
	RAW_LOG_F(1, "New analog system\n\tTag: %s\n\tLockout: %i", tag().c_str(), lockedOut());

	BOOST_FOREACH(ptree::value_type& v, pt.get_child(CHANNELS_KEY)){
		ptree entryPT = v.second;
		EntryPtr entry;
		std::string type = entryPT.get(CHAN_TYPE_KEY, "");
		try {
			entry = makeEntry[type](entryPT);
		} catch (std::exception& e) {
			LOG_F(WARNING, "Invalid entry type: %s", type.c_str());
			continue;
		}

		if(entry == nullptr)
			continue;

		RAW_LOG_F(1, "\t\tNew %s entry", type.c_str());
		RAW_LOG_F(1, "\t\tTag: %s", entry->tag().c_str());
		RAW_LOG_F(1, "\t\tFreq: %li", entry->freq());
		RAW_LOG_F(1, "\t\tLockout: %i", entry->isLockedOut());
		RAW_LOG_F(1, "\t\tDelay: %i", entry->delay());

		addEntry(entry);
	}
}

ptree AnalogSystem::getPropertyTree() {
	ptree pt, channels;
	pt.put(SYS_TYPE_KEY, ANALOG_SYSTEM_HASH);
	pt.put(TAG_KEY, tag());
	pt.put(LOCKOUT_KEY, lockedOut());
	BOOST_FOREACH(EntryPtr p, entries){
		channels.push_back(std::make_pair("", p->getPropertyTree()));
	}

	pt.add_child(CHANNELS_KEY, channels);

	return pt;
}
