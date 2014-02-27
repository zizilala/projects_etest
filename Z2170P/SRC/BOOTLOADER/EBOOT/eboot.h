// All rights reserved ADENEO EMBEDDED 2010
// Copyright (c) 2007, 2008 BSQUARE Corporation. All rights reserved.

/*
================================================================================
*             Texas Instruments OMAP(TM) Platform Software
* (c) Copyright Texas Instruments, Incorporated. All Rights Reserved.
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied.
*
================================================================================
*/
//
//  Header:  eboot.h
//
//  This header file is comprised of component header files that defines
//  the standard include hierarchy for the bootloader. It also defines few
//  trivial constant.
//
#ifndef __EBOOT_H
#define __EBOOT_H

//------------------------------------------------------------------------------
#include "bsp.h"

#pragma warning(push)
#pragma warning(disable: 4201 4115)

#include <blcommon.h>
#include <nkintr.h>
#include <halether.h>
#include <fmd.h>
#include <bootpart.h>
#include <oal.h>
#include <oal_blmenu.h>

#pragma warning(pop)

#include "boot_args.h"
#include "args.h"

//------------------------------------------------------------------------------

#define EBOOT_VERSION_MAJOR         1
#define EBOOT_VERSION_MINOR         0

//------------------------------------------------------------------------------
//  structure
//
typedef struct {
    UINT32 start;
    UINT32 length;
    UINT32 base;
} REGION_INFO_EX;

typedef struct {
    DWORD cookie;
    DWORD priority256;
    DWORD syncPeriod;
    HANDLE hTWL;
    HANDLE hIntrEvent;
    HANDLE hIntrThread;
    BOOL threadsExit;
    DWORD enableWake;
    CEDEVICE_POWER_STATE powerState;
} Device_for_ADCRTC;


//------------------------------------------------------------------------------

#define DOWNLOAD_TYPE_UNKNOWN       0
#define DOWNLOAD_TYPE_RAM           1
#define DOWNLOAD_TYPE_BINDIO        2
#define DOWNLOAD_TYPE_XLDR          3
#define DOWNLOAD_TYPE_EBOOT         4
#define DOWNLOAD_TYPE_IPL           5
#define DOWNLOAD_TYPE_FLASHNAND     6
#define DOWNLOAD_TYPE_FLASHNOR      7
#define DOWNLOAD_TYPE_LOGO			8
#define DOWNLOAD_TYPE_EXT			9

//------------------------------------------------------------------------------

#define LOGO_NB0_FILE				"logo.nb0"
#define LOGO_NB0_FILE_LEN           8

//  Preprocessor
#define EN_VIO_1V8_Z2170P           137
#define EN_DC_3V3                   23
#define EN_SB_CHARGE                139     //Ray 140225

//************************ COLOR Table*****************************************
#define RED_COLOR			0
#define GREEN_COLOR			1
#define	BLUE_COLOR			2
#define WHITE_COLOR			3
#define BLACK_COLOR			4
#define YELLOW_COLOR		5
#define PURPLE_COLOR		6
#define CYAN_COLOR			7
#define GRAY_COLOR			8
#define TRANSPARENT_COLOR	9 
//************************ COLOR Table*****************************************


//------------------------------------------------------------------------------
//  Global variable
//
HANDLE ghTwl;           //Ray 140218

//------------------------------------------------------------------------------

typedef struct {
    OAL_KITL_TYPE bootDeviceType;
    UINT32 type;
    UINT32 numRegions;
    UINT32 launchAddress;
    REGION_INFO_EX region[BL_MAX_BIN_REGIONS];

    UINT32 recordOffset;
    UINT8  *pReadBuffer;
    UINT32 readSize;
} EBOOT_CONTEXT;

//------------------------------------------------------------------------------

extern BOOT_CFG g_bootCfg;
extern EBOOT_CONTEXT g_eboot;
extern OAL_KITL_DEVICE g_bootDevices[];
extern OAL_KITL_DEVICE g_kitlDevices[];

extern UINT32   g_ulFlashBase;

//------------------------------------------------------------------------------

VOID OEMMultiBinNotify(MultiBINInfo *pInfo);

//------------------------------------------------------------------------------

VOID   BLMenu(BOOL bForced);    
BOOL   BLReadBootCfg(BOOT_CFG *pBootCfg);
BOOL   BLWriteBootCfg(BOOT_CFG *pBootCfg);
BOOL   BLReserveBootBlocks();
BOOL   BLConfigureFlashPartitions(BOOL bForceEnable);
BOOL   BLShowLogo();            
UINT32 BLEthDownload(BOOT_CFG *pBootCfg, OAL_KITL_DEVICE *pBootDevices);
BOOL   BLEthReadData(ULONG size, UCHAR *pData);
VOID   BLEthConfig(BSP_ARGS *pArgs);
UINT32 BLSDCardDownload(WCHAR *filename);
BOOL   BLSDCardReadData(ULONG size, UCHAR *pData);
UINT32 BLFlashDownload(BOOT_CFG *pConfig, OAL_KITL_DEVICE *pBootDevices);
BOOL   BLSDCardReadLogo(WCHAR *filename, UCHAR *pData, DWORD size);
//BOOL   BLSDCardReadCalibData(WCHAR *filename, BYTE *pData, DWORD size);
BOOL   BLSDCardReadEbootData(WCHAR *filename, BYTE *pData, DWORD size);
//UINT32 BLVAtoPA(UINT32 address);

BOOL   SDCardUpdatingFW(WCHAR *filename, UCHAR *pData, DWORD size);    //Ray 140122, Read SD card update firmware 
VOID   BLShowMenu();                    //Ray 140213
VOID   HotkeyMenuSelect(HANDLE);        //Ray 140218

UINT32 OALStringToUINT32(LPCWSTR psz);
VOID   printString(int row, int col, int fc, int bc, char showCharMode[][30]);    //Ray, 140224
VOID   ShowColor(UINT32, UINT32, UCHAR backColor);                          //140225
VOID   printStringMode(int row, int col, int fc, int bc, char showCharMode[][30], int mode);

VOID LcdStall(DWORD);
VOID LcdSleep(DWORD);

//------------------------------------------------------------------------------

#endif
