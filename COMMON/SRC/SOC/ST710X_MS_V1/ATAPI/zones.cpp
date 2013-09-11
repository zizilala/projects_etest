//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//=======================================================================
//  COPYRIGHT (C) STMicroelectronics 2007.  ALL RIGHTS RESERVED
//
//  Use of this source code is subject to the terms of your STMicroelectronics
//  development license agreement. If you did not accept the terms of such a license,
//  you are not authorized to use this source code.
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//========================================================================
#include <windows.h>
#include <debug.h>

DBGPARAM dpCurSettings = {
    TEXT("ATAPI"), {
    TEXT("Init"),TEXT("Deinit"),TEXT("Main"),TEXT("I/O"),
    TEXT("PCMCIA"),TEXT("PCI"),TEXT("IOCTL"),TEXT("CDROM"),
    TEXT("DMA"),TEXT("Power"),TEXT("Undefined"),TEXT("Undefined"),
    TEXT("Warning"),TEXT("Error"),TEXT("Helper"), TEXT("CELOG") },
#if 0
    ZONEMASK_IOCTL |
    ZONEMASK_DMA |
    ZONEMASK_INIT |
    ZONEMASK_HELPER |
    ZONEMASK_DEINIT |
    ZONEMASK_PCMCIA |
    ZONEMASK_PCI |
    ZONEMASK_MAIN |
    ZONEMASK_IO |
    ZONEMASK_CDROM |
    ZONEMASK_ERROR |
    ZONEMASK_CELOG
#else
    ZONEMASK_ERROR |
    ZONEMASK_WARNING |
    ZONEMASK_INIT
#endif
};

