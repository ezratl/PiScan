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

void SentinelFile::generateSystemList(SystemList& list) {
	_list = &list;

	std::ifstream file;
	try{
		std::string path = std::string(DATABASE_PATH) + "/" + SENTINEL_FILE;
		LOG_F(2, "Opening list file: %s", path.c_str());
		file.open(path, std::ios::in);
	}
	catch(std::exception& e){
		LOG_F(WARNING, "Unable to open Systems file: %s", e.what());
		return;
	}

	if(!file.is_open()){
		LOG_F(WARNING, "Unable to open Systems file");
		return;
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
		else if(!type.compare(C_FREQ))
			_newAnalogEntry(tokens);


		tokens.clear();
	}

	file.close();
}

void SentinelFile::_newAnalogSys(std::vector<std::string>& tokens){
	LOG_F(4, "New AnalogSystem: %s", tokens[C_GROUP_TAG_POS].c_str());
	_system = new AnalogSystem(tokens[C_GROUP_TAG_POS], (!tokens[C_GROUP_LO_POS].compare(SENTINEL_TRUE)));
	_list->addSystem(_system);
}

void SentinelFile::_newAnalogEntry(std::vector<std::string>& tokens){
	if(_system == nullptr)
		return;

	std::string& tag = tokens[C_FREQ_TAG_POS];
	std::string& lockout = tokens[C_FREQ_LO_POS];
	std::string& freq = tokens[C_FREQ_FREQ_POS];
	std::string& mode = tokens[C_FREQ_MODE_POS];
	std::string& tone = tokens[C_FREQ_TONE_POS];
	std::string& delay = tokens[C_FREQ_DELAY_POS];

	LOG_F(4, "Entry: %s - Freq: %s", tag.c_str(), freq.c_str());

	if(!mode.compare(SENTINEL_NFM) || !mode.compare(SENTINEL_FM) || !mode.compare(SENTINEL_AUTO)) {
		Entry* entry = new FMChannel(std::stoul(freq), tag,
				(!lockout.compare(SENTINEL_TRUE)), (delay.compare("0")));
		_system->addEntry(entry);
	}
}
