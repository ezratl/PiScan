/*
 * Configuration.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#ifndef SERVER_CONFIGURATION_H_
#define SERVER_CONFIGURATION_H_

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

#include "constants.h"

#ifndef NDEBUG
#define DEFAULT_LOGFILE_VERBOSITY	3
#define LOG_PATH		"latest.log"
#else
#define DEFAULT_LOGFILE_VERBOSITY	0
#define LOG_PATH		"~/piscan.log"
#endif

#define DATABASE_PATH	"../data"
#define LOG_FOLDER		"logs"
#define LOGFILE_EXT		".log"

#define CONFIG_FILE		"config.json"
#define STATE_FILE		"state.json"
#define SYSTEMS_FILE	"systems.json"

namespace piscan {

struct GeneralConfig {
	int logfileVerbosity = DEFAULT_LOGFILE_VERBOSITY;
};

#define DEFAULT_TCP_PORT			1234
#define MAX_TCP_CONNECTIONS			5
#define DEFAULT_SPAWN_CLIENT		false
#define DEFAULT_PY_CLIENT_LOCATION	"./"
#define DEFAULT_PY_ENV_LOCATION	"python"

struct SocketServerConfig {
	int tcpPort = DEFAULT_TCP_PORT;
	int maxConnections = MAX_TCP_CONNECTIONS;
	bool spawnLocalClient = DEFAULT_SPAWN_CLIENT;
	std::string pythonClient = DEFAULT_PY_CLIENT_LOCATION;
	std::string pythonBinary = DEFAULT_PY_ENV_LOCATION;
};

#define TUNER_RETUNE_TIME	225000
#define DEMOD_BUFFER_TIME	10000

#define SQUELCH_DBM	0
#define SQUELCH_SNR	1
#define SQUELCH_PCT 2

#define DEFAULT_SQUELCH_MODE	(SQUELCH_DBM)

struct DemodConfig {
	long int retuneDelay = TUNER_RETUNE_TIME;
	long int demodDelay = DEMOD_BUFFER_TIME;
	int squelchType = DEFAULT_SQUELCH_MODE;
};

#define DEFAULT_SQUELCH	0
#define DEFAULT_GAIN	(AUTO_GAIN)

struct DemodState {
	float squelch = DEFAULT_SQUELCH;
	float gain = DEFAULT_GAIN;
};

#define	SCAN_STATE_SCAN		0
#define SCAN_STATE_HOLD		1
#define SCAN_STATE_MANUAL	2
#define DEFAULT_SCAN_STATE	(SCAN_STATE_SCAN)

struct ScannerState {
	int scanState = DEFAULT_SCAN_STATE;
	long long manualFreq = 0;
	std::string manualModualtion = "";
	std::vector<int> holdIndex = {};
	std::string holdKey = "";
};

class Configuration {
public:

	~Configuration();

	static Configuration& getConfig();

	void setWorkingDirectory(std::string path);
	std::string getWorkingDirectory();
	void loadConfig();
	void loadState();
	void saveConfig();
	void saveState();

	GeneralConfig& getGeneralConfig() { return _generalConfig; };
	SocketServerConfig& getSocketConfig() { return _socketConfig; };
	DemodConfig& getDemodConfig() { return _demodConfig; };

	DemodState& getDemodState() { return _demodState; };
	ScannerState& getScannerState() { return _scannerState; };

	std::string getLogDirectory();
	std::string getDatedLogPath();
	std::string getLatestLogPath();

private:
	static Configuration* _config;
	std::string _configPath;

	GeneralConfig _generalConfig;
	SocketServerConfig _socketConfig;
	DemodConfig _demodConfig;

	DemodState _demodState;
	ScannerState _scannerState;

	Configuration();

	boost::property_tree::ptree _ptConfig;
	boost::property_tree::ptree _ptState;
};

}
#endif /* SERVER_CONFIGURATION_H_ */
