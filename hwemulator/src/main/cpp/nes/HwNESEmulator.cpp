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
    char *file = new char[rom.length() + 1];
    strcpy(file, rom.c_str());
    strcpy(strrchr(file, '.') + 1, "srm");
    string *fileStr = new string(file);
    int ret = load(fileStr);
    Logcat::i("HWEMULATOR", "Load %d", ret);
    delete[] file;
    delete fileStr;
    /* MainLoop */
    InfoNES_Main();
    return 0;
}

int HwNESEmulator::stop() {
    InfoNES_Stop();
    char *file = new char[rom.length() + 1];
    strcpy(file, rom.c_str());
    strcpy(strrchr(file, '.') + 1, "srm");
    string *fileStr = new string(file);
    int ret = save(fileStr);
    Logcat::i("HWEMULATOR", "Save %d", ret);
    delete[] file;
    delete fileStr;

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

int HwNESEmulator::save(string *file) {
/*
 *  Save a SRAM
 *
 *  Return values
 *     0 : Normally
 *    -1 : SRAM data couldn't be written
 */

    FILE *fp;
    int nUsedTable[256];
    unsigned char chData;
    unsigned char chPrevData;
    unsigned char chTag;
    int nIdx;
    int nEncoded;
    int nEncLen;
    int nRunLen;
    unsigned char pDstBuf[SRAM_SIZE];

//    if (!nSRAM_SaveFlag)
//        return 0;  // It doesn't need to save it

    /*-------------------------------------------------------------------*/
    /*  Compress a SRAM data                                             */
    /*-------------------------------------------------------------------*/

    memset(nUsedTable, 0, sizeof nUsedTable);

    for (nIdx = 0; nIdx < SRAM_SIZE; ++nIdx) {
        ++nUsedTable[SRAM[nIdx++]];
    }
    for (nIdx = 1, chTag = 0; nIdx < 256; ++nIdx) {
        if (nUsedTable[nIdx] < nUsedTable[chTag])
            chTag = nIdx;
    }

    nEncoded = 0;
    nEncLen = 0;
    nRunLen = 1;

    pDstBuf[nEncLen++] = chTag;

    chPrevData = SRAM[nEncoded++];

    while (nEncoded < SRAM_SIZE && nEncLen < SRAM_SIZE - 133) {
        chData = SRAM[nEncoded++];

        if (chPrevData == chData && nRunLen < 256)
            ++nRunLen;
        else {
            if (nRunLen >= 4 || chPrevData == chTag) {
                pDstBuf[nEncLen++] = chTag;
                pDstBuf[nEncLen++] = chPrevData;
                pDstBuf[nEncLen++] = nRunLen - 1;
            } else {
                for (nIdx = 0; nIdx < nRunLen; ++nIdx)
                    pDstBuf[nEncLen++] = chPrevData;
            }

            chPrevData = chData;
            nRunLen = 1;
        }

    }
    if (nRunLen >= 4 || chPrevData == chTag) {
        pDstBuf[nEncLen++] = chTag;
        pDstBuf[nEncLen++] = chPrevData;
        pDstBuf[nEncLen++] = nRunLen - 1;
    } else {
        for (nIdx = 0; nIdx < nRunLen; ++nIdx)
            pDstBuf[nEncLen++] = chPrevData;
    }

    /*-------------------------------------------------------------------*/
    /*  Write a SRAM data                                                */
    /*-------------------------------------------------------------------*/

    // Open SRAM file
    fp = fopen(file->c_str(), "wb");
    if (fp == NULL)
        return -1;

    // Write SRAM data
    fwrite(pDstBuf, nEncLen, 1, fp);

    // Close SRAM file
    fclose(fp);

    // Successful
    return 0;
}

int HwNESEmulator::load(string *file) {

/*
 *  Load a SRAM
 *
 *  Return values
 *     0 : Normally
 *    -1 : SRAM data couldn't be read
 */

    FILE *fp;
    unsigned char pSrcBuf[SRAM_SIZE];
    unsigned char chData;
    unsigned char chTag;
    int nRunLen;
    int nDecoded;
    int nDecLen;
    int nIdx;

    // It doesn't need to save it
//    nSRAM_SaveFlag = 0;

    // It is finished if the ROM doesn't have SRAM
    if (!ROM_SRAM)
        return -1;

    // There is necessity to save it
//    nSRAM_SaveFlag = 1;

    // The preparation of the SRAM file name
//    strcpy(szSaveName, szRomName);
//    strcpy(strrchr(szSaveName, '.') + 1, "srm");

    /*-------------------------------------------------------------------*/
    /*  Read a SRAM data                                                 */
    /*-------------------------------------------------------------------*/

    // Open SRAM file
    fp = fopen(file->c_str(), "rb");
    if (fp == NULL)
        return -2;

    // Read SRAM data
    fread(pSrcBuf, SRAM_SIZE, 1, fp);

    // Close SRAM file
    fclose(fp);

    /*-------------------------------------------------------------------*/
    /*  Extract a SRAM data                                              */
    /*-------------------------------------------------------------------*/

    nDecoded = 0;
    nDecLen = 0;

    chTag = pSrcBuf[nDecoded++];

    while (nDecLen < 8192) {
        chData = pSrcBuf[nDecoded++];

        if (chData == chTag) {
            chData = pSrcBuf[nDecoded++];
            nRunLen = pSrcBuf[nDecoded++];
            for (nIdx = 0; nIdx < nRunLen + 1; ++nIdx) {
                SRAM[nDecLen++] = chData;
            }
        } else {
            SRAM[nDecLen++] = chData;
        }
    }

    // Successful
    return 0;
}
