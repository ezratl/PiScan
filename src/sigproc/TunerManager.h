#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "Configuration.h"

class CubicSDR;
class SDRDeviceInfo;

namespace piscan {
namespace sigproc {
    struct ConfiguredTuner {
        piscan::config::TunerConfig config; // TODO may be changed to ref or shared_ptr
        SDRDeviceInfo* device;
    };

    class TunerManager {
    public:
        TunerManager(std::shared_ptr<CubicSDR> cubic);

        bool enumerateDevices();
        bool deviceAvailable();
        bool selectFirstAvailableDevice();
        /*std::vector<SDRDeviceInfo*> getKnownDevices();
        std::vector<SDRDeviceInfo*> getAllDevices();*/

        static long nearestSampleRate(long desired, std::vector<long>& supportedRates);

        bool autoSelectTuner();
        bool startSelectedTuner();
        bool stopSelectedTuner();

    private:
        std::shared_ptr<CubicSDR> _cubic;
        std::unordered_map<std::string, SDRDeviceInfo*> _devs;
        ConfiguredTuner _selectedTuner;
    };
}
}
