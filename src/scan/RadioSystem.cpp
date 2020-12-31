/*
 * RadioSysten.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#include "scan_types.h"
#include "RadioSystem.h"
#include "Entry.h"

using ptree = boost::property_tree::ptree;

namespace piscan::scan {

std::unordered_map<std::string, std::function<EntryPtr(ptree&)>> RadioSystem::makeEntry = {
		{ database::entry::channel::type_fm, makeFMChannel },
		{ database::entry::channel::type_am, makeAMChannel },
		{ database::entry::channel::type_pl, makePLChannel },
		{ database::entry::channel::type_dc, makeDCChannel }
};

EntryPtr RadioSystem::makeFMChannel(ptree& pt) {
	std::string tag = pt.get(database::entry::tag_key, "");
	long long freq = pt.get(database::entry::freq_key, 0);
	bool lockout = pt.get(database::entry::lockout_key, false);
	int delay = pt.get(database::entry::delay_key, 0);
	if(freq == 0)
		return nullptr;

	return std::make_shared<FMChannel>(freq, tag, lockout, delay);
}

EntryPtr RadioSystem::makeAMChannel(ptree& pt) {
	std::string tag = pt.get(database::entry::tag_key, "");
	long long freq = pt.get(database::entry::freq_key, 0);
	bool lockout = pt.get(database::entry::lockout_key, false);
	int delay = pt.get(database::entry::delay_key, 0);
	if(freq == 0)
		return nullptr;

	return std::make_shared<AMChannel>(freq, tag, lockout, delay);
}

EntryPtr RadioSystem::makePLChannel(ptree& pt) {
	std::string tag = pt.get(database::entry::tag_key, "");
	long long freq = pt.get(database::entry::freq_key, 0);
	bool lockout = pt.get(database::entry::lockout_key, false);
	int delay = pt.get(database::entry::delay_key, 0);
	if(freq == 0)
		return nullptr;

	std::string tone = pt.get(database::entry::tone_key, "");
	return std::make_shared<PLChannel>(freq, tone, tag, lockout, delay);
}

EntryPtr RadioSystem::makeDCChannel(ptree& pt) {
	std::string tag = pt.get(database::entry::tag_key, "");
	long long freq = pt.get(database::entry::freq_key, 0);
	bool lockout = pt.get(database::entry::lockout_key, false);
	int delay = pt.get(database::entry::delay_key, 0);
	if(freq == 0)
		return nullptr;

	std::string code = pt.get(database::entry::code_key, "");
	return std::make_shared<DCChannel>(freq, code, tag, lockout, delay);
}

AnalogSystem::AnalogSystem(ptree pt, size_t index) : RadioSystem(pt.get(database::entry::tag_key, ""), pt.get(database::entry::lockout_key, false)) {
	setSysIndex(index);

	BOOST_FOREACH(ptree::value_type& v, pt.get_child(database::systems::channels_key)){
		ptree entryPT = v.second;
		EntryPtr entry;
		std::string type = entryPT.get(database::entry::channel::type_key, "");
		try {
			entry = makeEntry[type](entryPT);
		} catch (std::exception& e) {
			LOG_F(WARNING, "Invalid entry type: %s", type.c_str());
			continue;
		}

		if(entry == nullptr)
			continue;

		addEntry(entry);
	}
}

void AnalogSystem::addEntry(EntryPtr entry){
		if(entry != nullptr){
			entry->setEntryIndex(numEntries);
			entry->setSysIndex(getSysIndex());
			entries.push_back(entry);
			numEntries++;
		}
}

ptree AnalogSystem::getPropertyTree() {
	ptree pt, channels;
	pt.put(database::systems::type_key, database::systems::type_analog);
	pt.put(database::systems::tag_key, tag());
	pt.put(database::systems::lockout_key, lockedOut());
	BOOST_FOREACH(EntryPtr p, entries){
		channels.push_back(std::make_pair("", p->getPropertyTree()));
	}

	pt.add_child(database::systems::channels_key, channels);

	return pt;
}

}
