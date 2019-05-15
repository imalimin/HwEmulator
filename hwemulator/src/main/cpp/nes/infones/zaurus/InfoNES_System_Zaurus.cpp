/*===================================================================*/
/*                                                                   */
/* InfoNES_System_Zaurus.cpp : The function which depends on a       */
/*                             system (for Linux Zaurus)             */
/*                                                                   */
/*  2004/06/13  InfoNES Project                                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>

#include <qpe/qpeapplication.h>
#include "zinfones.h"
#include "zfileselector.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/soundcard.h>

#include "../InfoNES.h"
#include "../InfoNES_System.h"
#include "../InfoNES_pAPU.h"

/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/

char szFileName[ 256 ];
char szRomName[ 256 ];
char szSaveName[ 256 ];
int nSRAM_SaveFlag;

/*-------------------------------------------------------------------*/
/*  Variables for Linux Zaurus                                       */
/*-------------------------------------------------------------------*/
#define SOUND_DEVICE "/dev/dsp"
#define VERSION      "InfoNES v0.93J RC4"

/* For Sound Emulation */
BYTE final_wave[2048];
int waveptr;
int wavflag;
int sound_fd;

// Palette data
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

/* Emulation thread */
pthread_t emulation_tid;
int bThread;

/* For Qt */
zinfones *zi;

/*-------------------------------------------------------------------*/
/*  Function prototypes ( Linux Zaurus specific )                    */
/*-------------------------------------------------------------------*/
void *emulation_thread( void *args );
int LoadSRAM();
int SaveSRAM();
int StartEmulation(const char *);
int TerminateEmulation();

/*===================================================================*/
/*                                                                   */
/*                  main() : Application main                        */
/*                                                                   */
/*===================================================================*/
int main( int argc, char ** argv )
{
  // Help message
  printf("%s: A fast and portable NES emulator\n"
	 "Copyright (c) 1999-2004 Jay's Factory <jays_factory@excite.co.jp>\n",
	 VERSION );

  // Create Qt window
  QPEApplication a( argc, argv );
  zi = new zinfones( WorkFrame );
  a.setMainWidget( zi );
  zi->setCaption( VERSION );	
  zi->show();

  // Start Emulation
  if ( argc < 2 )
  {   
    /* File selector */ 
    QString sFile;
    zfileselector dlg( ".", zi, VERSION, TRUE, 0 );

    if ( dlg.exec() == QDialog::Accepted )
    {
      sFile = dlg.getFile();
    }

    if ( StartEmulation( sFile.ascii() ) < 0 )
      exit( 0 );
  } else {  
    if ( StartEmulation( argv[1] ) < 0 )
      exit( 0 );
  }

  // Hook to Qt application
  a.exec();

  // Terminate emulation
  TerminateEmulation();  
}

/*===================================================================*/
/*                                                                   */
/*                StartEmulation() : Start Emulation                 */
/*                                                                   */
/*===================================================================*/

int StartEmulation(const char *fname)
{
  strcpy( szFileName, fname);

  // Load cassette
  if ( InfoNES_Load( szFileName ) == 0 )
  {
    // Set a ROM image name
    strcpy( szRomName, szFileName );
    LoadSRAM();

    /* Create Emulation Thread */
    bThread = TRUE;
    pthread_create( &emulation_tid, NULL, emulation_thread, NULL );
    return 0;
  }
  return -1;
}

/*===================================================================*/
/*                                                                   */
/*           TerminateEmulation() : Terminate Emulation              */
/*                                                                   */
/*===================================================================*/

int TerminateEmulation()
{
  /* Terminate emulation thread */
  bThread = FALSE;
  zi->dwKeySystem |= PAD_SYS_QUIT; 

  /* Waiting for Termination */
  pthread_join( emulation_tid, NULL );
  
  /* Save SRAM*/
  SaveSRAM();

  return 0;
}

/*===================================================================*/
/*                                                                   */
/*           emulation_thread() : Thread Hooking Routine             */
/*                                                                   */
/*===================================================================*/

void *emulation_thread(void *args)
{
  InfoNES_Main();
}

/*===================================================================*/
/*                                                                   */
/*           LoadSRAM() : Load a SRAM                                */
/*                                                                   */
/*===================================================================*/
int LoadSRAM()
{
/*
 *  Load a SRAM
 *
 *  Return values
 *     0 : Normally
 *    -1 : SRAM data couldn't be read
 */

  FILE *fp;
  unsigned char pSrcBuf[ SRAM_SIZE ];
  unsigned char chData;
  unsigned char chTag;
  int nRunLen;
  int nDecoded;
  int nDecLen;
  int nIdx;

  // It doesn't need to save it
  nSRAM_SaveFlag = 0;

  // It is finished if the ROM doesn't have SRAM
  if ( !ROM_SRAM )
    return 0;

  // There is necessity to save it
  nSRAM_SaveFlag = 1;

  // The preparation of the SRAM file name
  strcpy( szSaveName, szRomName );
  strcpy( strrchr( szSaveName, '.' ) + 1, "srm" );

  /*-------------------------------------------------------------------*/
  /*  Read a SRAM data                                                 */
  /*-------------------------------------------------------------------*/

  // Open SRAM file
  fp = fopen( szSaveName, "rb" );
  if ( fp == NULL )
    return -1;

  // Read SRAM data
  fread( pSrcBuf, SRAM_SIZE, 1, fp );

  // Close SRAM file
  fclose( fp );

  /*-------------------------------------------------------------------*/
  /*  Extract a SRAM data                                              */
  /*-------------------------------------------------------------------*/

  nDecoded = 0;
  nDecLen = 0;

  chTag = pSrcBuf[ nDecoded++ ];

  while ( nDecLen < 8192 )
  {
    chData = pSrcBuf[ nDecoded++ ];

    if ( chData == chTag )
    {
      chData = pSrcBuf[ nDecoded++ ];
      nRunLen = pSrcBuf[ nDecoded++ ];
      for ( nIdx = 0; nIdx < nRunLen + 1; ++nIdx )
      {
        SRAM[ nDecLen++ ] = chData;
      }
    }
    else
    {
      SRAM[ nDecLen++ ] = chData;
    }
  }

  // Successful
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*           SaveSRAM() : Save a SRAM                                */
/*                                                                   */
/*===================================================================*/
int SaveSRAM()
{
/*
 *  Save a SRAM
 *
 *  Return values
 *     0 : Normally
 *    -1 : SRAM data couldn't be written
 */

  FILE *fp;
  int nUsedTable[ 256 ];
  unsigned char chData;
  unsigned char chPrevData;
  unsigned char chTag;
  int nIdx;
  int nEncoded;
  int nEncLen;
  int nRunLen;
  unsigned char pDstBuf[ SRAM_SIZE ];

  if ( !nSRAM_SaveFlag )
    return 0;  // It doesn't need to save it

  /*-------------------------------------------------------------------*/
  /*  Compress a SRAM data                                             */
  /*-------------------------------------------------------------------*/

  memset( nUsedTable, 0, sizeof nUsedTable );

  for ( nIdx = 0; nIdx < SRAM_SIZE; ++nIdx )
  {
    ++nUsedTable[ SRAM[ nIdx++ ] ];
  }
  for ( nIdx = 1, chTag = 0; nIdx < 256; ++nIdx )
  {
    if ( nUsedTable[ nIdx ] < nUsedTable[ chTag ] )
      chTag = nIdx;
  }

  nEncoded = 0;
  nEncLen = 0;
  nRunLen = 1;

  pDstBuf[ nEncLen++ ] = chTag;

  chPrevData = SRAM[ nEncoded++ ];

  while ( nEncoded < SRAM_SIZE && nEncLen < SRAM_SIZE - 133 )
  {
    chData = SRAM[ nEncoded++ ];

    if ( chPrevData == chData && nRunLen < 256 )
      ++nRunLen;
    else
    {
      if ( nRunLen >= 4 || chPrevData == chTag )
      {
        pDstBuf[ nEncLen++ ] = chTag;
        pDstBuf[ nEncLen++ ] = chPrevData;
        pDstBuf[ nEncLen++ ] = nRunLen - 1;
      }
      else
      {
        for ( nIdx = 0; nIdx < nRunLen; ++nIdx )
          pDstBuf[ nEncLen++ ] = chPrevData;
      }

      chPrevData = chData;
      nRunLen = 1;
    }

  }
  if ( nRunLen >= 4 || chPrevData == chTag )
  {
    pDstBuf[ nEncLen++ ] = chTag;
    pDstBuf[ nEncLen++ ] = chPrevData;
    pDstBuf[ nEncLen++ ] = nRunLen - 1;
  }
  else
  {
    for ( nIdx = 0; nIdx < nRunLen; ++nIdx )
      pDstBuf[ nEncLen++ ] = chPrevData;
  }

  /*-------------------------------------------------------------------*/
  /*  Write a SRAM data                                                */
  /*-------------------------------------------------------------------*/

  // Open SRAM file
  fp = fopen( szSaveName, "wb" );
  if ( fp == NULL )
    return -1;

  // Write SRAM data
  fwrite( pDstBuf, nEncLen, 1, fp );

  // Close SRAM file
  fclose( fp );

  // Successful
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*                  InfoNES_Menu() : Menu screen                     */
/*                                                                   */
/*===================================================================*/
int InfoNES_Menu()
{
/*
 *  Menu screen
 *
 *  Return values
 *     0 : Normally
 *    -1 : Exit InfoNES
 */

  if ( PAD_PUSH( PAD_System, PAD_SYS_QUIT) )
    return -1;	 	

  // Nothing to do here
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*               InfoNES_ReadRom() : Read ROM image file             */
/*                                                                   */
/*===================================================================*/
int InfoNES_ReadRom( const char *pszFileName )
{
/*
 *  Read ROM image file
 *
 *  Parameters
 *    const char *pszFileName          (Read)
 *
 *  Return values
 *     0 : Normally
 *    -1 : Error
 */

  FILE *fp;

  /* Open ROM file */
  fp = fopen( pszFileName, "rb" );
  if ( fp == NULL )
    return -1;

  /* Read ROM Header */
  fread( &NesHeader, sizeof NesHeader, 1, fp );
  if ( memcmp( NesHeader.byID, "NES\x1a", 4 ) != 0 )
  {
    /* not .nes file */
    fclose( fp );
    return -1;
  }

  /* Clear SRAM */
  memset( SRAM, 0, SRAM_SIZE );

  /* If trainer presents Read Triner at 0x7000-0x71ff */
  if ( NesHeader.byInfo1 & 4 )
  {
    fread( &SRAM[ 0x1000 ], 512, 1, fp );
  }

  /* Allocate Memory for ROM Image */
  ROM = (BYTE *)malloc( NesHeader.byRomSize * 0x4000 );

  /* Read ROM Image */
  fread( ROM, 0x4000, NesHeader.byRomSize, fp );

  if ( NesHeader.byVRomSize > 0 )
  {
    /* Allocate Memory for VROM Image */
    VROM = (BYTE *)malloc( NesHeader.byVRomSize * 0x2000 );

    /* Read VROM Image */
    fread( VROM, 0x2000, NesHeader.byVRomSize, fp );
  }

  /* File close */
  fclose( fp );

  /* Successful */
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*           InfoNES_ReleaseRom() : Release a memory for ROM         */
/*                                                                   */
/*===================================================================*/
void InfoNES_ReleaseRom()
{
/*
 *  Release a memory for ROM
 *
 */

  if ( ROM )
  {
    free( ROM );
    ROM = NULL;
  }

  if ( VROM )
  {
    free( VROM );
    VROM = NULL;
  }
}

/*===================================================================*/
/*                                                                   */
/*      InfoNES_LoadFrame() :                                        */
/*           Transfer the contents of work frame on the screen       */
/*                                                                   */
/*===================================================================*/
void InfoNES_LoadFrame()
{
/*
 *  Transfer the contents of work frame on the screen
 *
 */
#if 0
  zi->repaint();
#else
  zi->loadFrame();
#endif
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_PadState() : Get a joypad state               */
/*                                                                   */
/*===================================================================*/
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem )
{
/*
 *  Get a joypad state
 *
 *  Parameters
 *    DWORD *pdwPad1                   (Write)
 *      Joypad 1 State
 *
 *    DWORD *pdwPad2                   (Write)
 *      Joypad 2 State
 *
 *    DWORD *pdwSystem                 (Write)
 *      Input for InfoNES
 *
 */

  *pdwPad1   = zi->dwKeyPad1;	
  *pdwPad2   = zi->dwKeyPad2;	
  *pdwSystem = zi->dwKeySystem;	
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemoryCopy() : memcpy                         */
/*                                                                   */
/*===================================================================*/
void *InfoNES_MemoryCopy( void *dest, const void *src, int count )
{
/*
 *  memcpy
 *
 *  Parameters
 *    void *dest                       (Write)
 *      Points to the starting address of the copied block's destination
 *
 *    const void *src                  (Read)
 *      Points to the starting address of the block of memory to copy
 *
 *    int count                        (Read)
 *      Specifies the size, in bytes, of the block of memory to copy
 *
 *  Return values
 *    Pointer of destination
 */

  memcpy( dest, src, count );
  return dest;
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemorySet() : memset                          */
/*                                                                   */
/*===================================================================*/
void *InfoNES_MemorySet( void *dest, int c, int count )
{
/*
 *  memset
 *
 *  Parameters
 *    void *dest                       (Write)
 *      Points to the starting address of the block of memory to fill
 *
 *    int c                            (Read)
 *      Specifies the byte value with which to fill the memory block
 *
 *    int count                        (Read)
 *      Specifies the size, in bytes, of the block of memory to fill
 *
 *  Return values
 *    Pointer of destination
 */

  memset( dest, c, count);  
  return dest;
}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundInit() : Sound Emulation Initialize           */
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundInit( void ) 
{
  sound_fd = 0;
}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundOpen() : Sound Open                           */
/*                                                                   */
/*===================================================================*/
int InfoNES_SoundOpen( int samples_per_sync, int sample_rate ) 
{
  int tmp;
  int result;
  int sound_rate;
  int sound_frag;

  waveptr = 0;
  wavflag = 0;

  /* Open sound device */
  sound_fd = open( SOUND_DEVICE, O_WRONLY );
  if ( sound_fd < 0 ) 
  {
    InfoNES_MessageBox("opening "SOUND_DEVICE"...failed");
    sound_fd = 0;
    return 0;
  } else {

  }
  
  /* Setting unsigned 8-bit format */ 
  tmp = AFMT_U8;
  result = ioctl(sound_fd, SNDCTL_DSP_SETFMT, &tmp);
  if ( result < 0 ) 
  {
    InfoNES_MessageBox("setting unsigned 8-bit format...failed");
    close(sound_fd);
    sound_fd = 0;
    return 0;
  } else {

  }
    
  /* Setting mono mode */
  tmp = 0;
  result = ioctl(sound_fd, SNDCTL_DSP_STEREO, &tmp);
  if (result < 0) 
  {
    InfoNES_MessageBox("setting mono mode...failed");
    close(sound_fd);
    sound_fd = 0;
    return 0;
  } else {

  }
    
  sound_rate = sample_rate;
  result = ioctl(sound_fd, SNDCTL_DSP_SPEED, &sound_rate);
  if ( result < 0 ) 
  {
    InfoNES_MessageBox("setting sound rate...failed");
    close(sound_fd);
    sound_fd = 0;
    return 0;
  } else {

  }

  /* high word of sound_frag is number of frags, low word is frag size */
  sound_frag = 0x00080008;
  result = ioctl(sound_fd, SNDCTL_DSP_SETFRAGMENT, &sound_frag);
  if (result < 0) 
  {
    InfoNES_MessageBox("setting soundfrags...failed");
    close(sound_fd);
    sound_fd = 0;
    return 0;
  } else {

  }

  /* Successful */
  return 1;
}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundClose() : Sound Close                         */
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundClose( void ) 
{
  if ( sound_fd ) 
  {
    close(sound_fd);
  }
}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_SoundOutput() : Sound Output 5 Waves           */           
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundOutput( int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5 )
{
  int i;

  if ( sound_fd ) 
  {
    for (i = 0; i < samples; i++) 
    {
#if 1
      final_wave[ waveptr ] = 
	( wave1[i] + wave2[i] + wave3[i] + wave4[i] + wave5[i] ) / 5;
#else
      final_wave[ waveptr ] = wave4[i];
#endif

      waveptr++;
      if ( waveptr == 2048 ) 
      {
	waveptr = 0;
	wavflag = 2;
      } 
      else if ( waveptr == 1024 )
      {
	wavflag = 1;
      }
    }
	
    if ( wavflag )
    {
      if ( write( sound_fd, &final_wave[(wavflag - 1) << 10], 1024) < 1024 ) 
      {
	InfoNES_MessageBox( "wrote less than 1024 bytes\n" );
      }
      wavflag = 0;
    }
  }
}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_Wait() : Wait Emulation if required            */
/*                                                                   */
/*===================================================================*/
void InfoNES_Wait() {}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_MessageBox() : Print System Message            */
/*                                                                   */
/*===================================================================*/
void InfoNES_MessageBox( char *pszMsg, ... )
{
    va_list args;
    va_start( args, pszMsg );
    printf( pszMsg, args );	
    va_end( args );
}
