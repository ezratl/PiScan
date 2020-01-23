/*
 * SentinelFile.cpp
 *
 *  Created on: Mar 14, 2019
 *      Author: ezra
 */

#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "constants.h"
#include "ListGenerator.h"
#include "loguru.hpp"

#define SENTINEL_FILE		"list.hpd"

#define SENTINEL_TRUE		"On"
#define SENTINEL_FALSE		"Off"
#define SENTINEL_NFM		"NFM"
#define SENTINEL_FM			"FM"
#define SENTINEL_AUTO		"AUTO"

#define C_GROUP				"C-Group"
#define C_GROUP_TAG_POS		1
#define C_GROUP_LO_POS		2

#define C_FREQ				"C-Freq"
#define C_FREQ_TAG_POS		1
#define C_FREQ_LO_POS		2
#define C_FREQ_FREQ_POS		3
#define C_FREQ_MODE_POS		4
#define C_FREQ_TONE_POS		5
#define C_FREQ_DELAY_POS	8

using namespace piscan;

bool SentinelFile::generateSystemList(SystemList& list) {
	_list = &list;

	std::ifstream file;
	try{
		//std::string path = std::string(DATABASE_PATH) + "/" + SENTINEL_FILE;
		LOG_F(2, "Opening list file: %s", _path.c_str());
		file.open(_path, std::ios::in);
	}
	catch(std::exception& e){
		LOG_F(WARNING, "Unable to open .hpd file: %s", e.what());
		return false;
	}

	if(!file.is_open()){
		LOG_F(WARNING, "Unable to open .hpd file");
		return false;
	}

	std::string line;
	std::string intermediate;
	std::stringstream sstream;
	std::vector<std::string> tokens;
	while(std::getline(file, line, '\n')){
		sstream = std::stringstream(line);

		while(std::getline(sstream, intermediate, '\t')){
			if(intermediate.size() > 0)
				tokens.push_back(intermediate);
		}

		if(tokens.size() == 0)
			continue;
		std::string& type = tokens[0];
		if(!type.compare(C_GROUP))
			_newAnalogSys(tokens);
		else if(!type.compare(C_FREQ) && _system != nullptr)
			_newAnalogEntry(tokens);


		tokens.clear();
	}

	file.close();
	return true;
}

void SentinelFile::_newAnalogSys(std::vector<std::string>& tokens){
	LOG_F(4, "New AnalogSystem: %s", tokens[C_GROUP_TAG_POS].c_str());
	_system = std::make_shared<AnalogSystem>(tokens[C_GROUP_TAG_POS], (!tokens[C_GROUP_LO_POS].compare(SENTINEL_TRUE)));
	_list->addSystem(_system);
}

void SentinelFile::_newAnalogEntry(std::vector<std::string>& tokens){
	if(_system == nullptr)
		return;

	std::string& tag = tokens[C_FREQ_TAG_POS];
	std::string& lockout = tokens[C_FREQ_LO_POS];
	std::string& freq = tokens[C_FREQ_FREQ_POS];
	std::string& mode = tokens[C_FREQ_MODE_POS];
	//std::string& tone = tokens[C_FREQ_TONE_POS];
	std::string& delay = tokens[C_FREQ_DELAY_POS];

	LOG_F(4, "Entry: %s - Freq: %s", tag.c_str(), freq.c_str());

	if(!mode.compare(SENTINEL_NFM) || !mode.compare(SENTINEL_FM) || !mode.compare(SENTINEL_AUTO)) {
		auto entry = std::make_shared<FMChannel>(std::stoul(freq), tag,
				(!lockout.compare(SENTINEL_TRUE)), (delay.compare("0")));

		entry->setSysIndex(_list->size() - 1);
		entry->setEntryIndex(_system->size());

		_system->addEntry(entry);
	}
}

void usage() {
	std::cout << "Usage:\n\t";
	std::cout << "piscan_hpdconv -i [path to .hpd file] -o [path to PiScan data folder]\n";
	std::cout << "\tOptional flags:";
	std::cout << "\t\t-d\tverbose mode" << std::endl;

	std::exit(0);
}

int main(int argc, char** argv){
	Configuration& config = Configuration::getConfig();
	SystemList list;
	std::string hpdPath;
	bool verbose = false;

	if(argc < 5)
		usage();

	int c;
	while((c = getopt(argc,argv,"i:o:d")) != -1){
		switch(c){
			case 'i':
				if(optarg)
					hpdPath = std::string(optarg);
				else
					usage();
				break;
			case 'o':
				if(optarg)
					config.setWorkingPath(std::string(optarg));
				else
					usage();
				break;
			case 'd':
				verbose = true;
				break;
		}
	}

	loguru::init(argc, argv);
	loguru::g_preamble_file = false;

	if(verbose)
		loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
	else
		loguru::g_stderr_verbosity = loguru::Verbosity_INFO;

	SentinelFile generator(hpdPath);

	if(!generator.generateSystemList(list)){
		LOG_F(ERROR, "File parsing failed");
		std::exit(1);
	}

	if(!list.writeToFile()){
		LOG_F(ERROR, "Writing data file failed");
		std::exit(1);
	}

	LOG_F(INFO, "Success");

	return 0;
}
