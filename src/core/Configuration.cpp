/*
 * Configuration.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#include <iomanip>
#include <ctime>
#include <sstream>
#include <experimental/filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Configuration.h"
#include "loguru.hpp"

using namespace piscan;
using namespace std::experimental;
using namespace boost::property_tree;

Configuration* Configuration::_config;
Configuration& Configuration::getConfig() {
	if(_config == nullptr)
		_config = new Configuration();

	return *_config;
};

Configuration::Configuration() {
	_configPath = DATABASE_PATH;

}

Configuration::~Configuration() {
	// TODO Auto-generated destructor stub
}

void Configuration::setWorkingPath(std::string path){
	filesystem::path configPath(path);
	if(!filesystem::exists(configPath))
		if(!filesystem::create_directory(configPath)){
			LOG_F(ERROR, "Could not create working directory %s, reverting to default", path.c_str());
			_configPath = DATABASE_PATH;
			return;
		}

	_configPath = path;
	LOG_F(INFO, "Config directory: %s", path.c_str());
}

std::string Configuration::getWorkingPath() {
	return _configPath;
}

void Configuration::loadConfig(){
	ptree pt;
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += CONFIG_FILE;
	if(!filesystem::exists(path)){
		LOG_F(INFO, "No config file exists, using defaults");
		return;
	}

	LOG_F(1, "Config path: %s", path.c_str());
	LOG_F(INFO, "Loading config");

	try{
		read_json(path, pt);
	} catch(boost::property_tree::json_parser_error& e){
		LOG_F(WARNING, "Error parsing file: %s", e.message().c_str());
	}

	_generalConfig.logfileVerbosity = pt.get("config.general.log_verbosity", DEFAULT_LOGFILE_VERBOSITY);

	_socketConfig.maxConnections = pt.get("config.socket.max_connections", MAX_TCP_CONNECTIONS);
	_socketConfig.tcpPort = pt.get("config.socket.port", DEFAULT_TCP_PORT);

}

void Configuration::loadState(){
	ptree pt;
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += STATE_FILE;;
	if(!filesystem::exists(path)){
		LOG_F(INFO, "No state file exists, using defaults");
		return;
	}

	LOG_F(1, "State path: %s", path.c_str());
	LOG_F(INFO, "Loading saved state");

	try{
		read_json(path, pt);
	} catch(boost::property_tree::json_parser_error& e){
		LOG_F(WARNING, "Error parsing file: %s", e.message().c_str());
	}

	_demodState.gain = pt.get("state.demod.gain", DEFAULT_GAIN);
	_demodState.squelch = pt.get("state.demod.squelch", DEFAULT_SQUELCH);

}

void Configuration::saveConfig(){
	ptree pt;
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += CONFIG_FILE;

	LOG_F(INFO, "Saving config");

	pt.put("config.general.log_verbosity", _generalConfig.logfileVerbosity);

	pt.put("config.socket.port", _socketConfig.tcpPort);
	pt.put("config.socket.max_connections", _socketConfig.maxConnections);

	write_json(path.c_str(), pt);
}

void Configuration::saveState(){
	ptree pt;
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += STATE_FILE;

	LOG_F(INFO, "Saving state");

	pt.put("state.demod.gain", _demodState.gain);
	pt.put("state.demod.squelch", _demodState.squelch);

	write_json(path.c_str(), pt);
}

std::string Configuration::getLogfilePath(){
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += LOG_FOLDER;

	if(!filesystem::exists(path)){
		if(!filesystem::create_directory(path)){
			LOG_F(ERROR, "Error creating logfile directory");
			path = DATABASE_PATH;
		}
	}

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y%m%d-%H%M%S");
	auto str = oss.str();

	path += filesystem::path::preferred_separator;
	path += str;
	path += LOGFILE_EXT;
	return path.string();
}

