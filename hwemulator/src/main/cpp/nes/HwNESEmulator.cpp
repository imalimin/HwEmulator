/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "include/HwNESEmulator.h"

HwNESEmulator::HwNESEmulator() {

}

HwNESEmulator::~HwNESEmulator() {

}

int HwNESEmulator::prepare(string rom) {
    this->rom = rom;
    return 0;
}

int HwNESEmulator::start() {
    run_emulator(const_cast<char *>(rom.c_str()));
    return 0;
}

int HwNESEmulator::load() {
    return 0;
}

int HwNESEmulator::save() {
    return 0;
}
