#pragma once

#include <vector>
#include <tuple>

#include "messages/context.h"
#include "Configuration.h"

class AudioThread; //forward declaration
namespace piscan {
namespace sigproc {
class DemodInterface;
}
}

namespace piscan {
namespace app {

enum ReturnStatus {
	SUCCESS,
	INVALID,
	NOT_IMPLEMENTED,
};
typedef std::tuple<ReturnStatus, void*> BasicReturnTuple;

struct ManualEntryData {
public:
	ManualEntryData(ManualEntryData& copy) : freq(copy.freq), modulation(copy.modulation){};
	ManualEntryData(long long freq, std::string mode): freq(freq), modulation(mode){};

	long long	freq;
	std::string	modulation;
};

/* system functions */
namespace system {
bool stopSystem();
const piscan::server::context::SystemInfo getSystemInfo();
inline piscan::config::Configuration& getConfig() { return piscan::config::Configuration::getConfig(); };
inline piscan::config::State& getState() { return piscan::config::State::getState(); };
void softAbort();
}

/* scanner functions */
namespace scanner {
void startScan();
void holdScan(std::vector<int> index = std::vector<int>());
void stopScanner();
void manualEntry(ManualEntryData* freq);
piscan::server::context::ScannerContext getScannerContext();
}

/* demod functions */
namespace demod {
piscan::sigproc::DemodInterface& getDemodInstance();
void setTunerGain(float gain);
void setDemodSquelch(float level);
piscan::server::context::DemodContext getDemodContext();
void squelchBreak(bool mute = true);
long long getTunerSampleRate();
//void setTunerPPM(int ppm);
}

/* server functions */
namespace server {
/*void scannerContextUpdate(piscan::server::context::ScannerContext ctx);
void demodContextUpdate(piscan::server::context::DemodContext ctx);
void signalLevelUpdate(int level);*/
}

/* audio related */
namespace audio {
AudioThread* getAudioController();
}

/* database */
namespace data {
/*
	Retrieve the entire System tree
*/	
BasicReturnTuple getScanList(); //TODO

/*
	Retrieve list of Systems and their indices, tags, and types
*/
BasicReturnTuple getSystemList();

/*
	Retrieve tree of System and its Entries at index
*/
BasicReturnTuple getSystemByIndex(size_t sys_index); //TODO

/*
	Retrieve list of Entries within indexed System and their indices and descriptors
*/
BasicReturnTuple getEntryList(size_t sys_index);

/*
	Retrieve Entry at index
*/
BasicReturnTuple getEntryByIndex(size_t sys_index, size_t entry_index); //TODO

namespace system {
	/* 
		Create a new Radio System
	*/
	BasicReturnTuple create(/*TODO data*/);

	/* 
		Replace the Radio System header at index. Entries will be retained unless the system type is changed.
	*/
	BasicReturnTuple replace(size_t sys_index /*, TODO new*/);

	/*
		Remove the Radio System and its Entries at index. Indices of succeeding Systems will be updated upon success
	*/
	BasicReturnTuple remove(size_t sys_index);

	/*
		Set lockout status of System at index.
		- '0' for unlocked
		- '-1' for persistent lock
		- '>1' lock for duration in seconds
	*/
	BasicReturnTuple setLockout(size_t sys_index, int duration_seconds);

	/*
		Move Radio System from original index to new index. All other indices are updated upon success
	*/
	BasicReturnTuple setIndex(size_t original_sys_index, size_t new_sys_index);

	namespace entry {
		/* 
			Create a new Entry within the indexed System
		*/
		BasicReturnTuple create(size_t sys_index /*,TODO data*/);

		/* 
			Replace the Entry at index
		*/
		BasicReturnTuple replace(size_t sys_index, size_t entry_index /*, TODO new*/);

		/* 
			Remove the Entry at index. Succeeding indices within the System are updated upon success
		*/
		BasicReturnTuple remove(size_t sys_index, size_t entry_index);

		/*
			Set lockout status of Entry at index.
			- '0' for unlocked
			- '-1' for persistent lock
			- '>1' lock for duration in seconds
		*/
		BasicReturnTuple setLockout(size_t sys_index, size_t entry_index, int duration_seconds);

		/*
			Move Entry within System from original index to new index. All other indices are updated upon success
		*/
		BasicReturnTuple setIndex(size_t sys_index, size_t original_entry_index, size_t new_entry_index);
	}
}
}

namespace configuration {
	/*
	Retrieve the full system configuration
	*/
	BasicReturnTuple getFullConfig();

	/*
	Set the full system configuration. Requires restart
	*/
	BasicReturnTuple setConfig(/*TODO*/);

	/*
	Retrieve general configuration
	*/
	BasicReturnTuple getGeneralConfig();

	/*
	Set the general configuration
	*/
	BasicReturnTuple setGeneralConfig(/*TODO*/);

	/*
	Retrieve configuration for demodulators
	*/
	BasicReturnTuple getDemodConfig();

	/*
	Set the configuration for demodulators. Restart bit required
	*/
	BasicReturnTuple setDemodConfig(/*TODO*/);

	/*
	Retrieve configuration for RTSP server
	*/
	BasicReturnTuple getAudioServerConfig();

	/*
	Set the configuration for RTSP server. Requires restart
	*/
	BasicReturnTuple setAudioServerConfig(/*TODO*/);

	/*
	Retrieve a list of configured tuners
	*/
	BasicReturnTuple getTunerList();
}

} // namespace app
} // namespace piscan
