/*-----------------------------------------------------------------------------
	Module Name: bkl_Ctls.h

	Abstract: Commands accessible through DeviceIoControl call for the backlight driver.

	Revision History:
 				2013.09.30 Release.
-----------------------------------------------------------------------------*/

#ifndef __bkl_Ctls_H__
#define __bkl_Ctls_H__

#ifdef LIB_INTERNAL
#define EXTCODE
#else
      #ifdef __cplusplus
          #define EXTCODE extern "C"	
      #else
          #define EXTCODE extern
      #endif 

#endif

#define  BACKLIGHT_REGKEY				TEXT("SOFTWARE\\Apps\\Backlight")
#define  BACKLIGHT_REGVALUE_LEVEL		TEXT("Brightness")
#define  BACKLIGHT_REGVALUE_MODE		TEXT("LCD Mode")
#define  BACKLIGHT_REGVALUE_TIMEOUT		TEXT("Timeout")
#define  KEYPADBACKLIGHT_REGVALUE_MODE	TEXT("Keypad Mode")

#endif  // __bkl_Ctls_H__
