//------------------------------------------------------------------------------
//
//  File:  args.h
//
//  This header file defines device structures and constant related to boot
//  configuration. BOOT_CFG structure defines layout of persistent device
//  information. It is used to control boot process. BSP_ARGS structure defines
//  information passed from boot loader to kernel HAL/OAL. Each structure has
//  version field which should be updated each time when structure layout
//  change.
//
#ifndef __ARGS_H
#define __ARGS_H

//------------------------------------------------------------------------------

#include <oal_args.h>
#include <oal_kitl.h>
#include <bsp_cfg.h>

//------------------------------------------------------------------------------

#define BSP_ARGS_VERSION    1
#define CALIBRATE_SIZE		120

typedef struct {
    OAL_ARGS_HEADER     header;
    BOOL 				updateMode;			// Should IPL run in update mode?
    BOOL 				coldBoot;			// Cold boot (erase registry)?
    UINT32				deviceID;			// Unique ID for development platform
    UINT32				imageLaunch;		// Image launch address
    OAL_KITL_ARGS 		kitl;				// KITL parameters
    UINT32				oalFlags;			// OAL flags
    UCHAR				DevicePrefix[18];
    WCHAR				SerialNumber[15];
    OMAP_LCD_DVI_RES	dispRes;			// display resolution
    UINT32				ECCtype;    
    UINT32				opp_mode;
    BYTE	            CalibBuffer[CALIBRATE_SIZE];	// touch calibration data
} BSP_ARGS;

//------------------------------------------------------------------------------

#endif
