// Copyright (c) Charles J. Cliffe
// SPDX-License-Identifier: GPL-2.0+

#include "ModemQAM.h"

ModemQAM::ModemQAM() : ModemDigital()  {
    demodQAM4 = modem_create(LIQUID_MODEM_QAM4);
    demodQAM8 = modem_create(LIQUID_MODEM_QAM8);
    demodQAM16 = modem_create(LIQUID_MODEM_QAM16);
    demodQAM32 = modem_create(LIQUID_MODEM_QAM32);
    demodQAM64 = modem_create(LIQUID_MODEM_QAM64);
    demodQAM128 = modem_create(LIQUID_MODEM_QAM128);
    demodQAM256 = modem_create(LIQUID_MODEM_QAM256);
    demodQAM = demodQAM4;
    cons = 4;
}

ModemBase *ModemQAM::factory() {
    return new ModemQAM;
}

std::string ModemQAM::getName() {
    return "QAM";
}

ModemQAM::~ModemQAM() {
    modem_destroy(demodQAM4);
    modem_destroy(demodQAM8);
    modem_destroy(demodQAM16);
    modem_destroy(demodQAM32);
    modem_destroy(demodQAM64);
    modem_destroy(demodQAM128);
    modem_destroy(demodQAM256);
}

ModemArgInfoList ModemQAM::getSettings() {
    ModemArgInfoList args;
    
    ModemArgInfo consArg;
    consArg.key = "cons";
    consArg.name = "Constellation";
    consArg.description = "Modem Constellation Pattern";
    consArg.value = std::to_string(cons);
    consArg.type = ModemArgInfo::STRING;
    std::vector<std::string> consOpts;
    consOpts.push_back("4");
    consOpts.push_back("8");
    consOpts.push_back("16");
    consOpts.push_back("32");
    consOpts.push_back("64");
    consOpts.push_back("128");
    consOpts.push_back("256");
    consArg.options = consOpts;
    args.push_back(consArg);
    
    return args;
}

void ModemQAM::writeSetting(std::string setting, std::string value) {
    if (setting == "cons") {
        int newCons = std::stoi(value);
        updateDemodulatorCons(newCons);
    }
}

std::string ModemQAM::readSetting(std::string setting) {
    if (setting == "cons") {
        return std::to_string(cons);
    }
    return "";
}

void ModemQAM::updateDemodulatorCons(int cons) {
    this->cons = cons;
    switch (cons) {
        case 4:
            demodQAM = demodQAM4;
            break;
        case 8:
            demodQAM = demodQAM8;
            break;
        case 16:
            demodQAM = demodQAM16;
            break;
        case 32:
            demodQAM = demodQAM32;
            break;
        case 64:
            demodQAM = demodQAM64;
            break;
        case 128:
            demodQAM = demodQAM128;
            break;
        case 256:
            demodQAM = demodQAM256;
            break;
    }
}

void ModemQAM::demodulate(ModemKit *kit, ModemIQData *input, AudioThreadInput * /* audioOut */) {
    ModemKitDigital *dkit = (ModemKitDigital *)kit;
    digitalStart(dkit, demodQAM, input);
   
    for (size_t i = 0, bufSize = input->data.size(); i < bufSize; i++) {
        modem_demodulate(demodQAM, input->data[i], &demodOutputDataDigital[i]);
    }
    updateDemodulatorLock(demodQAM, 0.5f);
    
    digitalFinish(dkit, demodQAM);
}
