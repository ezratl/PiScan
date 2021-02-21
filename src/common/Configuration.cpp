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

namespace piscan {
namespace config {

Configuration* Configuration::_config;
Configuration& Configuration::getConfig() {
	if(_config == nullptr)
		_config = new Configuration();

	return *_config;
};

Configuration::Configuration() : _generalConfig(*this, path::config_base_path), 
		_socketConfig(*this, path::config_base_path), _demodConfig(*this, path::config_base_path), 
		_rtspConfig(*this, path::config_base_path), _tunerList(*this, path::config_base_path) {
	_configPath = DATABASE_PATH;
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

bool Configuration::loadFromFile() {
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += CONFIG_FILE;
	if(!filesystem::exists(path)){
		LOG_F(INFO, "No config file exists, using defaults");
		return true;
	}

	LOG_F(1, "Config path: %s", path.c_str());
	LOG_F(INFO, "Loading config");

	try{
		read_json(path, _pt);
	} catch(boost::property_tree::json_parser_error& e){
		LOG_F(WARNING, "Error parsing file: %s", e.message().c_str());
		return false;
	}

	loadAll();

	return true;
}

bool Configuration::saveToFile() {
	filesystem::path path(_configPath);
	path += filesystem::path::preferred_separator;
	path += CONFIG_FILE;

	LOG_F(INFO, "Saving config");

	saveAll();

	write_json(path.c_str(), _pt);

	return true;
}

void Configuration::loadAll(){
	_generalConfig.load();
	_socketConfig.load();
	_demodConfig.load();
	_rtspConfig.load();
	_tunerList.load();
}

void Configuration::saveAll(){
	_generalConfig.save();
	_socketConfig.save();
	_demodConfig.save();
	_rtspConfig.save();
	_tunerList.save();
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

void Configuration::invalidate() {
	
}

/* Config implementations */
GeneralConfig::GeneralConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

void GeneralConfig::load() {
	logfileVerbosity = base.pTree().get(basePath + path::path_seperator + "general" + path::path_seperator + "log_verbosity", DEFAULT_LOGFILE_VERBOSITY);
}

void GeneralConfig::save() {
	logfileVerbosity = base.pTree().get(basePath + path::path_seperator + "general" + path::path_seperator + "log_verbosity", DEFAULT_LOGFILE_VERBOSITY);
	base.invalidate();
}

SocketServerConfig::SocketServerConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

void SocketServerConfig::load() {
	maxConnections = base.pTree().get(basePath + path::path_seperator + "socket" + path::path_seperator + "max_connections", MAX_TCP_CONNECTIONS);
	tcpPort = base.pTree().get(basePath + path::path_seperator + "socket" + path::path_seperator + "port", DEFAULT_TCP_PORT);
	spawnLocalClient = base.pTree().get(basePath + path::path_seperator + "socket" + path::path_seperator + "use_gui", DEFAULT_SPAWN_CLIENT);
	pythonClient = base.pTree().get(basePath + path::path_seperator + "socket" + path::path_seperator + "client_path", DEFAULT_PY_CLIENT_LOCATION);
	pythonBinary = base.pTree().get(basePath + path::path_seperator + "socket" + path::path_seperator + "python_path", DEFAULT_PY_ENV_LOCATION);
}

void SocketServerConfig::save() {
	base.pTree().put(basePath + path::path_seperator + "socket" + path::path_seperator + "port", tcpPort);
	base.pTree().put(basePath + path::path_seperator + "socket" + path::path_seperator + "max_connections", maxConnections);
	base.pTree().put(basePath + path::path_seperator + "socket" + path::path_seperator + "use_gui", spawnLocalClient);
	base.pTree().put(basePath + path::path_seperator + "socket" + path::path_seperator + "client_path", pythonClient);
	base.pTree().put(basePath + path::path_seperator + "socket" + path::path_seperator + "python_path", pythonBinary);
	base.invalidate();
}

DemodConfig::DemodConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};
		
void DemodConfig::load() {
	retuneDelay = base.pTree().get(basePath + path::path_seperator + "demod" + path::path_seperator + "retune_delay", static_cast<long int>(TUNER_RETUNE_TIME));
	demodDelay = base.pTree().get(basePath + path::path_seperator + "demod" + path::path_seperator + "demod_delay", static_cast<long int>(DEMOD_BUFFER_TIME));
	squelchType = base.pTree().get(basePath + path::path_seperator + "demod" + path::path_seperator + "squelch_mode", DEFAULT_SQUELCH_MODE);
}

void DemodConfig::save() {
	base.pTree().put(basePath + path::path_seperator + "demod" + path::path_seperator + "retune_delay", retuneDelay);
	base.pTree().put(basePath + path::path_seperator + "demod" + path::path_seperator + "demod_delay", demodDelay);
	base.pTree().put(basePath + path::path_seperator + "demod" + path::path_seperator + "squelch_mode", squelchType);
	base.invalidate();
}

AudioServerConfig::AudioServerConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};
		
void AudioServerConfig::load() {
	rtspPort = base.pTree().get(basePath + path::path_seperator + "audio_stream" + path::path_seperator + "rtsp_port", DEFAULT_RTSP_PORT);
	httpTunneling = base.pTree().get(basePath + path::path_seperator + "audio_stream" + path::path_seperator + "http_tunneling", DEFAULT_RTSP_OVER_HTTP);
}

void AudioServerConfig::save() {
	base.pTree().put(basePath + path::path_seperator + "audio_stream" + path::path_seperator + "rtsp_port", rtspPort);
	base.pTree().put(basePath + path::path_seperator + "audio_stream" + path::path_seperator + "http_tunneling", httpTunneling);
	base.invalidate();
}

TunerList::TunerList(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

void TunerList::load() {
	try {
		BOOST_FOREACH(ptree::value_type& v, base.pTree().get_child(basePath + path::path_seperator + "tuners")){
			ptree tunerPT = v.second;
			TunerConfig tuner;
			tuner.rank = tunerPT.get("rank", 0);
			tuner.descriptor = tunerPT.get("descriptor", "null");
			tuner.driver = tunerPT.get("driver", "null");
			tuner.ppmCorrection = tunerPT.get("ppm_correction", 0);
			tuner.sampleRate = tunerPT.get("sample_rate", 2048000);

			RAW_LOG_F(2, "Tuner:");
			RAW_LOG_F(2, "\tRank: %i", tuner.rank);
			RAW_LOG_F(2, "\tDescriptor: %s", tuner.descriptor.c_str());
			RAW_LOG_F(2, "\tDriver: %s", tuner.driver.c_str());
			RAW_LOG_F(2, "\tPPM: %i", tuner.ppmCorrection);
			RAW_LOG_F(2, "\tSample rate: %li", tuner.sampleRate);

			tuners.insert(std::move(tuner));
		}
	} catch (std::exception& e) {
		
	}
}

void TunerList::save() {
	ptree tunerList, p;

	base.pTree().erase(basePath + path::path_seperator + "tuners");

	BOOST_FOREACH(const TunerConfig& c, tuners){
		p.put("rank", c.rank);
		p.put("descriptor", c.descriptor);
		p.put("driver", c.driver);
		p.put("ppm_correction", c.ppmCorrection);
		p.put("sample_rate", c.sampleRate);
		tunerList.push_back(std::make_pair("", p));
	}

	base.pTree().put_child(basePath + path::path_seperator + "tuners", tunerList);
}

}
}
