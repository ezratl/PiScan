#pragma once

#include <vector>

#include "clientmessage.h"

namespace piscan::app {
/* system functions */
bool stopSystem();
const SystemInfo getSystemInfo();

/* scanner functions */
void startScan();
void holdScan(std::vector<int> index = std::vector<int>());
void stopScanner();
void manualEntry(uint32_t* freq);
ScannerContext getScannerContext();

/* demod functions */
void setTunerGain(float gain);
void setDemodSquelch(float level);
DemodContext getDemodContext();
void audioMute(bool mute = true);
long long getTunerSampleRate();

/* server functions */
void scannerContextUpdate(ScannerContext ctx);
void demodContextUpdate(DemodContext ctx);
}
