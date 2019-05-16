/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWEMULATOR_HWNESEMULATOR_H
#define HWEMULATOR_HWNESEMULATOR_H

#include <string>
#include <cstring>
#include "../infones/InfoNES.h"
#include "../infones/InfoNES_System.h"
#include "../infones/InfoNES_pAPU.h"

using namespace std;

class HwNESEmulator {
public:
    HwNESEmulator();

    virtual ~HwNESEmulator();

    int prepare(string rom);

    int load();

    int save();

private:
    int start();

private:
    string rom;
};


#endif //HWEMULATOR_HWNESEMULATOR_H
