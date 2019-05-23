/*===================================================================*/
/*                                                                   */
/*  InfoNES_System.h : The function which depends on a system        */
/*                                                                   */
/*  2000/05/29  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef InfoNES_SYSTEM_H_INCLUDED
#define InfoNES_SYSTEM_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../InfoNES_Types.h"
#include "../InfoNES.h"
#include "../InfoNES_System.h"
#include "../InfoNES_pAPU.h"

#include "AudioPlayer.h"
#include "Logcat.h"
#include "../include/HwNESEmulator.h"


/*-------------------------------------------------------------------*/
/*  Palette data                                                     */
/*-------------------------------------------------------------------*/

/* Palette data */
WORD NesPalette[64] =
        {
                0x39ce, 0x1071, 0x0015, 0x2013, 0x440e, 0x5402, 0x5000, 0x3c20,
                0x20a0, 0x0100, 0x0140, 0x00e2, 0x0ceb, 0x0000, 0x0000, 0x0000,
                0x5ef7, 0x01dd, 0x10fd, 0x401e, 0x5c17, 0x700b, 0x6ca0, 0x6521,
                0x45c0, 0x0240, 0x02a0, 0x0247, 0x0211, 0x0000, 0x0000, 0x0000,
                0x7fff, 0x1eff, 0x2e5f, 0x223f, 0x79ff, 0x7dd6, 0x7dcc, 0x7e67,
                0x7ae7, 0x4342, 0x2769, 0x2ff3, 0x03bb, 0x0000, 0x0000, 0x0000,
                0x7fff, 0x579f, 0x635f, 0x6b3f, 0x7f1f, 0x7f1b, 0x7ef6, 0x7f75,
                0x7f94, 0x73f4, 0x57d7, 0x5bf9, 0x4ffe, 0x0000, 0x0000, 0x0000
        };

bool bThread = true;
//Key
DWORD dwKeyPad1 = 0;
DWORD dwKeyPad2 = 0;
DWORD dwKeySystem = 0;
AudioPlayer *audioPlayer = nullptr;
HwNESEmulator *emulator = nullptr;

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

static void handleEvent(int event, bool pressed) {
    if (pressed) {
        dwKeyPad1 |= (1 << event);
    } else {
        dwKeyPad1 &= ~(1 << event);
    }
}

void InfoNES_Event(DWORD keyPad1, DWORD keyPad2, DWORD keySystem) {
    dwKeyPad1 = keyPad1;
    dwKeyPad2 = keyPad2;
    dwKeySystem = keySystem;
}

void InfoNES_Attach(HwNESEmulator *e) {
    emulator = e;
}

void InfoNES_Stop() {
    if (bThread) {
        bThread = false;
        dwKeySystem |= PAD_SYS_QUIT;
    }
    emulator = nullptr;
}

/* Menu screen */
int InfoNES_Menu() {
//    Logcat::e("HWEMULATOR", "InfoNES_Menu");
    if (!bThread) {
        return -1;
    }
    return 0;
}

/* Read ROM image file */
int InfoNES_ReadRom(const char *pszFileName) {
    Logcat::e("HWEMULATOR", "InfoNES_ReadRom: %s", pszFileName);
    FILE *fp;
    /* Open ROM file */
    fp = fopen(pszFileName, "rb");
    if (fp == NULL) return -1;

    /* Read ROM Header */
    fread(&NesHeader, sizeof NesHeader, 1, fp);
    if (memcmp(NesHeader.byID, "NES\x1a", 4) != 0) {
        /* not .nes file */
        fclose(fp);
        return -1;
    }

    /* Clear SRAM */
    memset(SRAM, 0, SRAM_SIZE);

    /* If trainer presents Read Triner at 0x7000-0x71ff */
    if (NesHeader.byInfo1 & 4) {
        fread(&SRAM[0x1000], 512, 1, fp);
    }

    /* Allocate Memory for ROM Image */
    ROM = (BYTE *) malloc(NesHeader.byRomSize * 0x4000);

    /* Read ROM Image */
    fread(ROM, 0x4000, NesHeader.byRomSize, fp);

    if (NesHeader.byVRomSize > 0) {
        /* Allocate Memory for VROM Image */
        VROM = (BYTE *) malloc(NesHeader.byVRomSize * 0x2000);

        /* Read VROM Image */
        fread(VROM, 0x2000, NesHeader.byVRomSize, fp);
    }

    /* File close */
    fclose(fp);

    /* Successful */
    return 0;
}

/* Release a memory for ROM */
void InfoNES_ReleaseRom() {
    Logcat::e("HWEMULATOR", "InfoNES_ReleaseRom");
    bThread = true;
    if (ROM) {
        free(ROM);
        ROM = nullptr;
    }

    if (VROM) {
        free(VROM);
        VROM = nullptr;
    }
//    if (frameBuf) {
//        delete[]frameBuf;
//        frameBuf = nullptr;
//    }
}

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame() {
//    Logcat::e("HWEMULATOR", "InfoNES_LoadFrame %d, %d, %d, %d",
//              WorkFrame[NES_DISP_HEIGHT * NES_DISP_WIDTH / 2],
//              WorkFrame[NES_DISP_HEIGHT * NES_DISP_WIDTH / 2 + 1],
//              WorkFrame[NES_DISP_HEIGHT * NES_DISP_WIDTH / 2 + 2],
//              WorkFrame[NES_DISP_HEIGHT * NES_DISP_WIDTH / 2 + 3]);
    uint8_t *frameBuf = new uint8_t[NES_DISP_WIDTH * NES_DISP_HEIGHT * 4];
    uint8_t *buf = frameBuf;
    /* Exchange 16-bit to 24-bit  */
    for (int y = 0; y < NES_DISP_HEIGHT; y++) {
        for (int x = 0; x < NES_DISP_WIDTH; x++) {
            WORD wColor = WorkFrame[(y << 8) + x];
            *(buf++) = (uint8_t) ((wColor & 0x7c00) >> 7);
            *(buf++) = (uint8_t) ((wColor & 0x03e0) >> 2);
            *(buf++) = (uint8_t) ((wColor & 0x001f) << 3);
            *(buf++) = 128;
        }
    }
    if (emulator) {
        emulator->draw(frameBuf, NES_DISP_WIDTH * NES_DISP_HEIGHT * 4);
    }
    delete[]frameBuf;
}

/* Get a joypad state */
void InfoNES_PadState(DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem) {
//    Logcat::e("HWEMULATOR", "InfoNES_PadState");
    *pdwPad1 = dwKeyPad1;
    *pdwPad2 = dwKeyPad2;
    *pdwSystem = dwKeySystem;
}

/* memcpy */
void *InfoNES_MemoryCopy(void *dest, const void *src, int count) {
//    Logcat::e("HWEMULATOR", "InfoNES_MemoryCopy: count=%d", count);
    memcpy(dest, src, count);
    return dest;
}

/* memset */
void *InfoNES_MemorySet(void *dest, int c, int count) {
//    Logcat::e("HWEMULATOR", "InfoNES_MemorySet: count=%d", count);
    memset(dest, c, count);
    return dest;
}

/* Print debug message */
void InfoNES_DebugPrint(char *pszMsg) {
    Logcat::e("HWEMULATOR", "DEBUG: %s", pszMsg);
}

/* Wait */
void InfoNES_Wait() {
//    Logcat::e("HWEMULATOR", "InfoNES_Wait");
    //TODO
}

/* Sound Initialize */
void InfoNES_SoundInit(void) {
    Logcat::e("HWEMULATOR", "InfoNES_SoundInit");
    //TODO
}

/* Sound Open */
int InfoNES_SoundOpen(int samples_per_sync, int sample_rate) {
    Logcat::e("HWEMULATOR", "InfoNES_SoundOpen: %d, %d", samples_per_sync, sample_rate);
    audioPlayer = new AudioPlayer(1, static_cast<uint32_t>(sample_rate),
                                  SL_PCMSAMPLEFORMAT_FIXED_8, 1024);
    audioPlayer->start();
    return 0;
}

/* Sound Close */
void InfoNES_SoundClose(void) {
    Logcat::e("HWEMULATOR", "InfoNES_SoundClose");
    if (audioPlayer) {
        audioPlayer->stop();
        delete audioPlayer;
        audioPlayer = nullptr;
    }
}

/* Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise, 1 DPCM */
void
InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5) {
//    Logcat::e("HWEMULATOR", "InfoNES_SoundOutput: %d", samples);
    if (audioPlayer) {
        BYTE *final_wave = new BYTE[samples];
        for (int i = 0; i < samples; i++) {
            final_wave[i] = static_cast<BYTE>(
                    (wave1[i] + wave2[i] + wave3[i] + wave4[i] + wave5[i]) / 5);
        }
        audioPlayer->write(final_wave, samples);
    }
}

/* Print system message */
void InfoNES_MessageBox(char *pszMsg, ...) {
    va_list args;
    va_start(args, pszMsg);
    Logcat::e("HWEMULATOR", pszMsg, args);
    va_end(args);
}

#endif /* !InfoNES_SYSTEM_H_INCLUDED */
