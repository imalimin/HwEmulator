/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "include/HwNESEmulator.h"
#include "Logcat.h"
#include "./infones/InfoNES.h"
#include "./infones/InfoNES_System.h"
#include "./infones/InfoNES_pAPU.h"

HwNESEmulator::HwNESEmulator() {

}

HwNESEmulator::~HwNESEmulator() {

}

int HwNESEmulator::prepare(string rom) {
    this->rom = rom;
    return 0;
}

int HwNESEmulator::start() {
    /* If a rom name specified, start it */
    if (0 != InfoNES_Load(rom.c_str())) {
        Logcat::e("HWEMULATOR", "Load rom failed!");
        return -1;
    }
    /* MainLoop */
    InfoNES_Main();
    return 0;
}

int HwNESEmulator::stop() {
    InfoNES_Stop();
    return 0;
}

int HwNESEmulator::load() {
    return 0;
}

int HwNESEmulator::save() {
    return 0;
}
