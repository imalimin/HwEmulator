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


/*-------------------------------------------------------------------*/
/*  Palette data                                                     */
/*-------------------------------------------------------------------*/

/* Palette data */
#if 0
WORD NesPalette[ 64 ] =
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
#else
WORD NesPalette[64];
#endif

BYTE NesPaletteRGB[64][3] = {
        112, 112, 112, 32, 24, 136, 0, 0, 168, 64, 0, 152,
        136, 0, 112, 168, 0, 16, 160, 0, 0, 120, 8, 0,
        64, 40, 0, 0, 64, 0, 0, 80, 0, 0, 56, 16,
        24, 56, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        184, 184, 184, 0, 112, 232, 32, 56, 232, 128, 0, 240,
        184, 0, 184, 224, 0, 88, 216, 40, 0, 200, 72, 8,
        136, 112, 0, 0, 144, 0, 0, 168, 0, 0, 144, 56,
        0, 128, 136, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        248, 248, 248, 56, 184, 248, 88, 144, 248, 64, 136, 248,
        240, 120, 248, 248, 112, 176, 248, 112, 96, 248, 152, 56,
        240, 184, 56, 128, 208, 16, 72, 216, 72, 88, 248, 152,
        0, 232, 216, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        248, 248, 248, 168, 224, 248, 192, 208, 248, 208, 200, 248,
        248, 192, 248, 248, 192, 216, 248, 184, 176, 248, 216, 168,
        248, 224, 160, 224, 248, 160, 168, 240, 184, 176, 248, 200,
        152, 248, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

/* Menu screen */
int InfoNES_Menu() {
    Logcat::e("HWEMULATOR", "InfoNES_Menu");
    return 0;
}

/* Read ROM image file */
int InfoNES_ReadRom(const char *pszFileName) {
    Logcat::e("HWEMULATOR", "InfoNES_ReadRom: %s", pszFileName);

    return 0;
}

/* Release a memory for ROM */
void InfoNES_ReleaseRom() {
    Logcat::e("HWEMULATOR", "InfoNES_ReleaseRom");

}

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame() {
    Logcat::e("HWEMULATOR", "InfoNES_LoadFrame");

}

/* Get a joypad state */
void InfoNES_PadState(DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem) {
    Logcat::e("HWEMULATOR", "InfoNES_PadState");

}

/* memcpy */
void *InfoNES_MemoryCopy(void *dest, const void *src, int count) {
    Logcat::e("HWEMULATOR", "InfoNES_MemoryCopy: count=%d", count);
    return 0;
}

/* memset */
void *InfoNES_MemorySet(void *dest, int c, int count) {
    Logcat::e("HWEMULATOR", "InfoNES_MemorySet: count=%d", count);
    return 0;
}

/* Print debug message */
void InfoNES_DebugPrint(char *pszMsg) {
    Logcat::e("HWEMULATOR", "DEBUG: %s", pszMsg);

}

/* Wait */
void InfoNES_Wait() {
    Logcat::e("HWEMULATOR", "InfoNES_Wait");

}

/* Sound Initialize */
void InfoNES_SoundInit(void) {
    Logcat::e("HWEMULATOR", "InfoNES_SoundInit");

}

/* Sound Open */
int InfoNES_SoundOpen(int samples_per_sync, int sample_rate) {
    Logcat::e("HWEMULATOR", "InfoNES_SoundOpen: %d, %d", samples_per_sync, sample_rate);
    return 0;
}

/* Sound Close */
void InfoNES_SoundClose(void) {
    Logcat::e("HWEMULATOR", "InfoNES_SoundClose");
}

/* Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise, 1 DPCM */
void
InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5) {
    Logcat::e("HWEMULATOR", "InfoNES_SoundOutput");
}

/* Print system message */
void InfoNES_MessageBox(char *pszMsg, ...) {
    va_list args;
    va_start(args, pszMsg);
    Logcat::e("HWEMULATOR", pszMsg, args);
    va_end(args);
}

#endif /* !InfoNES_SYSTEM_H_INCLUDED */
