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
//  File:  menu.c
//
#include <eboot.h>
#include "omap_cpuver.h"

//------------------------------------------------------------------------------
//
//  Define:  dimof
//
#ifdef dimof
#undef dimof
#endif
#define dimof(x) (sizeof(x)/sizeof(x[0]))
#define BSP_Z2000  2000
#define BSP_Z2170P 2170 

//------------------------------------------------------------------------------

static VOID SetMacAddress(OAL_BLMENU_ITEM *pMenu);
static VOID ShowSettings(OAL_BLMENU_ITEM *pMenu);
//static VOID ShowNetworkSettings(OAL_BLMENU_ITEM *pMenu);
//static VOID SetKitlMode(OAL_BLMENU_ITEM *pMenu);
static VOID SetKitlType(OAL_BLMENU_ITEM *pMenu);
static VOID SetDeviceID(OAL_BLMENU_ITEM *pMenu);
static VOID SaveSettings(OAL_BLMENU_ITEM *pMenu);
static VOID SetRetailMsgMode(OAL_BLMENU_ITEM *pMenu);
//static VOID SetDisplayResolution(OAL_BLMENU_ITEM *pMenu);
static VOID SetOPPmode(OAL_BLMENU_ITEM *pMenu);
//static VOID FunctionTest(OAL_BLMENU_ITEM *pMenu);     //Ray   131106



#if BUILDING_EBOOT_SD
static VOID ShowSDCardSettings(OAL_BLMENU_ITEM *pMenu);
static VOID EnterSDCardFilename(OAL_BLMENU_ITEM *pMenu);
#endif

//------------------------------------------------------------------------------

// Functions
extern BOOL LAN911XFindController(void* pAddr, DWORD * pdwID);
extern BOOL LAN911XSetMacAddress(void* pAddr, UINT16 mac[3]);
extern BOOL LAN911XGetMacAddress(void* pAddr, UINT16 mac[3]);

extern OAL_BLMENU_ITEM g_menuFlash[];
//------------------------------------------------------------------------------

//extern OAL_BLMENU_ITEM g_menuTest[];            //Ray   131106 
extern OAL_BLMENU_ITEM g_menu2170PTest[];       //Ray   131115  
extern OAL_BLMENU_ITEM g_menuZ2000Test[];     //Ray   131115 


//------------------------------------------------------------------------------
/*
static OAL_BLMENU_ITEM g_menuNetwork[] = {
    {
        L'1', L"Show Current Settings", ShowNetworkSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Enable/disable KITL", OALBLMenuEnable,
        L"KITL", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_ENABLED
    }, {
        L'3', L"KITL interrupt/poll mode", SetKitlMode,
        NULL, NULL, NULL
    }, {
        L'4', L"KITL Active/Passive mode", SetKitlType,
        NULL, NULL, NULL
    }, {
        L'5', L"Enable/disable DHCP", OALBLMenuEnable,
        L"DHCP", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_DHCP
    }, {
        L'6', L"Set IP address", OALBLMenuSetIpAddress,
        L"Device", &g_bootCfg.ipAddress, NULL
    }, {
        L'7', L"Set IP mask", OALBLMenuSetIpMask,
        L"Device", &g_bootCfg.ipMask, NULL
    }, {
        L'8', L"Set default router", OALBLMenuSetIpAddress,
        L"Default router", &g_bootCfg.ipRoute, NULL
    }, {
        L'9', L"Enable/disable VMINI", OALBLMenuEnable,
        L"VMINI", &g_bootCfg.kitlFlags, (VOID*)OAL_KITL_FLAGS_VMINI
    }, {
        L'a', L"Set MAC address", SetMacAddress,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};*/

#if BUILDING_EBOOT_SD
static OAL_BLMENU_ITEM g_menuSDCard[] = {
    {
        L'1', L"Show Current Settings", ShowSDCardSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Enter Filename", EnterSDCardFilename,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#endif

#if BUILDING_EBOOT_SD
static OAL_BLMENU_ITEM g_menuMain[] = {
    {
        L'1', L"Show Current Settings", ShowSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Select Boot Device", OALBLMenuSelectDevice,
        L"Boot", &g_bootCfg.bootDevLoc, g_bootDevices
    }, {
        L'3', L"SDCard Settings", OALBLMenuShow,
        L"SDCard Settings", &g_menuSDCard, NULL
    }, {
        L'4', L"Set S/N", SetDeviceID,
        NULL, NULL, NULL
    }, {
        L'5', L"Save Settings", SaveSettings,
        NULL, NULL, NULL
    }, {
        L'6', L"Flash Management", OALBLMenuShow,
        L"Flash Management", &g_menuFlash, NULL
    },  {
        L'7', L"Enable/Disable OAL Retail Messages", SetRetailMsgMode,
        NULL, NULL, NULL
    }, {
        L'8', L"Select OPP Mode", SetOPPmode,
        NULL, NULL, NULL
    },/* {
        L'9', L"Function Test", OALBLMenuShow,           //Ray 131106
        L"Function Test Management", &g_menuTest, NULL
    },*/ {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#else
static OAL_BLMENU_ITEM g_menuMain[] = {
    {
        L'1', L"Show Current Settings", ShowSettings,
        NULL, NULL, NULL
    }, {
        L'2', L"Flash Management", OALBLMenuShow,
        L"Flash Management", &g_menuFlash, NULL
    }, {
        L'3', L"Set S/N", SetDeviceID,
        NULL, NULL, NULL
    }, {
        L'4', L"Save Settings", SaveSettings,
        NULL, NULL, NULL
    }, {
        L'5', L"Enable/Disable OAL Retail Messages", SetRetailMsgMode,
        NULL, NULL, NULL
    }, {
        L'6', L"Test", NULL,
        NULL, NULL, NULL
    }, {
        L'7', L"Select OPP Mode", SetOPPmode,
        NULL, NULL, NULL
    },/*  {
        L'8', L"Function Test", OALBLMenuShow,           //Ray 131106
        L"Function Test Management", &g_menuTest, NULL
    },*/ {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    }, {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};
#endif

static OAL_BLMENU_ITEM g_menuRoot = {
    0, NULL, OALBLMenuShow,
    L"Main Menu", g_menuMain, NULL
};



//-----------------------------------------------------------------------------
//  Function test menu, Ray 131115
//

static OAL_MENU_ITEM_TEST g_menuBoardTest[] ={
    {
        L'1', L"Z2000 Device", OALBLMenuShow,
        L"Z2000 E-Test Management", &g_menuZ2000Test, BSP_Z2000
    }, {
        L'2', L"Z2170P Device", OALBLMenuShow,
        L"Z2170P E-Test Management", &g_menu2170PTest, BSP_Z2170P
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, 0
    }, {
        0, NULL, NULL,
        NULL, NULL, 0
    }
};

static OAL_BLMENU_ITEM g_menuBoard = {
    0, NULL, OALBLMenuShow,
    L"Selection Devices", g_menuBoardTest, NULL
};

#define DEFINE_LOGO g_menuBoardTest->pParam3    //DEFINE_LOGO are has Value 


//------------------------------------------------------------------------------
//  Ray 131118
/*VOID OALBLMenuShowTest(OAL_BLMENU_ITEM *pMenu)
{
    LPCWSTR title = pMenu->pParam1;
    OAL_BLMENU_ITEM *aMenu = pMenu->pParam2, 
                    *pItem;
    WCHAR key;
    BOOL run = TRUE; 
   
    while (run) 
    {
        OALBLMenuHeader(L"%s", title);

        // Print menu items
        for (pItem = aMenu; pItem->key != 0; pItem++) {
            OALLog(L" [%c] %s\r\n", pItem->key, pItem->text);
        }
        OALLog(L"\r\n Selection: ");

        while (run)
        {
            // Get key
            key = OALBLMenuReadKey(TRUE);
            // Look for key in menu
            for (pItem = aMenu; pItem->key != 0; pItem++) {
                if (pItem->key == key) break;
            }
            // Print out selection character
            OALLog(L"%c\r\n", key);
                
            // If we find it, break loop
            if (pItem->key != 0) break;
        }
                 
        // When action is NULL return back to parent menu
        if (pItem->pfnAction == NULL) break;
        
        // Else call menu action
        pItem->pfnAction(pItem); 
        
        
    }     
}*/


//-----------------------------------------------------------------------------

typedef struct OMAP_LCD_DVI_RES_MENU {
    DWORD       lcdDviRes;
    LPCWSTR     resName;
} OMAP_LCD_DVI_RES_MENU;

OMAP_LCD_DVI_RES_MENU dispResMenu[] = {
    {OMAP_LCD_DEFAULT,          L"LCD  480x640 60Hz"},
    {OMAP_DVI_640W_480H,        L"DVI  640x480 60Hz"},
    {OMAP_DVI_800W_600H,        L"DVI  800x600 60Hz"},
    {OMAP_DVI_1024W_768H,       L"DVI 1024x768 60Hz"},
    {OMAP_DVI_1280W_720H,       L"DVI 1280x720 60Hz"},
    {OMAP_RES_INVALID,          L"Invalid Entry"},
};

extern UINT32 g_CPUFamily;

typedef struct OPP_MODE_MENU {
    LPCWSTR     oppModeName;
} OPP_MODE_MENU;

OPP_MODE_MENU oppModeMenu35x[OMAP35x_OPP_NUM] = {
    {L"MPU[125Mhz @ 0.975V], IVA2[ 90Mhz @ 0.975V]"},
    {L"MPU[250Mhz @ 1.000V], IVA2[180Mhz @ 1.00V]"},
    {L"MPU[500Mhz @ 1.2000V], IVA2[360Mhz @ 1.20V]"},
    {L"MPU[550Mhz @ 1.2750V], IVA2[400Mhz @ 1.27V]"},
    {L"MPU[600Mhz @ 1.3500V], IVA2[430Mhz @ 1.35V]"},
    {L"MPU[720hz @ 1.3500V], IVA2[520Mhz @ 1.35V]"},
};

OPP_MODE_MENU oppModeMenu37x[OMAP37x_OPP_NUM] = {
     {L"MPU[300Mhz  @ 0.9375V], IVA2[260Mhz @ 0.9375V]"},
     {L"MPU[600Mhz  @ 1.1000V], IVA2[520Mhz @ 1.1000V]"},
     {L"MPU[800Mhz  @ 1.2625V], IVA2[660Mhz @ 1.2625V]"},
     {L"MPU[1000Mhz @ 1.375V], IVA2[800Mhz @ 1.375V]"},
};

//-----------------------------------------------------------------------------
VOID BLMenu(BOOL bForced)
{
    UINT32 time, delay = 6;
    WCHAR key = 0;
    BSP_ARGS *pArgs = OALCAtoUA(IMAGE_SHARE_ARGS_CA);
    
    // First let user break to menu
    while (!bForced && (delay > 0 && key != L' ')) 
	{
		OALLog(L"Hit space to enter configuration menu %d...\r\n", delay);
		time = OALGetTickCount();
		while ((OALGetTickCount() - time) < 1000) 
		{
			if ((key = OALBLMenuReadKey(FALSE)) == L' ') 
				break;
		}
		delay--;
	}
    
    if ((bForced == TRUE) || (key == L' ')) 
    {
        //OALBLMenuShow(&g_menuRoot);       
        OALBLMenuShow(&g_menuBoard);        //Ray 131115 
  
        // Invalidate arguments to force them to be reinitialized
        // with new config data generated by the boot menu
        pArgs->header.signature = 0;
    }        
}
//-----------------------------------------------------------------------------
VOID SetMacAddress(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);  
}

//------------------------------------------------------------------------------

VOID ShowSettings(OAL_BLMENU_ITEM *pMenu)
{
    OPP_MODE_MENU * pOppMenu = oppModeMenu35x;
    UNREFERENCED_PARAMETER(pMenu);
    
    if(g_CPUFamily == CPU_FAMILY_DM37XX)
    {
        pOppMenu = oppModeMenu37x;
    }
    
    OALLog(L"\r\n Main:\r\n");
    OALLog(
        L"  Boot device:   %s\r\n",
        OALKitlDeviceName(&g_bootCfg.bootDevLoc, g_bootDevices)
    );
    OALLog(
        L"  Debug device:  %s\r\n",
        (g_bootCfg.kitlDevLoc.LogicalLoc) ? OALKitlDeviceName(&g_bootCfg.kitlDevLoc, g_kitlDevices) : L"not specified"
    );
    OALLog(
        L"  Retail Msgs:   %s\r\n",
        (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) ? L"enabled" : L"disabled"
    );
    OALLog(
        L"  S/N:     %s\r\n", g_bootCfg.SerialNumber
    );
    /*OALLog(
        L"  Display Res:   %s\r\n", dispResMenu[g_bootCfg.displayRes].resName
    );*/
    OALLog(
        L"  Flashing NK.bin:   %s\r\n", 
        (g_bootCfg.flashNKFlags & ENABLE_FLASH_NK) ? L"enabled" : L"disabled"
    );    
    OALLog(
        L"  OPP Mode:   %s\r\n", pOppMenu[g_bootCfg.opp_mode].oppModeName
    );
	
#if BUILDING_EBOOT_SD
    ShowSDCardSettings(pMenu);
#endif
    //ShowNetworkSettings(pMenu);
}

//------------------------------------------------------------------------------
/*
VOID ShowNetworkSettings(OAL_BLMENU_ITEM *pMenu)
{
    BOOL fValidExtMacAddr;
    UINT16 mac[3];
    UNREFERENCED_PARAMETER(pMenu);

    memset(mac,0xFF,sizeof(mac));
    LAN911XGetMacAddress(OALPAtoUA((DWORD) BSP_LAN9115_REGS_PA),mac);
    if (((mac[0]== 0xFFFF) && (mac[1] == 0xFFFF) && (mac[2] == 0xFFFF)) || ((mac[0]== 0x0) && (mac[1] == 0x0) && (mac[2] == 0x0)))
    {
        fValidExtMacAddr = FALSE;        
    }
    else
    {
        fValidExtMacAddr = TRUE;        
    }


    OALLog(L"\r\n Network:\r\n");
    OALLog(
        L"  KITL state:    %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_ENABLED) ? L"enabled" : L"disabled"
    );

    OALLog(
        L"  KITL type:     %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) ? L"passive" : L"active"
    );

    OALLog(
        L"  KITL mode:     %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) ? L"polled" : L"interrupt"
    );
    OALLog(
        L"  DHCP:          %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_DHCP) ? L"enabled" : L"disabled"
    );
    OALLog(L"  IP address:    %s\r\n", OALKitlIPtoString(g_bootCfg.ipAddress));
    OALLog(L"  IP mask:       %s\r\n", OALKitlIPtoString(g_bootCfg.ipMask));
    OALLog(L"  IP router:     %s\r\n", OALKitlIPtoString(g_bootCfg.ipRoute));
    OALLog(L"  Eth MAC Addr:  %s (%s)\r\n", 
        OALKitlMACtoString(fValidExtMacAddr ? mac : g_bootCfg.mac)
        ,fValidExtMacAddr ? L"EEPROM" : L"Boot settings");
    OALLog(
        L"  VMINI:         %s\r\n",
        (g_bootCfg.kitlFlags & OAL_KITL_FLAGS_VMINI) ? L"enabled" : L"disabled"
    );
    OALLog(L"  Note: USBFN RNDIS MAC Addr cannot be changed.\r\n");
    
    OALBLMenuReadKey(TRUE);
}
*/
//------------------------------------------------------------------------------
/*
VOID SetKitlMode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) != 0) {
        OALLog(L" Set KITL to interrupt mode [y/-]: ");
    } else {
        OALLog(L" Set KITL to polled mode [y/-]: ");
    }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') {
        if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_POLL) != 0) {
            g_bootCfg.kitlFlags &= ~OAL_KITL_FLAGS_POLL;
            OALLog(L" KITL set to interrupt mode\r\n");
        } else {
            g_bootCfg.kitlFlags |= OAL_KITL_FLAGS_POLL;
            OALLog(L" KITL set to polled mode\r\n");
        }            
    }
}
*/
//------------------------------------------------------------------------------

VOID
SetKitlType(
    OAL_BLMENU_ITEM *pMenu
    )
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) != 0) 
        {
        OALLog(L" Set KITL to active mode [y/-]: ");
        } 
   else 
        {
        OALLog(L" Set KITL to passive mode [y/-]: ");
        }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') 
        {
        if ((g_bootCfg.kitlFlags & OAL_KITL_FLAGS_PASSIVE) != 0) 
            {
            g_bootCfg.kitlFlags &= ~OAL_KITL_FLAGS_PASSIVE;
            OALLog(L" KITL set to active mode\r\n");
            }
        else 
            {
            g_bootCfg.kitlFlags |= OAL_KITL_FLAGS_PASSIVE;
            OALLog(L" KITL set to passive mode\r\n");
            }            
        }
}

//------------------------------------------------------------------------------
VOID SetDeviceID(OAL_BLMENU_ITEM *pMenu)
{
    //WCHAR  szInputLine[16];
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L" Current S/N:  %s\r\n", g_bootCfg.SerialNumber);

    OALLog(L"\r\n New S/N: ");

    if (OALBLMenuReadLine(g_bootCfg.SerialNumber, dimof(g_bootCfg.SerialNumber)) == 0) 
	{
        goto cleanUp;
	}

    // Get device ID
	// g_bootCfg.deviceID = OALStringToUINT32(szInputLine);

cleanUp:
    return;
}

//------------------------------------------------------------------------------

VOID SetRetailMsgMode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;

    UNREFERENCED_PARAMETER(pMenu);

    if (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) {
        OALLog(L" Disable OAL Retail Messages [y/-]: ");
    } else {
        OALLog(L" Enable OAL Retail Messages [y/-]: ");
    }    

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    if (key == L'y' || key == L'Y') {
        if (g_bootCfg.oalFlags & BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE) 
		{
            g_bootCfg.oalFlags &= ~BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE;
            OALLog(L" OAL Retail Messages disabled\r\n");
        }
		else 
		{
            g_bootCfg.oalFlags |= BOOT_CFG_OAL_FLAGS_RETAILMSG_ENABLE;
            OALLog(L" OAL Retail Messages enabled\r\n");
        }    
    }
}

//------------------------------------------------------------------------------
/*
VOID SetDisplayResolution(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UINT32 i;

    UNREFERENCED_PARAMETER(pMenu);
    
    OALBLMenuHeader(L"Select Display Resolution");

    for (i=0; i<OMAP_RES_INVALID; i++)
    {
        OALLog(L" [%d] %s\r\n", i+1 , dispResMenu[i].resName);
    }
    OALLog(L" [0] Exit and Continue\r\n");

    OALLog(L"\r\n Selection (actual %s): ", dispResMenu[g_bootCfg.displayRes].resName);
    // Get key
    do {
        key = OALBLMenuReadKey(TRUE);
    } while (key < L'0' || key > L'0' + i);    
    OALLog(L"%c\r\n", key);
    
    // If user select exit don't change device
    if (key == L'0') return;

    g_bootCfg.displayRes = (key - L'0' - 1);

}
*/

//------------------------------------------------------------------------------

VOID SetOPPmode(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UINT32 i;
    UINT32 len = OMAP35x_OPP_NUM;
    OPP_MODE_MENU * pOppMenu = oppModeMenu35x;

    UNREFERENCED_PARAMETER(pMenu);
    
    OALBLMenuHeader(L"Select OPP Mode");

    
    if(g_CPUFamily == CPU_FAMILY_DM37XX)
    {
        len = OMAP37x_OPP_NUM;
        pOppMenu = oppModeMenu37x;
    }
    
    for (i=0; i<len; i++)
    {
        OALLog(L" [%d] %s\r\n", i+1 , pOppMenu[i].oppModeName);
    }
    OALLog(L" [0] Exit and Continue\r\n");

    OALLog(L"\r\n Selection (actual [%s]): ", pOppMenu[g_bootCfg.opp_mode].oppModeName);
    
    // Get key
    do {
        key = OALBLMenuReadKey(TRUE);
    } while (key < L'0' || key > L'0' + i); //(key < 0 or key > 0+4) are true;continues for running do loop, Ray 131111    
    OALLog(L"%c\r\n", key);
    
    // If user select exit don't change device
    if (key == L'0') return;

    g_bootCfg.opp_mode = (key - L'0' -1);

}

//------------------------------------------------------------------------------
//Ray 131106
/*VOID FunctionTest(OAL_BLMENU_ITEM *pMenu)
{
    
}*/

//------------------------------------------------------------------------------

VOID SaveSettings(OAL_BLMENU_ITEM *pMenu)
{
    WCHAR key;
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L" Do you want save current settings [-/y]? ");

    // Get key
    key = OALBLMenuReadKey(TRUE);
    OALLog(L"%c\r\n", key);

    // Depending on result
    if (key != L'y' && key != L'Y') goto cleanUp;

    if (BLWriteBootCfg(&g_bootCfg)) {
        OALLog(L" Current settings has been saved\r\n");
    } else {        
        OALLog(L"ERROR: Settings save failed!\r\n");
    }

cleanUp:
    return;
}

//------------------------------------------------------------------------------

UINT32 OALStringToUINT32(LPCWSTR psz)
{
    UINT32 i = 0;

    // Replace the dots with NULL terminators
    while (psz != NULL && *psz != L'\0') 
        {
        if (*psz < L'0' || *psz > L'9') 
            {
            break;
            }
        i = i * 10 + (*psz - L'0');
        psz++;
        }

    return i;
}

#if BUILDING_EBOOT_SD

//------------------------------------------------------------------------------

VOID ShowSDCardSettings(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n SDCard:\r\n");
    OALLog(
        L"  Filename:      \"%s\"\r\n", g_bootCfg.filename
    );
//  OALBLMenuReadKey(TRUE);
}

//------------------------------------------------------------------------------
#define dimof(x)                (sizeof(x)/sizeof(x[0]))

VOID EnterSDCardFilename(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);

    OALLog(L"\r\n Type new filename (8.3 format) :");
    OALBLMenuReadLine(g_bootCfg.filename, dimof(g_bootCfg.filename));
}

//------------------------------------------------------------------------------

#endif
