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
#pragma warning(push)
#pragma warning(disable : 6385)


#include "bsp.h"
#include "bsp_cfg.h"
#include "ceddkex.h"
#include "image_cfg.h"
#include "omap_dss_regs.h"
#include "lcd_cfg.h"
#include "lcd.h"
//
//  Defines
//

typedef struct OMAP_LCD_DVI_RES_PARAMS {
    DWORD pixelFmt;
    DWORD width;
    DWORD height;
    DWORD hsw;
    DWORD hfp;
    DWORD hbp;
    DWORD vsw;
    DWORD vfp;
    DWORD vbp;
    DWORD logClkDiv;
    DWORD pixelClkDiv;
    DWORD dss1ClkSel;    
    DWORD loadMode;
    DWORD polFreq;
    DWORD lcdDefaultColor;
    DWORD lcdTransColor;
    DWORD tvDefaultColor;
    DWORD tvTransColor;    
}OMAP_LCD_DVI_RES_PARAMS;

OMAP_LCD_DVI_RES_PARAMS lcd_res_params[OMAP_RES_INVALID+1] = 
{
    {   /*OMAP_LCD_640W_480H*/     
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        480,                                //width;
        640,                                //height;
        1,                                  //hsw;
        87,                                 //hfp;
        77,                                 //hbp;
        0,                                  //vsw;
        5,                                  //vfp;
        2,                                  //vbp;
        1,                                  //logClkDiv;
        4,                                  //pixelClkDiv;
        (9 << 0),                           //dss1ClkSel;    
        0,                                  //loadMode;
        (DISPC_POL_FREQ_IVS | DISPC_POL_FREQ_IHS | DISPC_POL_FREQ_ONOFF),//polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    },
    {   /* OMAP_DVI_640W_480H */
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        640,                                //width;
        480,                                //height;
        63,                                 //hsw;
        15,                                 //hfp;
        (47 + 95 - 63),                     //hbp;
        1,                                  //vsw;
        10,                                 //vfp;
        33,                                 //vbp;
        1,                                  //logClkDiv;
        2,                                  //pixelClkDiv;
        (17 << 0),                          //dss1ClkSel;    
        0,                                  //loadMode;
        (DISPC_POL_FREQ_IVS | DISPC_POL_FREQ_IHS | DISPC_POL_FREQ_ONOFF),//polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    },
    {   /*OMAP_DVI_640W_480H_72HZ*/
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        640,                                //width;
        480,                                //height;
        39,                                 //hsw;
        23,                                 //hfp;
        127,                                //hbp;
        2,                                  //vsw;
        9,                                  //vfp;
        28,                                 //vbp;
        1 ,                                 //logClkDiv;
        4,                                  //pixelClkDiv;
        (7 << 0),                           //dss1ClkSel;    
        0,                                  //loadMode;
        (DISPC_POL_FREQ_IVS | DISPC_POL_FREQ_IHS | DISPC_POL_FREQ_ONOFF),//polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    },
    {   /*OMAP_DVI_800W_480H*/
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        800,                                //width;
        480,                                //height;
        55,                                 //hsw;
        31,                                 //hfp;
        135,                                //hbp;
        2,                                  //vsw;
        70,                                 //vfp;
        68,                                 //vbp;
        1 ,                                 //logClkDiv;
        2,                                  //pixelClkDiv;
        (11 << 0),                          //dss1ClkSel;    
        0,                                  //loadMode;
        DISPC_POL_FREQ_ONOFF,               //polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    },
    {   /* OMAP_DVI_800W_600H */
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        800,                                //width;
        600,                                //height;
        63,                                 //hsw;
        39,                                 //hfp;
        (87 + 127 - 63),                    //hbp;
        3,                                  //vsw;
        1,                                  //vfp;
        23,                                 //vbp;
        1,                                  //logClkDiv;
        2,                                  //pixelClkDiv;
        (11 << 0),                          //dss1ClkSel;    
        0,                                  //loadMode;
        DISPC_POL_FREQ_ONOFF,               //polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    },
    {   /* OMAP_DVI_800W_600H_56HZ */
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        800,                                //width;
        600,                                //height;
        63,                                 //hsw;
        23,                                 //hfp;
        (127 + 71 - 63),                    //hbp;
        1,                                  //vsw;
        1,                                  //vfp;
        22,                                 //vbp;
        1,                                  //logClkDiv;
        4,                                  //pixelClkDiv;
        (6 << 0),                           //dss1ClkSel;    
        0,                                  //loadMode;
        DISPC_POL_FREQ_ONOFF,               //polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    },
    {   /* OMAP_DVI_1024W_768H */
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        1024,                               //width;
        768,                                //height;
        63,                                 //hsw;
        23,                                 //hfp;
        (159 + 135 - 63),                   //hbp;
        5,                                  //vsw;
        3,                                  //vfp;
        29,                                 //vbp;
        1,                                  //logClkDiv;
        2,                                  //pixelClkDiv;
        (7 << 0),                           //dss1ClkSel;    
        0,                                  //loadMode;
        DISPC_POL_FREQ_ONOFF,               //polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;     
    },
    {   /* OMAP_DVI_1280W_720H */
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        1280,                               //width;
        720,                                //height;
        63,                                 //hsw;
        63,                                 //hfp;
        (191 + 127 - 63),                   //hbp;
        4,                                  //vsw;
        3,                                  //vfp;
        20,                                 //vbp;
        1,                                  //logClkDiv;
        2,                                  //pixelClkDiv;
        (6 << 0),                           //dss1ClkSel;    
        0,                                  //loadMode;
        DISPC_POL_FREQ_IHS | DISPC_POL_FREQ_ONOFF| DISPC_POL_FREQ_IPC,//polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    },
    {   /* OMAP_DVI_1280W_720H */
        DISPC_PIXELFORMAT_RGB16,            //pixelFmt;
        0,                               //width;
        0,                                //height;
        0,                                 //hsw;
        0,                                 //hfp;
        0,                   //hbp;
        0,                                  //vsw;
        0,                                  //vfp;
        0,                                 //vbp;
        0,                                  //logClkDiv;
        0,                                  //pixelClkDiv;
        0,                                  //dss1ClkSel;    
        0,                                  //loadMode;
        0,                                  //polFreq;
        0x00000000,                         //lcdDefaultColor;
        0x00000000,                         //lcdTransColor;
        0x00000000,                         //tvDefaultColor;
        0x00000000,                         //tvTransColor;    
    }                                                            
};

static OMAP_DISPC_REGS       *g_pDispRegs;

static BOOL bDVIEnabled = FALSE;

static OMAP_LCD_DVI_RES g_dwSelectedDSSResolution = OMAP_RES_INVALID;


#define DEFAULT_PIXELTYPE   lcd_res_params[g_dwSelectedDSSResolution].pixelFmt

#define LCD_WIDTH           lcd_res_params[g_dwSelectedDSSResolution].width
#define LCD_HEIGHT          lcd_res_params[g_dwSelectedDSSResolution].height

#define LCD_HSW             lcd_res_params[g_dwSelectedDSSResolution].hsw
#define LCD_HFP             lcd_res_params[g_dwSelectedDSSResolution].hfp
#define LCD_HBP             lcd_res_params[g_dwSelectedDSSResolution].hbp

#define LCD_VSW             lcd_res_params[g_dwSelectedDSSResolution].vsw
#define LCD_VFP             lcd_res_params[g_dwSelectedDSSResolution].vfp
#define LCD_VBP             lcd_res_params[g_dwSelectedDSSResolution].vbp

#define LCD_LOGCLKDIV       lcd_res_params[g_dwSelectedDSSResolution].logClkDiv
#define LCD_PIXCLKDIV       lcd_res_params[g_dwSelectedDSSResolution].pixelClkDiv

#define LCD_LOADMODE        lcd_res_params[g_dwSelectedDSSResolution].loadMode

#define LCD_POLFREQ         lcd_res_params[g_dwSelectedDSSResolution].polFreq

#define LCD_DEFAULT_COLOR   lcd_res_params[g_dwSelectedDSSResolution].lcdDefaultColor
#define LCD_TRANS_COLOR     lcd_res_params[g_dwSelectedDSSResolution].lcdTransColor

#define TV_DEFAULT_COLOR    lcd_res_params[g_dwSelectedDSSResolution].tvDefaultColor
#define TV_TRANS_COLOR      lcd_res_params[g_dwSelectedDSSResolution].tvTransColor

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL_DSS
//
//  Determines EMU clock settings.  Used to update CM_CLKSEL_DSS
//
//  Allowed values: BSP_DSS_CLKSEL_DSS1 minimum value is 5 (173 MHz) at nominal voltage
//                  BSP_DSS_CLKSEL_DSS1 minimum value is 9 (96 MHz) at low voltage
//                  BSP_DSS_CLKSEL_TV should be fixed at 16 (54 MHz)
//
//  Note: DPLL4 is 864Mhz
#define BSP_DSS_CLKSEL_DSS1 lcd_res_params[g_dwSelectedDSSResolution].dss1ClkSel
#define BSP_LCD_PIXCLKDIV   lcd_res_params[g_dwSelectedDSSResolution].pixelClkDiv
#define BSP_DSS_CLKSEL_TV   (16 << 8)    // DPLL4/16=54Mhz
#define BSP_CM_CLKSEL_DSS   (BSP_DSS_CLKSEL_TV | \
                             BSP_DSS_CLKSEL_DSS1)

void LcdPdd_LCD_InitResolution()
{
    if (g_dwSelectedDSSResolution == OMAP_RES_INVALID)
    {
        GetDisplayResolutionFromBootArgs((DWORD *)&g_dwSelectedDSSResolution);
        if ((g_dwSelectedDSSResolution >= OMAP_RES_INVALID) || (g_dwSelectedDSSResolution <0))
            g_dwSelectedDSSResolution = OMAP_LCD_DEFAULT;
    
        if (g_dwSelectedDSSResolution == OMAP_LCD_DEFAULT)
            bDVIEnabled = FALSE;
        else
            bDVIEnabled = TRUE;
        
        if (LCD_HSW > 63)
            RETAILMSG(1,(L"error LCD_HSW out of range \r\n"));
                    
        if (LCD_HFP > 255)
            RETAILMSG(1,(L"error LCD_HFP out of range \r\n"));
                    
        if (LCD_HBP > 255)
            RETAILMSG(1,(L"error LCD_HBP out of range \r\n"));
                    
        if (LCD_VSW > 63)
            RETAILMSG(1,(L"error LCD_VSW out of range \r\n"));
                    
        if (LCD_VFP > 255)
            RETAILMSG(1,(L"error LCD_VFP out of range \r\n"));
                    
        if (LCD_VBP > 255)
            RETAILMSG(1,(L"error LCD_VBP out of range \r\n"));
    }
        
}

//------------------------------------------------------------------------------
BOOL LcdPdd_LCD_Initialize(     //??
    OMAP_DSS_REGS         *pDSSRegs,
    OMAP_DISPC_REGS       *pDispRegs,
    OMAP_RFBI_REGS        *pRfbiRegs,
    OMAP_VENC_REGS        *pVencRegs
    )
{
    PHYSICAL_ADDRESS pa;
    OMAP_PRCM_DSS_CM_REGS *pPrcmDssCM;
    OALLog(L"3.XXXXXXXXXXXXXXXXXXX");
	UNREFERENCED_PARAMETER(pVencRegs);
	UNREFERENCED_PARAMETER(pRfbiRegs);
	UNREFERENCED_PARAMETER(pDSSRegs);

    LcdPdd_LCD_InitResolution();
    

    pa.QuadPart = OMAP_PRCM_DSS_CM_REGS_PA;    
    pPrcmDssCM = (OMAP_PRCM_DSS_CM_REGS*)MmMapIoSpace(pa, sizeof(OMAP_PRCM_DSS_CM_REGS), FALSE);
    if (pPrcmDssCM == NULL) return FALSE;

    // Store pointer to display controller for future use
    g_pDispRegs = pDispRegs;
    
    // Initialize GPIO  
    LcdInitGpio();      //??
    
    // setup the DSS1 clock divider - disable DSS1 clock, change divider, enable DSS clock
    OUTREG32(&pPrcmDssCM->CM_FCLKEN_DSS, INREG32(&pPrcmDssCM->CM_FCLKEN_DSS) & ~CM_CLKEN_DSS1);
    OUTREG32(&pPrcmDssCM->CM_CLKSEL_DSS, BSP_CM_CLKSEL_DSS);
    OUTREG32(&pPrcmDssCM->CM_FCLKEN_DSS, INREG32(&pPrcmDssCM->CM_FCLKEN_DSS) | CM_CLKEN_DSS1);
    
    //  LCD control
    if (bDVIEnabled)
    {
        OUTREG32( &pDispRegs->DISPC_CONTROL,
                DISPC_CONTROL_GPOUT1 |
                DISPC_CONTROL_GPOUT0 |
                DISPC_CONTROL_TFTDATALINES_24 |
                DISPC_CONTROL_STNTFT
                );
    }
    else
    {
        OUTREG32( &pDispRegs->DISPC_CONTROL,
                DISPC_CONTROL_GPOUT1 |
                DISPC_CONTROL_GPOUT0 |
                DISPC_CONTROL_TFTDATALINES_18 |
                DISPC_CONTROL_STNTFT
                );
    }
                
    //  LCD config
    OUTREG32( &pDispRegs->DISPC_CONFIG,
                DISPC_CONFIG_FUNCGATED |
                DISPC_CONFIG_LOADMODE(LCD_LOADMODE) |
                DISPC_CONFIG_PALETTEGAMMATABLE
                );

    // Default Color
    OUTREG32( &pDispRegs->DISPC_DEFAULT_COLOR0,
                LCD_DEFAULT_COLOR
                );

    // Default Transparency Color
    OUTREG32( &pDispRegs->DISPC_TRANS_COLOR0,
                LCD_TRANS_COLOR
                );

    // Timing logic for HSYNC signal
    OUTREG32( &pDispRegs->DISPC_TIMING_H,
                DISPC_TIMING_H_HSW(LCD_HSW) |
                DISPC_TIMING_H_HFP(LCD_HFP) |
                DISPC_TIMING_H_HBP(LCD_HBP)
                );

    // Timing logic for VSYNC signal
    OUTREG32( &pDispRegs->DISPC_TIMING_V,
                DISPC_TIMING_V_VSW(LCD_VSW) |
                DISPC_TIMING_V_VFP(LCD_VFP) |
                DISPC_TIMING_V_VBP(LCD_VBP)
                );

    // Signal configuration
    OUTREG32( &pDispRegs->DISPC_POL_FREQ,
                LCD_POLFREQ
                );

    // Configures the divisor
    OUTREG32( &pDispRegs->DISPC_DIVISOR,
                DISPC_DIVISOR_PCD(LCD_PIXCLKDIV) |
                DISPC_DIVISOR_LCD(LCD_LOGCLKDIV)
                );

    // Configures the panel size
    OUTREG32( &pDispRegs->DISPC_SIZE_LCD,
                DISPC_SIZE_LCD_LPP(LCD_HEIGHT) |
                DISPC_SIZE_LCD_PPL(LCD_WIDTH)
                );

    MmUnmapIoSpace((VOID*)pPrcmDssCM, sizeof(OMAP_PRCM_DSS_CM_REGS));


    // Set initial power level
    LcdPdd_SetPowerLevel(D0);
  
    return TRUE;
}

//------------------------------------------------------------------------------
BOOL
LcdPdd_TV_Initialize(
    OMAP_DSS_REGS       *pDSSRegs,
    OMAP_DISPC_REGS     *pDispRegs,
    OMAP_RFBI_REGS      *pRfbiRegs,
    OMAP_VENC_REGS      *pVencRegs
    )
{
	UNREFERENCED_PARAMETER(pVencRegs);
	UNREFERENCED_PARAMETER(pRfbiRegs);

    //  Enable TV out DSS control settings
    
#if BSP_TVOUT_COMPOSITE 
    SETREG32( &pDSSRegs->DSS_CONTROL,
                DSS_CONTROL_DAC_POWERDN_BGZ |
                DSS_CONTROL_DAC_DEMEN |
                DSS_CONTROL_VENC_CLOCK_4X_ENABLE |
                DSS_CONTROL_VENC_CLOCK_MODE_0
                );
    CLRREG32( &pDSSRegs->DSS_CONTROL,
                DSS_CONTROL_DAC_VENC_OUT_SEL            //Enable Composite TVout
            );
#else
    SETREG32( &pDSSRegs->DSS_CONTROL,
                DSS_CONTROL_DAC_VENC_OUT_SEL |          // Enables S-video
                DSS_CONTROL_DAC_POWERDN_BGZ |
                DSS_CONTROL_DAC_DEMEN |
                DSS_CONTROL_VENC_CLOCK_4X_ENABLE |
                DSS_CONTROL_VENC_CLOCK_MODE_0
                );
#endif              


    // Default Color
    OUTREG32( &pDispRegs->DISPC_DEFAULT_COLOR1,
                TV_DEFAULT_COLOR
                );

    // Default Transparency Color
    OUTREG32( &pDispRegs->DISPC_TRANS_COLOR1,
                TV_TRANS_COLOR
                );


#if BSP_TVOUT_PAL
    // Configures the TV size for PAL interlaced (half height)
    // the LPP is programmed to (PAL_HEIGHT+2)/2 for interlaced mode
    OUTREG32(   &pDispRegs->DISPC_SIZE_DIG,
                DISPC_SIZE_DIG_LPP((PAL_HEIGHT+2)/2) |
                DISPC_SIZE_DIG_PPL(PAL_WIDTH)
                );
#else
    // Configures the TV size for NTSC interlaced (half height)
    // the LPP is programmed to (NTSC_HEIGHT+2)/2 for interlaced mode
    OUTREG32( &pDispRegs->DISPC_SIZE_DIG,
                DISPC_SIZE_DIG_LPP((NTSC_HEIGHT+2)/2) |
                DISPC_SIZE_DIG_PPL(NTSC_WIDTH)
                );
#endif

    return TRUE;
}


//------------------------------------------------------------------------------
BOOL
LcdPdd_GetMemory(
    DWORD   *pVideoMemLen,
    DWORD   *pVideoMemAddr
    )
{
    //  Return video memory parameters
    if( pVideoMemLen )
        *pVideoMemLen = IMAGE_WINCE_DISPLAY_SIZE;

    if( pVideoMemAddr )
    {
        DWORD videoCAAddr = IMAGE_WINCE_DISPLAY_CA;
        *pVideoMemAddr = ConvertCAtoPA((DWORD *)videoCAAddr);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
BOOL
LcdPdd_LCD_GetMode(
    DWORD   *pPixelFormat,
    DWORD   *pWidth,
    DWORD   *pHeight,
    DWORD   *pPixelClock
    )
{

    LcdPdd_LCD_InitResolution();        
    
    //  Return default LCD parameters
    if( pPixelFormat )
        *pPixelFormat = DEFAULT_PIXELTYPE;

    if( pWidth )
        *pWidth = LCD_WIDTH;

    if( pHeight )
        *pHeight = LCD_HEIGHT;

    if ( pPixelClock )
        *pPixelClock = (864000000 / BSP_DSS_CLKSEL_DSS1) / BSP_LCD_PIXCLKDIV;

    return TRUE;
}

//------------------------------------------------------------------------------
BOOL
LcdPdd_TV_GetMode(
    DWORD   *pWidth,
    DWORD   *pHeight,
    DWORD   *pMode
    )
{
    //  Return default TV parameters
    if( pWidth )
        #if BSP_TVOUT_PAL
            *pWidth = PAL_WIDTH;
        #else
            *pWidth = NTSC_WIDTH;
        #endif
            
    if( pHeight )
        #if BSP_TVOUT_PAL
            *pHeight = PAL_HEIGHT;
        #else
            *pHeight = NTSC_HEIGHT;
        #endif

    if( pMode )
    {
        #if BSP_TVOUT_COMPOSITE
            *pMode = TVOUT_COMPOSITE;
        #else
            *pMode = TVOUT_SVIDEO;
        #endif
    }
        
    return TRUE;
}


//------------------------------------------------------------------------------
BOOL
LcdPdd_SetPowerLevel(
    DWORD   dwPowerLevel
    )
{
    // Do nothing if LCD has not yet been initialized
    if (g_pDispRegs == NULL)
        return FALSE;
    OALLog(L"22.XXXXXXXXXXXXXXXXXXX");    
    // Power display up/down
    switch(dwPowerLevel)
    {
        case D0:
        case D1:
        case D2:
            
            if (bDVIEnabled)
            {
                // change output format to 24 bit
                OUTREG32( &g_pDispRegs->DISPC_CONTROL,
                        DISPC_CONTROL_GPOUT1 |
                        DISPC_CONTROL_GPOUT0 |
                        DISPC_CONTROL_TFTDATALINES_24 |
                        DISPC_CONTROL_STNTFT
                        );

                // Apply display configuration
                SETREG32(&g_pDispRegs->DISPC_CONTROL, DISPC_CONTROL_GOLCD);

                // start scanning
                
                if (dwPowerLevel == D2)
                    CLRREG32( &g_pDispRegs->DISPC_CONTROL, DISPC_CONTROL_LCDENABLE );        
                else
                    SETREG32( &g_pDispRegs->DISPC_CONTROL, DISPC_CONTROL_LCDENABLE );        
                LcdSleep(20);
                // enable DVI
                LcdDviEnableControl(TRUE);
            }
            else
            {
                // change output format to 18 bit
                OUTREG32( &g_pDispRegs->DISPC_CONTROL,
                        DISPC_CONTROL_GPOUT1 |
                        DISPC_CONTROL_GPOUT0 |
                        DISPC_CONTROL_TFTDATALINES_18 |
                        DISPC_CONTROL_STNTFT
                        );

                // Apply display configuration
                SETREG32(&g_pDispRegs->DISPC_CONTROL, DISPC_CONTROL_GOLCD);

                // LCD power enable sequence
                // 1 - Turn on power
                LcdPowerControl(TRUE);
                        
                // 2 - Start scanning
                if (dwPowerLevel == D2)
                    CLRREG32( &g_pDispRegs->DISPC_CONTROL, DISPC_CONTROL_LCDENABLE );        
                else
                    SETREG32( &g_pDispRegs->DISPC_CONTROL, DISPC_CONTROL_LCDENABLE );        
    
            
                // 3 - Delay for minimum 20uS
                LcdStall(50);
            
                // 4 - Raise RESB
                LcdResBControl(TRUE);
            
                LcdStall(10);
            
                // 5 - Raise INI
                LcdIniControl(TRUE);
            }
            break;        

        case D3:
        case D4:
            // disable DVI
            LcdDviEnableControl(FALSE);

            // LCD power disable sequence
            // 1 - Clear INI
            LcdIniControl(FALSE);
                
            // 2 - Delay > 5 frames
            LcdSleep(20*5);
            
            // 3 - Stop all
            CLRREG32( &g_pDispRegs->DISPC_CONTROL, DISPC_CONTROL_LCDENABLE );        
            LcdResBControl(FALSE);
            LcdPowerControl(FALSE);
            break;
    }
        
    return TRUE;
}   

//------------------------------------------------------------------------------
BOOL
LcdPdd_DVI_Select(
    BOOL bEnable
    )
{

    // only changes the select variable, does not perform the change
    bDVIEnabled = bEnable;
    return TRUE;
}

//------------------------------------------------------------------------------
BOOL
LcdPdd_DVI_Enabled(void)
{
    LcdPdd_LCD_InitResolution();
    return bDVIEnabled;
}

//------------------------------------------------------------------------------
DWORD
LcdPdd_Get_PixClkDiv(void)
{
    LcdPdd_LCD_InitResolution();
    return LCD_PIXCLKDIV;
}

#pragma warning(pop)


