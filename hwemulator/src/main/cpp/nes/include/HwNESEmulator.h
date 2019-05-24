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
#include <android/native_window.h>
#include "HwGamePadEvent.h"
#include "../../common/include/SimpleLock.h"

using namespace std;

class HwNESEmulator {
public:
    HwNESEmulator();

    virtual ~HwNESEmulator();

    int prepare(string rom);

    int attachWindow(ANativeWindow *win, int width, int height);

    int start();

    int stop();

    int load();

    int save();

    int draw(uint8_t *rgba, size_t size);

    void postEvent(HwGamePadEvent *event);

private:
    void handleEvent(int event, int action);

    void drawFrame(uint8_t *rgba, size_t size);

private:
    string rom;
    ANativeWindow *win = nullptr;
    int width = 0;
    int height = 0;
    int focusHeight = 0;//1:focus height, -1:focus width, 0:height same as width
    uint8_t *frameBuf = nullptr;
    SimpleLock simpleLock;

    unsigned long dwKeyPad1 = 0;
    unsigned long dwKeyPad2 = 0;
    unsigned long dwKeySystem = 0;
};


#endif //HWEMULATOR_HWNESEMULATOR_H
