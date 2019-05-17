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
    InfoNES_Attach(this);
}

HwNESEmulator::~HwNESEmulator() {

}

int HwNESEmulator::prepare(string rom, ANativeWindow *win, int width, int height) {
    this->rom = rom;
    this->win = win;
    this->width = width;
    this->height = height;
    ANativeWindow_setBuffersGeometry(this->win, NES_DISP_WIDTH,
                                     NES_DISP_HEIGHT,
                                     WINDOW_FORMAT_RGBA_8888);
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
    simpleLock.lock();
    ANativeWindow_release(this->win);
    this->win = nullptr;
    simpleLock.unlock();
    return 0;
}

int HwNESEmulator::load() {
    return 0;
}

int HwNESEmulator::save() {
    return 0;
}

int HwNESEmulator::draw(uint8_t *rgba, size_t size) {
    simpleLock.lock();
    if (this->win && rgba) {
        ANativeWindow_Buffer nwBuffer;
        if (0 == ANativeWindow_lock(this->win, &nwBuffer, NULL)) {
//            memset(rgba, 128, size);
//            Logcat::i("HWEMULATOR", "draw %d, %d, %d, %d", rgba[size / 2], rgba[size / 2 + 1],
//                      rgba[size / 2 + 2], rgba[size / 2 + 3]);
            memcpy(nwBuffer.bits, rgba, size);
            if (0 == ANativeWindow_unlockAndPost(this->win)) {
                simpleLock.unlock();
                return 0;
            }
        }
    }
    simpleLock.unlock();
    Logcat::e("HWEMULATOR", "Native window draw error!");
    return 0;
}
