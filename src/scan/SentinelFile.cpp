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
#include <iostream>

#include "scan_types.h"
#include "Entry.h"
#include "RadioSystem.h"
#include "SystemList.h"
#include "ListGenerator.h"
#include "loguru.hpp"
#include "sigproc_types.h"
#include "constants.h"

#define SENTINEL_FILE		"list.hpd"

#define SENTINEL_TRUE		"On"
#define SENTINEL_FALSE		"Off"
#define SENTINEL_NFM		"NFM"
#define SENTINEL_FM			"FM"
#define SENTINEL_AUTO		"AUTO"
#define SENTINEL_AM			"AM"
#define SENTINEL_CSQ		""
#define SENTINEL_TONE_PRE	"TONE="

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

namespace piscan {
namespace scan {

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

	std::string& s_tag = tokens[C_FREQ_TAG_POS];
	std::string& s_lockout = tokens[C_FREQ_LO_POS];
	std::string& s_freq = tokens[C_FREQ_FREQ_POS];
	std::string& s_mode = tokens[C_FREQ_MODE_POS];
	std::string& s_tone = tokens[C_FREQ_TONE_POS];
	std::string& s_delay = tokens[C_FREQ_DELAY_POS];

	LOG_F(4, "Entry: %s - Freq: %s", s_tag.c_str(), s_freq.c_str());

	long long freq = std::stoll(s_freq);
	bool lockout = !s_lockout.compare(SENTINEL_TRUE);
	int delayMS = std::stoi(s_delay) * 1000;

	EntryPtr newEntry;

	if(!s_mode.compare(SENTINEL_NFM) || !s_mode.compare(SENTINEL_FM) || !s_mode.compare(SENTINEL_AUTO)) {
		if(!s_tone.compare(SENTINEL_CSQ)){
			LOG_F(5, "Using CSQ");
			newEntry = std::make_shared<FMChannel>(freq, s_tag, lockout, delayMS);
		}
		else if (!s_tone.substr(0, 5).compare(SENTINEL_TONE_PRE)){
			std::string tone_coded = s_tone.substr(5);
			std::string tone = tone_coded.substr(1);
			LOG_F(5, "Found tone %s", tone_coded.c_str());
			if(tone_coded.at(0) == 'C'){
				LOG_F(5, "CTCSS tone %s", tone.c_str());
				newEntry = std::make_shared<PLChannel>(freq, tone, s_tag, lockout, delayMS);
			}
			else if(tone_coded.at(0) == 'D'){
				LOG_F(5, "DCS tone %s", tone.c_str());
				newEntry = std::make_shared<DCChannel>(freq, tone, s_tag, lockout, delayMS);
			}
			else{
				LOG_F(5, "Unknown code, default to CSQ");
				newEntry = std::make_shared<FMChannel>(freq, s_tag, lockout, delayMS);
			}
		}
		
	}
	else if(!s_mode.compare(SENTINEL_AM)){
		newEntry = std::make_shared<AMChannel>(freq, s_tag, lockout, delayMS);
	}

	if (newEntry)
	{
		newEntry->setSysIndex(_list->size() - 1);
		newEntry->setEntryIndex(_system->size());
		_system->addEntry(newEntry);
	}
}

}
} // namespace piscan::scan

namespace piscan {
namespace sigproc {

class DummyDemod : public DemodInterface {
public:
	virtual bool setFrequency(long long /*freq*/) { return false; };
	virtual bool setTunerFrequency(long long /*freq*/) { return false; };
	virtual float getSignalLevel() { return 0.0f; };
	virtual float getDecodedPL() { return 0.0f; };
	virtual unsigned int getDecodedDC() { return 0u;};
	virtual bool squelchThresholdMet() { return false; };
	virtual bool setModem(Modulation /*mode*/) { return false; };
	virtual void setSquelch(float /*level*/) {};
	virtual float getSNR() { return 0.0f; };
	virtual int getSignalStrength() { return 0; };
} demod;
}
}

//piscan::sigproc::DummyDemod demod;

piscan::sigproc::DemodInterface& piscan::app::getDemodInstance() {
	return piscan::sigproc::demod;
}

void usage() {
	std::cout << "Usage:\n\t";
	std::cout << "piscan_hpdconv -i [path to .hpd file] -o [path to PiScan data folder]\n";
	std::cout << "\tOptional flags:";
	std::cout << "\t\t-d\tverbose mode" << std::endl;

	std::exit(0);
}

int main(int argc, char** argv){
	piscan::config::Configuration& config = piscan::config::Configuration::getConfig();
	piscan::scan::SystemList list;
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
					config.setWorkingDirectory(std::string(optarg));
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

	piscan::scan::SentinelFile generator(hpdPath);

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
