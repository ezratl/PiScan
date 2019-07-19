// Copyright (c) Charles J. Cliffe
// SPDX-License-Identifier: GPL-2.0+

#include "ModemBPSK.h"

ModemBPSK::ModemBPSK() : ModemDigital()  {
    demodBPSK = modem_create(LIQUID_MODEM_BPSK);
}

ModemBase *ModemBPSK::factory() {
    return new ModemBPSK;
}

ModemBPSK::~ModemBPSK() {
    modem_destroy(demodBPSK);
}

std::string ModemBPSK::getName() {
    return "BPSK";
}

void ModemBPSK::demodulate(ModemKit *kit, ModemIQData *input, AudioThreadInput * /* audioOut */) {
    ModemKitDigital *dkit = (ModemKitDigital *)kit;
    digitalStart(dkit, demodBPSK, input);

    for (size_t i = 0, bufSize=input->data.size(); i < bufSize; i++) {
        modem_demodulate(demodBPSK, input->data[i], &demodOutputDataDigital[i]);
    }
    updateDemodulatorLock(demodBPSK, 0.005f);
    
    digitalFinish(dkit, demodBPSK);
}
