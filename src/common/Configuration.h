#pragma once

/*
 * Configuration.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

#include "constants.h"

#define DEFAULT_LOGFILE_VERBOSITY 0
#define LOG_PATH		"latest.log"
#define LOG_FOLDER		"logs"
#define LOGFILE_EXT		".log"

#define CONFIG_FILE		"config.json"
#define STATE_FILE		"state.json"
#define SYSTEMS_FILE	"systems.json"

namespace piscan {
namespace config {

namespace path {
	static constexpr char path_seperator[] = ".";
	static constexpr char config_base_path[] = "config";
	static constexpr char state_base_path[] = "state";
}

class ConfigManager {
public:
	virtual boost::property_tree::ptree& pTree() = 0;

	virtual void invalidate() = 0;
};

struct ConfigBase {
public:
	ConfigBase(ConfigManager& cm, std::string path) : base(cm), basePath(std::move(path)) {};

	virtual void save() = 0;
	virtual void load() = 0;

protected:
	ConfigManager& base;
	std::string basePath;
};

struct GeneralConfig : public ConfigBase {
public:
	GeneralConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

	int logfileVerbosity = DEFAULT_LOGFILE_VERBOSITY;

	virtual void save();
	virtual void load();
};

#define DEFAULT_TCP_PORT			1234
#define MAX_TCP_CONNECTIONS			5
#define DEFAULT_SPAWN_CLIENT		false
#define DEFAULT_PY_CLIENT_LOCATION	"./"
#define DEFAULT_PY_ENV_LOCATION	"python"

struct SocketServerConfig : public ConfigBase {
public:
	SocketServerConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

	int tcpPort = DEFAULT_TCP_PORT;
	int maxConnections = MAX_TCP_CONNECTIONS;
	bool spawnLocalClient = DEFAULT_SPAWN_CLIENT;
	std::string pythonClient = DEFAULT_PY_CLIENT_LOCATION;
	std::string pythonBinary = DEFAULT_PY_ENV_LOCATION;

	virtual void save();
	virtual void load();
};

#define TUNER_RETUNE_TIME	225000
#define DEMOD_BUFFER_TIME	10000

#define SQUELCH_DBM	0
#define SQUELCH_SNR	1
#define SQUELCH_PCT 2

#define DEFAULT_SQUELCH_MODE	(SQUELCH_DBM)

struct DemodConfig : public ConfigBase {
	DemodConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

	long int retuneDelay = TUNER_RETUNE_TIME;
	long int demodDelay = DEMOD_BUFFER_TIME;
	int squelchType = DEFAULT_SQUELCH_MODE;
	
	virtual void save();
	virtual void load();
};

#define DEFAULT_RANK	0
#define DEFAULT_DESC	"NULL"
#define DEFAULT_DRIVER	"null"
#define DEFAULT_PPM		0
#define DEFAULT_TUNER_SAMPLE_RATE	2048000

struct TunerConfig {
	int rank = DEFAULT_RANK;
	std::string description = DEFAULT_DESC;
	std::string driver = DEFAULT_DRIVER;
	int ppmCorrection = DEFAULT_PPM;
	long int sampleRate = DEFAULT_TUNER_SAMPLE_RATE;
};

#define DEFAULT_RTSP_PORT	8554
#define DEFAULT_RTSP_OVER_HTTP	false

struct AudioServerConfig : public ConfigBase {
	AudioServerConfig(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

	int rtspPort = DEFAULT_RTSP_PORT;
	bool httpTunneling = DEFAULT_RTSP_OVER_HTTP;
		
	virtual void save();
	virtual void load();
};

#define DEFAULT_SQUELCH	0
#define DEFAULT_GAIN	(AUTO_GAIN)

struct DemodState : public ConfigBase {
	DemodState(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

	float squelch = DEFAULT_SQUELCH;
	float gain = DEFAULT_GAIN;
			
	virtual void save();
	virtual void load();
};

#define	SCAN_STATE_SCAN		0
#define SCAN_STATE_HOLD		1
#define SCAN_STATE_MANUAL	2
#define DEFAULT_SCAN_STATE	(SCAN_STATE_SCAN)

struct ScannerState : public ConfigBase {
	ScannerState(ConfigManager& cm, std::string path) : ConfigBase(cm, std::move(path)) {};

	int scanState = DEFAULT_SCAN_STATE;
	long long manualFreq = 0;
	std::string manualModualtion = "";
	std::vector<int> holdIndex = {};
	std::string holdKey = "";
			
	virtual void save();
	virtual void load();
};

class Configuration : public ConfigManager {
public:
	static Configuration& getConfig();

	void setWorkingDirectory(std::string path);
	std::string getWorkingDirectory();

	bool loadFromFile();
	bool saveToFile();

	void loadAll();
	void saveAll();

	GeneralConfig& getGeneralConfig() { return _generalConfig; };
	SocketServerConfig& getSocketConfig() { return _socketConfig; };
	DemodConfig& getDemodConfig() { return _demodConfig; };
	AudioServerConfig& getAudioServerConfig() { return _rtspConfig; };

	std::string getLogDirectory();
	std::string getDatedLogPath();
	std::string getLatestLogPath();

	boost::property_tree::ptree& pTree() { return _pt; };
	void invalidate();

private:
	static Configuration* _config;
	std::string _configPath;

	GeneralConfig _generalConfig;
	SocketServerConfig _socketConfig;
	DemodConfig _demodConfig;
	AudioServerConfig _rtspConfig;

	Configuration();

	boost::property_tree::ptree _pt;
};

class State : public ConfigManager {
public:
	static State& getState();

	bool loadFromFile();
	bool saveToFile();

	void loadAll();
	void saveAll();

	DemodState& getDemodState() { return _demodState; };
	ScannerState& getScannerState() { return _scannerState; };

	boost::property_tree::ptree& pTree() { return _pt; };
	void invalidate();

private:
	static State* _state;

	DemodState _demodState;
	ScannerState _scannerState;

	State();

	boost::property_tree::ptree _pt;
};

}
}
