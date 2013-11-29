// All rights reserved ADENEO EMBEDDED 2010
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
//  Header:  bsp_logo.h
//
#ifndef __BSP_LOGO_H
#define __BSP_LOGO_H


//------------------------------------------------------------------------------
//
//  Function:  ShowLogo
//
//  This function shows the logo splash screen
//
VOID ShowLogo(UINT32 flashAddr, UINT32 offset);

//------------------------------------------------------------------------------
//
//VOID ShowTest(UINT32 flashAddr, UINT32 offset);               //Ray 131106
VOID ShowTest(UINT32 flashAddr, UINT32 offset/*,int board*/);   //Ray 131115
//VOID Initial_lcd_TSC2046(void);                                 //Ray 131119
VOID detect_TSC2046(void);                                      //Ray 131126




//------------------------------------------------------------------------------
//
//  Function:  ShowSDLogo
//
//   This function is called to display the splaschreen bitmap from the SDCard
//
BOOL ShowSDLogo();

//------------------------------------------------------------------------------
//
//  Function:  HideLogo
//
//  This function hides the logo splash screen
//
VOID HideLogo(VOID);

#endif // __BSP_LOGO_H
