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

int HwNESEmulator::prepare(string rom) {
    this->rom = rom;
    return 0;
}

int HwNESEmulator::attachWindow(ANativeWindow *win, int width, int height) {
    simpleLock.lock();
    if (this->win) {
        ANativeWindow_release(this->win);
        this->win = nullptr;
        if (frameBuf) {
            delete[]frameBuf;
            frameBuf = nullptr;
        }
    }
    this->win = win;
    this->width = NES_DISP_WIDTH;
    this->height = NES_DISP_HEIGHT;
    float winScale = width / (float) height;
    float frameScale = NES_DISP_WIDTH / (float) NES_DISP_HEIGHT;
    if (winScale > frameScale) {
        this->focusHeight = 1;
        this->height = NES_DISP_HEIGHT;
        this->width = static_cast<int>(NES_DISP_HEIGHT * winScale);
    } else if (winScale < frameScale) {
        this->focusHeight = -1;
        this->width = NES_DISP_WIDTH;
        this->height = static_cast<int>(NES_DISP_WIDTH / winScale);
    }
    if (0 != this->width % 2) {
        this->width += 1;
    }
    if (0 != this->height % 2) {
        this->height += 1;
    }
    Logcat::i("HWEMULATOR", "Window size %dx%d(%dx%d) -> %dx%d", ANativeWindow_getWidth(win),
              ANativeWindow_getHeight(win), width, height, this->width, this->height);
    frameBuf = new uint8_t[this->width * this->height * 4];
    memset(frameBuf, 0, this->width * this->height * 4);
    ANativeWindow_setBuffersGeometry(this->win, this->width, this->height, WINDOW_FORMAT_RGBA_8888);
    simpleLock.unlock();
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
    if (frameBuf) {
        delete[]frameBuf;
        frameBuf = nullptr;
    }
    simpleLock.unlock();
    return 0;
}

int HwNESEmulator::load() {
    return 0;
}

int HwNESEmulator::save() {
    return 0;
}

void HwNESEmulator::drawFrame(uint8_t *rgba, size_t size) {
    if (1 == focusHeight) {
        int offset = static_cast<int>((this->width - NES_DISP_WIDTH) / 2.0);
        for (int i = 0; i < NES_DISP_HEIGHT; ++i) {
            memcpy(&frameBuf[(i * this->width + offset) * 4], &rgba[i * NES_DISP_WIDTH * 4],
                   static_cast<size_t>(NES_DISP_WIDTH * 4));
        }
    } else if (-1 == focusHeight) {
        int offset = static_cast<int>((this->height - NES_DISP_HEIGHT) / 2.0);
        memcpy(&frameBuf[offset * this->width * 4], rgba, size);
    } else {
        memcpy(frameBuf, rgba, size);
    }
}

int HwNESEmulator::draw(uint8_t *rgba, size_t size) {
    simpleLock.lock();
    if (this->win && rgba) {
        drawFrame(rgba, size);
        ANativeWindow_Buffer nwBuffer;
        if (0 == ANativeWindow_lock(this->win, &nwBuffer, nullptr)) {
//            memset(rgba, 128, size);
//            Logcat::i("HWEMULATOR", "draw %d, %d, %d, %d", rgba[size / 2], rgba[size / 2 + 1],
//                      rgba[size / 2 + 2], rgba[size / 2 + 3]);
            memcpy(nwBuffer.bits, frameBuf, static_cast<size_t>(this->width * this->height * 4));
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

void HwNESEmulator::postEvent(HwGamePadEvent *event) {
    switch (event->getKey()) {
        case HwGamePadEvent::KEY_RIGHT:
            /* Right */
            handleEvent(7, event->getAction());
            break;
        case HwGamePadEvent::KEY_LEFT:
            /* Left */
            handleEvent(6, event->getAction());
            break;
        case HwGamePadEvent::KEY_DOWN:
            /* Down */
            handleEvent(5, event->getAction());
            break;
        case HwGamePadEvent::KEY_UP:
            /* Up */
            handleEvent(4, event->getAction());
            break;
        case HwGamePadEvent::KEY_START: {
            /* Start */
            handleEvent(3, event->getAction());
//            simpleLock.lock();
//            FILE *file = fopen("/sdcard/nes_capture.rgba", "wb");
//            fwrite(frameBuf, 1, this->width * this->height * 4, file);
//            fclose(file);
            Logcat::i("HWEMULATOR", "Capture %dx%d", this->width, this->height);
//            simpleLock.unlock();
            break;
        }
        case HwGamePadEvent::KEY_SELECT:
            /* Select */
            handleEvent(2, event->getAction());
            break;
        case HwGamePadEvent::KEY_A:
            /* 'A' */
            handleEvent(1, event->getAction());
            break;
        case HwGamePadEvent::KEY_B:
            /* 'B' */
            handleEvent(0, event->getAction());
            break;
        case HwGamePadEvent::KEY_ESC:
            /* Exit */
            dwKeySystem |= PAD_SYS_QUIT;
            break;
        default:
            break;
    }
    InfoNES_Event(dwKeyPad1, dwKeyPad2, dwKeySystem);
}

void HwNESEmulator::handleEvent(int event, int action) {
    if (HwGamePadEvent::ACTION_DOWN == action) {
        dwKeyPad1 |= (1 << event);
    } else {
        dwKeyPad1 &= ~(1 << event);
    }
}
