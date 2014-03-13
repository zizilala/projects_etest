//
// This header file are self define, By Ray      
//
//
#ifndef __OTHER_DEF_H
#define __OTHER_DEF_H

#if __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
//  structure
//
typedef struct _OAL_MENU_ITEM_TEST{             //Ray
    WCHAR key;
    LPCWSTR text;
    VOID (*pfnAction)(struct OAL_MENU_ITEM *);
    VOID *pParam1;
    VOID *pParam2;
    int pParam3;
} OAL_MENU_ITEM_TEST;

//------------------------------------------------------------------------------
//  Function Prototype      
//
VOID OALBLMenuShowTest(OAL_BLMENU_ITEM *pMenu);          //Ray   131118

//------------------------------------------------------------------------------

#if __cplusplus
}
#endif

#endif

