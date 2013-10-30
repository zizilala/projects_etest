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
//  File:  eth.c
//
//  This file implements bootloader functions related to ethernet download
//  
#include "eboot.h"

//------------------------------------------------------------------------------
//
//  Global:  EdbgDebugZone
//
//  This variable specifies level of debug ouptup from BLCommon library.
//
ULONG EdbgDebugZone = 0;
extern CHAR  *gDevice_prefix;

//------------------------------------------------------------------------------
//
//  Static: g_ethState
//
//  This structure contains local state variables.
//
static struct {
    EDBG_ADDR edbgAddress;
    OAL_KITL_ETH_DRIVER *pDriver;
    UINT8 buffer[OAL_KITL_BUFFER_SIZE];
} g_ethState;

//------------------------------------------------------------------------------
//
//  Function:  BLEthDownload
//
//  This function initialize ethernet adapter and call download function from
//  bootloader common library.
//
UINT32
BLEthDownload(
    BOOT_CFG *pConfig, 
    OAL_KITL_DEVICE *pBootDevices
    )
{
	UNREFERENCED_PARAMETER(pConfig);
	UNREFERENCED_PARAMETER(pBootDevices);
 
    return 0;
}

//------------------------------------------------------------------------------
//
//  Function:  BLEthConfig
//
//  This function receives connect message from host and it updates arguments
//  based on it. Most part of connect message isn't used by KITL anymore, but
//  we want set passive KITL mode when no-KITL is select on PB.
//
VOID
BLEthConfig(
    BSP_ARGS *pArgs
    )
{
    EDBG_OS_CONFIG_DATA *pConfig;

    // Get host connect info
    pConfig = EbootWaitForHostConnect(&g_ethState.edbgAddress, NULL);

    pArgs->kitl.flags &= ~OAL_KITL_FLAGS_PASSIVE;
    if ((pConfig->KitlTransport & KTS_PASSIVE_MODE) != 0)
        {
        pArgs->kitl.flags |= OAL_KITL_FLAGS_PASSIVE;
        }        
}

//------------------------------------------------------------------------------
//
//  Function:  OEMEthGetFrame
//
//  Check to see if a frame has been received, and if so copy to buffer.
//  An optimization  which may be performed in the Ethernet driver is to 
//  filter out all received broadcast packets except for ARPs.
//
BOOL
OEMEthGetFrame(
    BYTE *pData, 
    UINT16 *pLength
    )
{
    return g_ethState.pDriver->pfnGetFrame(pData, pLength) > 0;
}

//------------------------------------------------------------------------------
//
//  Function:  OEMEthSendFrame
//
//  Send Ethernet frame.  
//
BOOL
OEMEthSendFrame(
    BYTE *pData, 
    DWORD length
    )
{
    return g_ethState.pDriver->pfnSendFrame(pData, length) == 0;
}

//------------------------------------------------------------------------------
//
//  Function:   BLEthReadData
//
//  This function is called to read data from the transport during
//  the download process.
//
BOOL
BLEthReadData(
    ULONG size, 
    UCHAR *pData
    )
{
    // Save read data size and location. It is used in workaround
    // for download BIN DIO images larger than RAM.
    g_eboot.readSize = size;
    g_eboot.pReadBuffer = pData;
    return EbootEtherReadData(size, pData);
}

//------------------------------------------------------------------------------

