#include <functional>

#include "PiScan.h"
#include "TunerManager.h"
#include "CubicSDR.h"
#include "loguru.hpp"

#define AUDIO_DRIVER "audio"

using TunerList = piscan::config::TunerList;

namespace piscan{
namespace sigproc {

/*
 * @param cubic pointer to CubicSDR instance
 */
TunerManager::TunerManager(std::shared_ptr<CubicSDR> cubic) : _cubic(cubic) {
}

/*
 * @return True if at least one SDR device was discovered and is available,
 * False if no SDRs are available or if an error occured
 */
bool TunerManager::enumerateDevices() {
	while(_cubic->areDevicesEnumerating()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

	std::vector<SDRDeviceInfo*>* devs = _cubic->getDevices();

	//CHECK_F(devs->size() > 0, "No SDR devices are available");
    if (devs->empty()) {
        LOG_F(WARNING, "No devices are available");
        return false;
    }

    // Place devices into map by serial number and filter out audio devices
    for (auto device : *devs) {
        if (device->getDriver() != AUDIO_DRIVER) {
            const std::string& name = device->getName();
            if (name.size()) {
                _devs[name] = device;
            }
        }
    }

	return true;
}

bool TunerManager::deviceAvailable() {
    return true;
}

bool TunerManager::selectFirstAvailableDevice() {
    //TODO this is temporary - will eventually be replaced by a config interface
    LOG_F(INFO, "Selecting first available device");
    SDRDeviceInfo* device = (*(_devs.begin())).second;
    _selectedTuner.device = device;
    _selectedTuner.config.driver = device->getDriver();
    _selectedTuner.config.descriptor = device->getName();
    return true;
}

/*std::vector<SDRDeviceInfo*> TunerManager::getKnownDevices() {

}

std::vector<SDRDeviceInfo*> TunerManager::getAllDevices() {

}*/

/*
 * @param desired The desired sample rate
 * @param supportedRates Vector containing all sample rates supported by a tuner
 * @return The supported sample rate closest to desired, or -1 if no rates are supported
 */
long TunerManager::nearestSampleRate(long desired, std::vector<long>& supportedRates) {
    long minDiff = LONG_MAX;
    long result = -1;
    long diff;

    for (long rate : supportedRates) {
        diff = std::abs(desired - rate);
        if (diff < minDiff) {
            result = rate;
            minDiff = diff;
        }
    }

    return result;
}

/* 
 * Allow the tuner to choose the tuner to use - by default the first available tuner found in config
 * @return False if a tuner could not be selected
 */
bool TunerManager::autoSelectTuner() {
    TunerList& tunerList = piscan::app::system::getConfig().getTunerList();

    if(_devs.empty()) {
        LOG_F(WARNING, "No SDR devices were found");
        return false;
    }

    if (tunerList.tuners.empty()) {
        // TODO temporary
        LOG_F(INFO, "No saved tuners found in config, defaulting to first available");
        selectFirstAvailableDevice();

        LOG_F(INFO, "New tuner will be saved to config");
        _selectedTuner.config.rank = tunerList.tuners.size();
        tunerList.tuners.insert(_selectedTuner.config);
    }
    else {
        bool foundTuner = false;
        LOG_F(1, "Searching for known tuners...");
        for (piscan::config::TunerConfig t : tunerList.tuners) {
            auto dev = _devs.find(t.descriptor);
            if (dev != _devs.end()) {
                if (!(*dev).second->isAvailable()) {
                    LOG_F(WARNING, "Found tuner %s but it is unavailable", t.descriptor.c_str());
                    continue;
                }

                LOG_F(1, "Found tuner %s with rank %i", t.descriptor.c_str(), t.rank);

                _selectedTuner.config = t;
                _selectedTuner.device = (*dev).second;

                foundTuner = true;
                break;
            }
        }

        // TODO temporary
        if (!foundTuner) {
            LOG_F(INFO, "No saved tuners were detected, defaulting to first available");
            selectFirstAvailableDevice();

            LOG_F(INFO, "New tuner will be saved to config");
            _selectedTuner.config.rank = tunerList.tuners.size();
            tunerList.tuners.insert(_selectedTuner.config);
        }
    }
    
    LOG_F(INFO, "Auto selecting sample rate");
	std::vector<long> srates(std::move(_selectedTuner.device->getSampleRates(SOAPY_SDR_RX, 0)));
	
    RAW_LOG_F(2, "Supported sample rates for tuner:");
    for (long rate : srates)
        RAW_LOG_F(2, "\t%li", rate);

    _selectedTuner.config.sampleRate = nearestSampleRate(_selectedTuner.config.sampleRate, srates); // TODO hardcoded sample rate temporary

	LOG_F(INFO, "Auto selected: %s", _selectedTuner.device->getName().c_str());

    return true;
}

/*
 * Start the tuner's operation
 * @return False if no tuner is selected or an error occured during init
 */
bool TunerManager::startSelectedTuner() {
    LOG_F(INFO, "Starting tuner %s", _selectedTuner.device->getName().c_str());
	_cubic->setDevice(_selectedTuner.device, 2000);
    LOG_F(INFO, "Setting tuner PPM correction to %i", _selectedTuner.config.ppmCorrection);
    _cubic->setPPM(_selectedTuner.config.ppmCorrection);
    LOG_F(INFO, "Setting device sample rate to %li", _selectedTuner.config.sampleRate);
	_cubic->setSampleRate(_selectedTuner.config.sampleRate);

    return true;
}

/*
 * Stop the tuner's operation
 * @return False if no tuner is selected, the tuner wasn't started, or an error occured during deinit
 */
bool TunerManager::stopSelectedTuner() {
    _cubic->stopDevice(false, 2000);
    return true;
}

}
}
