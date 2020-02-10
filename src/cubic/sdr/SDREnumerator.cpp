// Copyright (c) Charles J. Cliffe
// SPDX-License-Identifier: GPL-2.0+

#include "SDREnumerator.h"
#include "CubicSDRDefs.h"
#include <vector>
#include "CubicSDR.h"
#include <string>

#ifdef WIN32
#include <locale>
#endif

#include "loguru.hpp"
#include "threadname.h"

#define SDRENUM_THREAD_NAME "SDR Enumerator"


std::vector<std::string> SDREnumerator::factories;
std::vector<std::string> SDREnumerator::modules;
std::vector<std::string> SDREnumerator::remotes;
std::map< std::string, std::vector<SDRDeviceInfo *> > SDREnumerator::devs;
bool SDREnumerator::soapy_initialized = false;
bool SDREnumerator::has_remote = false;
std::vector<SDRManualDef> SDREnumerator::manuals;

SDREnumerator::SDREnumerator() : IOThread() {
  
}

SDREnumerator::~SDREnumerator() {

}

// Some utility from SoapySDR :)
static std::string trim(const std::string &s)
{
#if WIN32
	std::string out = s;
	locale loc("");
	while (not out.empty() and std::isspace(out[0], loc)) out = out.substr(1);
	while (not out.empty() and std::isspace(out[out.size() - 1], loc)) out = out.substr(0, out.size() - 1);
	return out;
#else
	std::string out = s;
    while (not out.empty() and std::isspace(out[0])) out = out.substr(1);
    while (not out.empty() and std::isspace(out[out.size()-1])) out = out.substr(0, out.size()-1);
    return out;
#endif
}

SoapySDR::Kwargs SDREnumerator::argsStrToKwargs(const std::string &args)
{
    SoapySDR::Kwargs kwargs;
    
    bool inKey = true;
    std::string key, val;
    for (size_t i = 0; i < args.size(); i++)
    {
        const char ch = args[i];
        if (inKey)
        {
            if (ch == '=') inKey = false;
            else if (ch == ',') inKey = true;
            else key += ch;
        }
        else
        {
            if (ch == ',') inKey = true;
            else val += ch;
        }
        if ((inKey and not val.empty()) or ((i+1) == args.size()))
        {
            key = trim(key);
            val = trim(val);
            if (not key.empty()) kwargs[key] = val;
            key = "";
            val = "";
        }
    }
    
    return kwargs;
}


std::vector<SDRDeviceInfo *> *SDREnumerator::enumerate_devices(std::string remoteAddr, bool noInit) {

    if (SDREnumerator::devs[remoteAddr].size()) {
        return &SDREnumerator::devs[remoteAddr];
    }
    
    if (noInit) {
        return NULL;
    }
    
    if (!soapy_initialized) {
        LOG_F(1, "SoapySDR init..");
        RAW_LOG_F(1, "\tAPI Version: v%s", SoapySDR::getAPIVersion().c_str());
        RAW_LOG_F(1, "\tABI Version: v%s", SoapySDR::getABIVersion().c_str());
        RAW_LOG_F(1, "\tInstall root: %s", SoapySDR::getRootPath().c_str());
        
        RAW_LOG_F(1, "\tLoading modules... ");
        
        std::string userModPath = wxGetApp().getModulePath();
        
        if (userModPath != "") {
            wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Loading SoapySDR modules from " + userModPath + "..");
            std::vector<std::string> localMods = SoapySDR::listModules(userModPath);
            for (std::string mod : localMods) {
                wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Initializing user specified SoapySDR module " + (mod) + "..");
                RAW_LOG_F(1, "Initializing user specified SoapySDR module %s", (mod).c_str());
                SoapySDR::loadModule(mod);
            }
        } else {
            #ifdef BUNDLE_SOAPY_MODS
			#ifdef BUNDLED_MODS_ONLY
			wxFileName exePath = wxFileName(wxStandardPaths::Get().GetExecutablePath());
			std::vector<std::string> localMods = SoapySDR::listModules(exePath.GetPath().ToStdString() + "/modules/");
			for (std::vector<std::string>::iterator mods_i = localMods.begin(); mods_i != localMods.end(); mods_i++) {
				wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Initializing bundled SoapySDR module " + (*mods_i) + "..");
				RAW_LOG_F(1, "Loading bundled SoapySDR module %s", (*mods_i).c_str());
				SoapySDR::loadModule(*mods_i);
			}
			#else
            bool localModPref = wxGetApp().getUseLocalMod();
            if (localModPref) {
                wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Loading SoapySDR modules..");
                RAW_LOG_F(1, "Checking local system SoapySDR modules..");
                SoapySDR::loadModules();
            }

            wxFileName exePath = wxFileName(wxStandardPaths::Get().GetExecutablePath());
            std::vector<std::string> localMods = SoapySDR::listModules(exePath.GetPath().ToStdString() + "/modules/");
            for (std::string mod : localMods) {
                wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Initializing bundled SoapySDR module " + (mod) + "..");
                RAW_LOG_F(1, "Loading bundled SoapySDR module %s", (mod).c_str());
                SoapySDR::loadModule(mod);
            }
        
            if (!localModPref) {
                wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Loading SoapySDR modules..");
                RAW_LOG_F(1, "Checking system SoapySDR modules.." );
                SoapySDR::loadModules();
            }
			#endif
            #else
            wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Loading SoapySDR modules..");
            SoapySDR::loadModules();
            #endif

        }
        
		SDREnumerator::factories.clear();
       
        
        LOG_F(1, "\tAvailable factories:");
        SoapySDR::FindFunctions factories = SoapySDR::Registry::listFindFunctions();
        for (SoapySDR::FindFunctions::const_iterator it = factories.begin(); it != factories.end(); ++it) {
            /*if (it != factories.begin()) {
                std::cout << ", ";
            }*/
            //std::cout << it->first;
            RAW_LOG_F(1, "\t%s", it->first.c_str());
            
            if (it->first == "remote") {
                has_remote = true;
            }
            SDREnumerator::factories.push_back(it->first);
        }
        if (factories.empty()) {
            //std::cout << "No factories found!" << std::endl;
            LOG_F(ERROR, "No factories found!");
        }
        if ((factories.size() == 1) && factories.find("null") != factories.end()) {
            //std::cout << "Just 'null' factory found." << std::endl;
            LOG_F(ERROR, "Just 'null' factory found");
            wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_FAILED, std::string("No modules available."));
        }
        //std::cout << std::endl;
        soapy_initialized = true;
    }
    
    modules = SoapySDR::listModules();

    std::vector<SoapySDR::Kwargs> results;
    SoapySDR::Kwargs enumArgs;
    bool isRemote = false;
    
    if (remoteAddr.length()) {
        //std::cout << "Enumerating remote address: " << remoteAddr << std::endl;
        LOG_F(1, "Enumerating remote address");
        enumArgs["driver"] = "remote";
        enumArgs["remote"] = remoteAddr;
        isRemote = true;
        
        results = SoapySDR::Device::enumerate(enumArgs);
    } else {
        results = SoapySDR::Device::enumerate();
    }
    
    size_t manualsIdx = results.size();
    std::vector<std::string> manualParams;
    std::vector<bool> manualResult;
    
    if (manuals.size()) {
        for (std::vector<SDRManualDef>::const_iterator m_i = manuals.begin(); m_i != manuals.end(); m_i++) {
            std::vector<SoapySDR::Kwargs> manual_result;

            std::string strDevArgs = "driver="+m_i->factory+","+m_i->params;
            
            manualParams.push_back(m_i->params);
            
            wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, std::string("Enumerating manual device '") + strDevArgs + "'..");

            manual_result = SoapySDR::Device::enumerate(strDevArgs);
            
            if (manual_result.size()) {
                for (std::vector<SoapySDR::Kwargs>::const_iterator i = manual_result.begin(); i != manual_result.end(); i++) {
                    results.push_back(*i);
                    manualResult.push_back(true);
                }
            } else {
                SoapySDR::Kwargs failedEnum;
                failedEnum = argsStrToKwargs(strDevArgs);
                failedEnum["label"] = "Not Found ("+m_i->factory+")";
                results.push_back(failedEnum);
                manualResult.push_back(false);
            }
        }
    }
    
    if (isRemote) {
        wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, std::string("Opening remote server ") + remoteAddr + "..");
    }
    for (size_t i = 0; i < results.size(); i++) {
        SDRDeviceInfo *dev = new SDRDeviceInfo();
        
        SoapySDR::Kwargs deviceArgs = results[i];

        for (SoapySDR::Kwargs::const_iterator it = deviceArgs.begin(); it != deviceArgs.end(); ++it) {
            //std::cout << "  " << it->first << " = " << it->second << std::endl;
            RAW_LOG_F(1, "\t%s = %s", it->first.c_str(), it->second.c_str());
            if (it->first == "driver") {
                dev->setDriver(it->second);
            } else if (it->first == "label" || it->first == "device") {
                dev->setName(it->second);
			}
        }
        
        
        if (deviceArgs.count("remote")) {
            isRemote = true;
        } else {
            isRemote = false;
        }
        
        dev->setRemote(isRemote);
        dev->setManual(i>=manualsIdx);
        if (i>=manualsIdx) {
            dev->setManualParams(manualParams[i-manualsIdx]);
        }
        
        //std::cout << "Make device " << i << std::endl;
        LOG_F(1, "Make device");
        if (i<manualsIdx || manualResult[i-manualsIdx]) try {
            SoapySDR::Device *device = SoapySDR::Device::make(deviceArgs);
            SoapySDR::Kwargs info = device->getHardwareInfo();
            for (SoapySDR::Kwargs::const_iterator it = info.begin(); it != info.end(); ++it) {
                //std::cout << "  " << it->first << "=" << it->second << std::endl;
                RAW_LOG_F(1, "\t%s = %s", it->first.c_str(), it->second.c_str());
                if (it->first == "hardware") {
                    dev->setHardware(it->second);
                }
            }
            
            if (isRemote) {
                wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Querying remote " + remoteAddr + " device #" + std::to_string(i) + ": " + dev-> getName());
            } else {
                wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, std::string("Querying device #") + std::to_string(i) + ": " + dev->getName());
            }

            SoapySDR::ArgInfoList settingsInfo = device->getSettingInfo();

            DeviceConfig *cfg = wxGetApp().getConfig()->getDevice(dev->getDeviceId());

            ConfigSettings devSettings = cfg->getSettings();
            if (devSettings.size()) {
				// Load the saved device settings to deviceArgs, and back to settingsInfo.
                for (ConfigSettings::const_iterator set_i = devSettings.begin(); set_i != devSettings.end(); set_i++) {
                    deviceArgs[set_i->first] = set_i->second;
                }
                for (size_t j = 0; j < settingsInfo.size(); j++) {
                    if (deviceArgs.find(settingsInfo[j].key) != deviceArgs.end()) {
                        settingsInfo[j].value = deviceArgs[settingsInfo[j].key];
                    }
                }
            }
            
            dev->setDeviceArgs(deviceArgs);
            
            SoapySDR::Device::unmake(device);
            dev->setAvailable(true);
        } catch (const std::exception &ex) {
            //std::cerr << "Error making device: " << ex.what() << std::endl;
            LOG_F(ERROR, "Error making device: %s", ex.what());
            wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, std::string("Error querying device #") + std::to_string(i));
            dev->setAvailable(false);
        } else {
            dev->setAvailable(false);
        }
        //std::cout << std::endl;

        SDREnumerator::devs[remoteAddr].push_back(dev);
    }
    if (SDREnumerator::devs[remoteAddr].empty()) {
        wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, std::string("No devices found!"));
    }
    //std::cout << std::endl;

    return &SDREnumerator::devs[remoteAddr];
}


void SDREnumerator::run() {

    //std::cout << "SDR enumerator starting." << std::endl;
    setThreadName(SDRENUM_THREAD_NAME);
    LOG_F(1, "SDR enumerator starting");
    

    wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Scanning local devices, please wait..");
    SDREnumerator::enumerate_devices("");

    if (remotes.size()) {
      
        for (std::string remote : remotes) {
            wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_MESSAGE, "Scanning devices at " + (remote) + ", please wait..");
            SDREnumerator::enumerate_devices(remote);
        }
    }
    
    //std::cout << "Reporting enumeration complete." << std::endl;
    LOG_F(1, "Reporting enumeration complete");
    wxGetApp().sdrEnumThreadNotify(SDREnumerator::SDR_ENUM_DEVICES_READY, "Finished scanning devices.");
    //std::cout << "SDR enumerator done." << std::endl;
    LOG_F(1, "SDR Enumerator done");

}



void SDREnumerator::addRemote(std::string remoteAddr) {
    std::vector<std::string>::iterator remote_i = std::find(remotes.begin(), remotes.end(), remoteAddr);
    
    if (remote_i != remotes.end()) {
        return;
    } else {
        remotes.push_back(remoteAddr);
    }
}

void SDREnumerator::removeRemote(std::string remoteAddr) {
    std::vector<std::string>::iterator remote_i = std::find(remotes.begin(), remotes.end(), remoteAddr);
    
    if (remote_i != remotes.end()) {
        if (devs.find(*remote_i) != devs.end()) {
            while (devs[*remote_i].size()) {
                SDRDeviceInfo *devRemove = devs[*remote_i].back();
                devs[*remote_i].pop_back();
                delete devRemove;
            }
        }
        remotes.erase(remote_i);
    } else {
        return;
    }
}

std::vector<std::string> &SDREnumerator::getRemotes() {
    return remotes;
}

void SDREnumerator::addManual(std::string factory, std::string params) {
    SDRManualDef def;
    def.factory = factory;
    def.params = params;
    manuals.push_back(def);
}

void SDREnumerator::removeManual(std::string factory, std::string params) {
    for (std::vector<SDRManualDef>::iterator i = manuals.begin(); i != manuals.end(); i++) {
        if (i->factory == factory && i->params == params) {
            manuals.erase(i);
            for (std::vector<SDRDeviceInfo *>::iterator subdevs_i = devs[""].begin(); subdevs_i != devs[""].end(); subdevs_i++) {
                if ((*subdevs_i)->isManual() && (*subdevs_i)->getDriver() == factory && (*subdevs_i)->getManualParams() == params) {
                    devs[""].erase(subdevs_i);
                    break;
                }
            }
            break;
        }
    }
}

std::vector<SDRManualDef> &SDREnumerator::getManuals() {
    return SDREnumerator::manuals;
}

void SDREnumerator::setManuals(std::vector<SDRManualDef> manuals) {
    SDREnumerator::manuals = manuals;
}

bool SDREnumerator::hasRemoteModule() {
    return SDREnumerator::has_remote;
}

void SDREnumerator::reset() {
    soapy_initialized = false;
	factories.clear(); 
    modules.clear();

    for (std::map< std::string, std::vector<SDRDeviceInfo *> >::iterator di = devs.begin(); di != devs.end(); di++) {

        for (std::vector<SDRDeviceInfo *>::iterator i = di->second.begin(); i != di->second.end(); i++) {
            (*i)->setSoapyDevice(nullptr);
        }
        
    }
    devs.clear();
}

std::vector<std::string> &SDREnumerator::getFactories() {
    return SDREnumerator::factories;
}
