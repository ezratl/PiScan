#pragma once

#include <vector>

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
}

/* server functions */
namespace server {
void scannerContextUpdate(piscan::server::context::ScannerContext ctx);
void demodContextUpdate(piscan::server::context::DemodContext ctx);
void signalLevelUpdate(int level);
}

/* audio related */
namespace audio {
AudioThread* getAudioController();
}

} // namespace app
} // namespace piscan
