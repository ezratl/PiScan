/*
 * Configuration.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#include "Configuration.h"

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

#ifndef DATABASE_PATH
#error "Database path not defined, please run CMake"
#endif

using namespace std::experimental;
using namespace boost::property_tree;

namespace piscan::config {

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

void Configuration::setWorkingDirectory(std::string path){
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

std::string Configuration::getWorkingDirectory() {
	return _configPath;
}

void Configuration::loadConfig(){
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
		read_json(path, _ptConfig);
	} catch(boost::property_tree::json_parser_error& e){
		LOG_F(WARNING, "Error parsing file: %s", e.message().c_str());
	}

	_generalConfig.logfileVerbosity = _ptConfig.get("config.general.log_verbosity", DEFAULT_LOGFILE_VERBOSITY);

	_socketConfig.maxConnections = _ptConfig.get("config.socket.max_connections", MAX_TCP_CONNECTIONS);
	_socketConfig.tcpPort = _ptConfig.get("config.socket.port", DEFAULT_TCP_PORT);
	_socketConfig.spawnLocalClient = _ptConfig.get("config.socket.use_gui", DEFAULT_SPAWN_CLIENT);
	_socketConfig.pythonClient = _ptConfig.get("config.socket.client_path", DEFAULT_PY_CLIENT_LOCATION);
	_socketConfig.pythonBinary = _ptConfig.get("config.socket.python_path", DEFAULT_PY_ENV_LOCATION);

	_demodConfig.retuneDelay = _ptConfig.get("config.demod.retune_delay", static_cast<long int>(TUNER_RETUNE_TIME));
	_demodConfig.demodDelay = _ptConfig.get("config.demod.demod_delay", static_cast<long int>(DEMOD_BUFFER_TIME));
	_demodConfig.squelchType = _ptConfig.get("config.demod.squelch_mode", DEFAULT_SQUELCH_MODE);

	_rtspConfig.rtspPort = _ptConfig.get("config.audio_stream.rtsp_port", DEFAULT_RTSP_PORT);
	_rtspConfig.httpTunneling = _ptConfig.get("config.audio_stream.http_tunneling", DEFAULT_RTSP_OVER_HTTP);

}

void Configuration::loadState(){
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
		read_json(path, _ptState);
	} catch(boost::property_tree::json_parser_error& e){
		LOG_F(WARNING, "Error parsing file: %s", e.message().c_str());
	}

	_demodState.gain = _ptState.get("state.demod.gain", DEFAULT_GAIN);
	_demodState.squelch = _ptState.get("state.demod.squelch", DEFAULT_SQUELCH);

	_scannerState.scanState = _ptState.get("state.scanner.state", DEFAULT_SCAN_STATE);
	_scannerState.manualFreq = _ptState.get("state.scanner.manual_freq", 100000000);
	_scannerState.manualModualtion = _ptState.get("state.scanner.manual_mode", "");
	_scannerState.holdKey = _ptState.get("state.scanner.hold_key", "");
	try{
		BOOST_FOREACH(ptree::value_type& v, _ptState.get_child("state.scanner.hold_index")) {
			int ind = std::stoi(v.second.data());
			_scannerState.holdIndex.push_back(ind);
		}
	}catch(std::exception& e){};
}

void Configuration::saveConfig(){
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += CONFIG_FILE;

	LOG_F(INFO, "Saving config");

	_ptConfig.put("config.general.log_verbosity", _generalConfig.logfileVerbosity);

	_ptConfig.put("config.socket.port", _socketConfig.tcpPort);
	_ptConfig.put("config.socket.max_connections", _socketConfig.maxConnections);
	_ptConfig.put("config.socket.use_gui", _socketConfig.spawnLocalClient);
	_ptConfig.put("config.socket.client_path", _socketConfig.pythonClient);
	_ptConfig.put("config.socket.python_path", _socketConfig.pythonBinary);

	_ptConfig.put("config.demod.retune_delay", _demodConfig.retuneDelay);
	_ptConfig.put("config.demod.demod_delay", _demodConfig.demodDelay);
	_ptConfig.put("config.demod.squelch_mode", _demodConfig.squelchType);

	_ptConfig.put("config.audio_stream.rtsp_port", _rtspConfig.rtspPort);
	_ptConfig.put("config.audio_stream.http_tunneling", _rtspConfig.httpTunneling);

	write_json(path.c_str(), _ptConfig);
}

void Configuration::saveState(){
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += STATE_FILE;

	LOG_F(INFO, "Saving state");

	_ptState.put("state.demod.gain", _demodState.gain);
	_ptState.put("state.demod.squelch", _demodState.squelch);

	_ptState.put("state.scanner.state", _scannerState.scanState);
	_ptState.put("state.scanner.manual_freq", _scannerState.manualFreq);
	_ptState.put("state.scanner.manual_mode", _scannerState.manualModualtion);
	_ptState.put("state.scanner.hold_key", _scannerState.holdKey);
	ptree ind;
	BOOST_FOREACH(int i, _scannerState.holdIndex) {
		ptree val;
		val.put("", i);
		ind.push_back(std::make_pair("", val));
	}
	_ptState.put_child("state.scanner.hold_index", ind);

	write_json(path.c_str(), _ptState);
}

std::string Configuration::getLogDirectory(){
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += LOG_FOLDER;

	if (!filesystem::exists(path)) {
		if (!filesystem::create_directory(path)) {
			LOG_F(ERROR, "Error creating logfile directory");
			path = DATABASE_PATH;
		}
	}
	return path;
}

std::string Configuration::getDatedLogPath(){
	filesystem::path path(getLogDirectory());

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y%m%d-%H%M%S");
	auto str = oss.str();

	path += filesystem::path::preferred_separator;
	path += str;
	path += LOGFILE_EXT;
	return path;
}

std::string Configuration::getLatestLogPath(){
	filesystem::path path(getLogDirectory());
	path += filesystem::path::preferred_separator;
	path += LOG_PATH;
	return path;
}

}
