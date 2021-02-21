#include <iostream>

#include <iomanip>
#include <ctime>
#include <sstream>
#include <experimental/filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional/optional.hpp>

#include "loguru.hpp"
#include "Configuration.h"

using namespace std::experimental;
using namespace boost::property_tree;

namespace piscan {
namespace config {

State* State::_state;
State& State::getState() {
	if(_state == nullptr)
		_state = new State();

	return *_state;
};

State::State() : _demodState(*this, path::state_base_path), _scannerState(*this, path::state_base_path) {
}

bool State::loadFromFile() {
    filesystem::path path(Configuration::getConfig().getWorkingDirectory());
	path += filesystem::path::preferred_separator;
	path += STATE_FILE;;
	if(!filesystem::exists(path)){
		LOG_F(INFO, "No state file exists, using defaults");
		return true;
	}

	LOG_F(1, "State path: %s", path.c_str());
	LOG_F(INFO, "Loading saved state");

	try{
		read_json(path, _pt);
	} catch(boost::property_tree::json_parser_error& e){
		LOG_F(WARNING, "Error parsing file: %s", e.message().c_str());
        return false;
	}

    loadAll();

    return true;
}

bool State::saveToFile() {
	filesystem::path path(Configuration::getConfig().getWorkingDirectory());
	path += filesystem::path::preferred_separator;
	path += STATE_FILE;

	LOG_F(INFO, "Saving state");

    saveAll();

	write_json(path.c_str(), _pt);

    return true;
}

void State::loadAll(){
    _demodState.load();
    _scannerState.load();
}

void State::saveAll(){
    _demodState.save();
    _scannerState.save();
}

void State::invalidate() {

}

void DemodState::load() {
    gain = base.pTree().get(basePath + path::path_seperator + "demod" + path::path_seperator + "gain", DEFAULT_GAIN);
	squelch = base.pTree().get(basePath + path::path_seperator + "demod" + path::path_seperator + "squelch", DEFAULT_SQUELCH);
}

void DemodState::save() {
    base.pTree().put(basePath + path::path_seperator + "demod" + path::path_seperator + "gain", gain);
	base.pTree().put(basePath + path::path_seperator + "demod" + path::path_seperator + "squelch", squelch);
    base.invalidate();
}

void ScannerState::load() {
    scanState = base.pTree().get(basePath + path::path_seperator + "scanner" + path::path_seperator + "state", DEFAULT_SCAN_STATE);
	manualFreq = base.pTree().get(basePath + path::path_seperator + "scanner" + path::path_seperator + "manual_freq", 100000000);
	manualModualtion = base.pTree().get(basePath + path::path_seperator + "scanner" + path::path_seperator + "manual_mode", "");
	holdKey = base.pTree().get(basePath + path::path_seperator + "scanner" + path::path_seperator + "hold_key", "");
	try {
		BOOST_FOREACH(ptree::value_type& v, base.pTree().get_child(basePath + path::path_seperator + "scanner" + path::path_seperator + "hold_index")) {
			int ind = std::stoi(v.second.data());
			holdIndex.push_back(ind);
		}
	} catch(std::exception& e){};
}

void ScannerState::save() {
    base.pTree().put(basePath + path::path_seperator + "scanner" + path::path_seperator + "state", scanState);
	base.pTree().put(basePath + path::path_seperator + "scanner" + path::path_seperator + "manual_freq", manualFreq);
	base.pTree().put(basePath + path::path_seperator + "scanner" + path::path_seperator + "manual_mode", manualModualtion);
	base.pTree().put(basePath + path::path_seperator + "scanner" + path::path_seperator + "hold_key", holdKey);
	ptree ind;
	BOOST_FOREACH(int i, holdIndex) {
		ptree val;
		val.put("", i);
		ind.push_back(std::make_pair("", val));
	}
	base.pTree().put_child(basePath + path::path_seperator + "scanner" + path::path_seperator + "hold_index", ind);
    base.invalidate();
}

}
}
