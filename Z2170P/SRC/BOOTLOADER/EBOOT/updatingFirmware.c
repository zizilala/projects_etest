//------------------------------------------------------------------------------
//  File:  updatingFirmware.c , by Ray
//
//
#include <eboot.h>
#include "bsp.h"
 


VOID UpdatingGaugeFW(OAL_BLMENU_ITEM *pMenu)
{
    DWORD size = 10000;
    UCHAR *pData = NULL;
    UNREFERENCED_PARAMETER(pMenu);
    OALBLMenuHeader(L"Updating Fuel Gauge Firmware");
            
    if (!SDCardUpdatingFW(L"test.bmp", pData, size))
    //if (!BLSDCardReadLogo(L"test.dffs", pData, size))
    {
        OALMSG(OAL_ERROR, (L"Update fuel gauge firmware wrong!!\r\n"));
    }
}





