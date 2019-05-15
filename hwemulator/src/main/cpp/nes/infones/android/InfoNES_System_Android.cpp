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

static long run_emulator(char *rom_path);

/*-------------------------------------------------------------------*/
/*  Palette data                                                     */
/*-------------------------------------------------------------------*/
extern WORD NesPalette[];

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

/* Menu screen */
int InfoNES_Menu() {
    return 0;
}

/* Read ROM image file */
int InfoNES_ReadRom(const char *pszFileName) {

    return 0;
}

/* Release a memory for ROM */
void InfoNES_ReleaseRom() {

}

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame() {

}

/* Get a joypad state */
void InfoNES_PadState(DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem) {

}

/* memcpy */
void *InfoNES_MemoryCopy(void *dest, const void *src, int count) {
    return 0;
}

/* memset */
void *InfoNES_MemorySet(void *dest, int c, int count) {
    return 0;
}

/* Print debug message */
void InfoNES_DebugPrint(char *pszMsg) {

}

/* Wait */
void InfoNES_Wait() {

}

/* Sound Initialize */
void InfoNES_SoundInit(void) {

}

/* Sound Open */
int InfoNES_SoundOpen(int samples_per_sync, int sample_rate) {
    return 0;
}

/* Sound Close */
void InfoNES_SoundClose(void) {

}

/* Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise, 1 DPCM */
void
InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5) {

}

/* Print system message */
void InfoNES_MessageBox(char *pszMsg, ...) {

}

#endif /* !InfoNES_SYSTEM_H_INCLUDED */
