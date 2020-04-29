#pragma once

#include <vector>

#include "clientmessage.h"
#include "Configuration.h"

namespace piscan::app {
struct ManualEntryData {
public:
	ManualEntryData(ManualEntryData& copy) : freq(copy.freq), modulation(copy.modulation){};
	ManualEntryData(long long freq, std::string mode): freq(freq), modulation(mode){};

	long long	freq;
	std::string	modulation;
};

/* system functions */
bool stopSystem();
const SystemInfo getSystemInfo();
inline Configuration& getConfig() { return Configuration::getConfig(); };
void softAbort();

/* scanner functions */
void startScan();
void holdScan(std::vector<int> index = std::vector<int>());
void stopScanner();
void manualEntry(ManualEntryData* freq);
ScannerContext getScannerContext();

/* demod functions */
void setTunerGain(float gain);
void setDemodSquelch(float level);
DemodContext getDemodContext();
void squelchBreak(bool mute = true);
long long getTunerSampleRate();

/* server functions */
void scannerContextUpdate(ScannerContext ctx);
void demodContextUpdate(DemodContext ctx);
void signalLevelUpdate(int level);
}
