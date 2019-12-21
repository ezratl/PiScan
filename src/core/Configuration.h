/*
 * Configuration.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#ifndef SERVER_CONFIGURATION_H_
#define SERVER_CONFIGURATION_H_

#include <string>

#include "constants.h"

#ifndef NDEBUG
#define DEFAULT_LOGFILE_VERBOSITY	3
#define LOG_PATH		"log.txt"
#else
#define DEFAULT_LOGFILE_VERBOSITY	0
#define LOG_PATH		"~/piscan.log"
#endif

#define DATABASE_PATH	"../data"
#define LOG_FOLDER		"logs"
#define LOGFILE_EXT		".txt"

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

#define DEFAULT_SQUELCH	0
#define DEFAULT_GAIN	(AUTO_GAIN)

struct DemodState {
	float squelch = DEFAULT_SQUELCH;
	float gain = DEFAULT_GAIN;
};

class Configuration {
public:

	~Configuration();

	static Configuration& getConfig();

	void setWorkingPath(std::string path);
	std::string getWorkingPath();
	void loadConfig();
	void loadState();
	void saveConfig();
	void saveState();

	GeneralConfig& getGeneralConfig() { return _generalConfig; };
	SocketServerConfig& getSocketConfig() { return _socketConfig; };
	DemodState& getDemodState() { return _demodState; };

	std::string getLogfilePath();

private:
	static Configuration* _config;
	std::string _configPath;

	GeneralConfig _generalConfig;
	SocketServerConfig _socketConfig;
	DemodState _demodState;

	Configuration();
};

}
#endif /* SERVER_CONFIGURATION_H_ */
