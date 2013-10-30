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
//  File:  bsp_logo.c
//


//------------------------------------------------------------------------------
//
// includes
//

#include "bsp.h"
#include "bsp_logo.h"
#include "omap_dss_regs.h"
#include "lcd.h"
#include "oalex.h"
#include "eboot.h"
#include "sdk_gpio.h"

#include "twl.h"
#include "triton.h"
#include "tps659xx_internals.h"
//------------------------------------------------------------------------------
//
// prototypes
//
void reset_display_controller( void );
void disable_dss( void );
UINT32 enable_lcd_power( void );
UINT32 enable_lcd_backlight( void );
void configure_dss( UINT32 framebuffer );
void display_lcd_image( void );
void lcd_config(UINT32 framebuffer);
void lcd_shutdown(void);
UINT32 disable_lcd_power(void);
UINT32 disable_lcd_backlight(void);
void LcdStall(DWORD dwMicroseconds);
void LcdSleep(DWORD dwMilliseconds);
VOID FillASCII(BYTE showCharMode[][10]);
//VOID FillASCII(void);



//------------------------------------------------------------------------------
//
// defines
//
#ifdef BSP_Z2000
	#define LOGO_WIDTH			320	//480    // Logo bitmap image is RGB24 VGA Portrait bitmap
	#define LOGO_HEIGHT			240	//640
#else
	#define LOGO_WIDTH			240 // Logo bitmap image is RGB24 VGA Portrait bitmap
	#define LOGO_HEIGHT			320	
#endif

#define BYTES_PER_PIXEL			3
#define DELAY_COUNT				100 
#define LOGO_GFX_ATTRIBUTES		(DISPC_GFX_ATTR_GFXENABLE | DISPC_GFX_ATTR_GFXFORMAT(DISPC_PIXELFORMAT_RGB24))           // RGB24 packed, enabled

#define BSP_LCD_CONFIG			(DISPC_CONFIG_FUNCGATED | DISPC_CONFIG_LOADMODE(2))

#define BSP_GFX_POS					(DISPC_GFX_POS_GFXPOSY(g_dwLogoPosY) | DISPC_GFX_POS_GFXPOSX(g_dwLogoPosX))
#define BSP_GFX_SIZE                (DISPC_GFX_SIZE_GFXSIZEY(g_dwLogoHeight) | DISPC_GFX_SIZE_GFXSIZEX(g_dwLogoWidth))
#define BSP_GFX_FIFO_THRESHOLD      (DISPC_GFX_FIFO_THRESHOLD_LOW(192) | DISPC_GFX_FIFO_THRESHOLD_HIGH(252))
#define BSP_GFX_ROW_INC             0x00000001
#define BSP_GFX_PIXEL_INC           0x00000001
#define BSP_GFX_WINDOW_SKIP         0x00000000

#define SPI_CS0_PIN     174         //mcspi1_cs0
#define SPI_CLK_PIN     171         //mcspi1_clk
#define SPI_DOUT_PIN    172         //mcspi1_simo

DWORD   g_dwLogoPosX;
DWORD   g_dwLogoPosY;

DWORD   g_dwLogoWidth;
DWORD   g_dwLogoHeight;

//-----------------------------------------------------------------------------
//Ray 131025
#define FONT_HEIGHT		16
#define FONT_WIDTH		8

static DWORD g_nHeight = 0;
static DWORD g_nWidth = 0;
static DWORD g_nBpp;
static DWORD g_wFontColor = 0x00000000; //??_RR_GG_BB
static DWORD g_wBkColor   = 0xffffffff; //??
static volatile DWORD *g_dwFrameBuffer = NULL;


const BYTE asciiFont[][16] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  []  ASCII =  0x1F */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [ ]  ASCII =  0x20 */
{0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18,
 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},  /*  [!]  ASCII =  0x21 */
{0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  ["]  ASCII =  0x22 */
{0x00, 0x00, 0x00, 0x36, 0x36, 0x7F, 0x36, 0x36,
 0x36, 0x7F, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00},  /*  [#]  ASCII =  0x23 */
{0x00, 0x18, 0x18, 0x3C, 0x66, 0x60, 0x30, 0x18,
 0x0C, 0x06, 0x66, 0x3C, 0x18, 0x18, 0x00, 0x00},  /*  [$]  ASCII =  0x24 */
{0x00, 0x00, 0x70, 0xD8, 0xDA, 0x76, 0x0C, 0x18,
 0x30, 0x6E, 0x5B, 0x1B, 0x0E, 0x00, 0x00, 0x00},  /*  [%]  ASCII =  0x25 */
{0x00, 0x00, 0x70, 0xD0, 0xD0, 0x70, 0x00, 0x10,
 0x30, 0x60, 0x50, 0x10, 0x00, 0x00, 0x00, 0x00},  /*  [&]  ASCII =  0x26 */
{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [']  ASCII =  0x27 */
{0x00, 0x00, 0x00, 0x0C, 0x18, 0x18, 0x30, 0x30,
 0x30, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x00, 0x00},  /*  [(]  ASCII =  0x28 */
{0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x0C, 0x0C,
 0x0C, 0x0C, 0x0C, 0x18, 0x18, 0x30, 0x00, 0x00},  /*  [)]  ASCII =  0x29 *///10
{0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x1C, 0x7F,
 0x1C, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [*]  ASCII =  0x2A */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E,
 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [+]  ASCII =  0x2B */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x1C, 0x1C, 0x0C, 0x18, 0x00, 0x00},  /*  [,]  ASCII =  0x2C */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [-]  ASCII =  0x2D */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x00},  /*  [.]  ASCII =  0x2E */
{0x00, 0x00, 0x00, 0x06, 0x06, 0x0C, 0x0C, 0x18,
 0x18, 0x30, 0x30, 0x60, 0x60, 0x00, 0x00, 0x00},  /*  [/]  ASCII =  0x2F */
{0x00, 0x00, 0x00, 0x1E, 0x33, 0x37, 0x37, 0x33,
 0x3B, 0x3B, 0x33, 0x1E, 0x00, 0x00, 0x00, 0x00},  /*  [0]  ASCII =  0x30 */
{0x00, 0x00, 0x00, 0x0C, 0x1C, 0x7C, 0x0C, 0x0C,
 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00},  /*  [1]  ASCII =  0x31 */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x0C,
 0x18, 0x30, 0x60, 0x7E, 0x00, 0x00, 0x00, 0x00},  /*  [2]  ASCII =  0x32 */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x1C,
 0x06, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [3]  ASCII =  0x33 *///20
{0x00, 0x00, 0x00, 0x30, 0x30, 0x36, 0x36, 0x36,
 0x66, 0x7F, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00},  /*  [4]  ASCII =  0x34 */
{0x00, 0x00, 0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C,
 0x06, 0x06, 0x0C, 0x78, 0x00, 0x00, 0x00, 0x00},  /*  [5]  ASCII =  0x35 */
{0x00, 0x00, 0x00, 0x1C, 0x18, 0x30, 0x7C, 0x66,
 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [6]  ASCII =  0x36 */
{0x00, 0x00, 0x00, 0x7E, 0x06, 0x0C, 0x0C, 0x18,
 0x18, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00},  /*  [7]  ASCII =  0x37 */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x76, 0x3C,
 0x6E, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [8]  ASCII =  0x38 */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66,
 0x3E, 0x0C, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00},  /*  [9]  ASCII =  0x39 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00,
 0x00, 0x00, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x00},  /*  [:]  ASCII =  0x3A */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00,
 0x00, 0x00, 0x1C, 0x1C, 0x0C, 0x18, 0x00, 0x00},  /*  [;]  ASCII =  0x3B */
{0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60,
 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00},  /*  [<]  ASCII =  0x3C */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00,
 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [=]  ASCII =  0x3D *///30
{0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06,
 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00},  /*  [>]  ASCII =  0x3E */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x0C, 0x18,
 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},  /*  [?]  ASCII =  0x3F */
{0x00, 0x00, 0x00, 0x7E, 0xC3, 0xC3, 0xCF, 0xDB,
 0xDB, 0xCF, 0xC0, 0x7F, 0x00, 0x00, 0x00, 0x00},  /*  [@]  ASCII =  0x40 */
{0x00, 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x66,
 0x7E, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [A]  ASCII =  0x41 */
{0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x7C,
 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00},  /*  [B]  ASCII =  0x42 *///35
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x60,
 0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [C]  ASCII =  0x43 *///36
{0x00, 0x00, 0x00, 0x78, 0x6C, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x6C, 0x78, 0x00, 0x00, 0x00, 0x00},  /*  [D]  ASCII =  0x44 */
{0x00, 0x00, 0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C,
 0x60, 0x60, 0x60, 0x7E, 0x00, 0x00, 0x00, 0x00},  /*  [E]  ASCII =  0x45 *///38
{0x00, 0x00, 0x00, 0x7E, 0x60, 0x60, 0x60, 0x7C,
 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00},  /*  [F]  ASCII =  0x46 */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x60,
 0x6E, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00},  /*  [G]  ASCII =  0x47 */
{0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7E,
 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [H]  ASCII =  0x48 */
{0x00, 0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18,
 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [I]  ASCII =  0x49 */
{0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06,
 0x06, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [J]  ASCII =  0x4A */
{0x00, 0x00, 0x00, 0x66, 0x66, 0x6C, 0x6C, 0x78,
 0x6C, 0x6C, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [K]  ASCII =  0x4B */
{0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60,
 0x60, 0x60, 0x60, 0x7E, 0x00, 0x00, 0x00, 0x00},  /*  [L]  ASCII =  0x4C *///45
{0x00, 0x00, 0x00, 0x63, 0x63, 0x77, 0x6B, 0x6B,
 0x6B, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00},  /*  [M]  ASCII =  0x4D */
{0x00, 0x00, 0x00, 0x63, 0x63, 0x73, 0x7B, 0x6F,
 0x67, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00},  /*  [N]  ASCII =  0x4E */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [O]  ASCII =  0x4F *///48
{0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x7C,
 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00},  /*  [P]  ASCII =  0x50 */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3C, 0x0C, 0x06, 0x00, 0x00},  /*  [Q]  ASCII =  0x51 *///50
{0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x7C,
 0x6C, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [R]  ASCII =  0x52 */
{0x00, 0x00, 0x00, 0x3C, 0x66, 0x60, 0x30, 0x18,
 0x0C, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [S]  ASCII =  0x53 */
{0x00, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18,
 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},  /*  [T]  ASCII =  0x54 */
{0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [U]  ASCII =  0x55 */
{0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00},  /*  [V]  ASCII =  0x56 */
{0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x6B, 0x6B,
 0x6B, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00},  /*  [W]  ASCII =  0x57 */
{0x00, 0x00, 0x00, 0x66, 0x66, 0x34, 0x18, 0x18,
 0x2C, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [X]  ASCII =  0x58 */
{0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C,
 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},  /*  [Y]  ASCII =  0x59 */
{0x00, 0x00, 0x00, 0x7E, 0x06, 0x06, 0x0C, 0x18,
 0x30, 0x60, 0x60, 0x7E, 0x00, 0x00, 0x00, 0x00},  /*  [Z]  ASCII =  0x5A */
{0x00, 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30,
 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x3C},  /*  [[]  ASCII =  0x5B *///60
{0x00, 0x00, 0x00, 0x60, 0x60, 0x30, 0x30, 0x18,
 0x18, 0x0C, 0x0C, 0x06, 0x06, 0x00, 0x00, 0x00},  /*  [\]  ASCII =  0x5C */
{0x00, 0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C,
 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x3C},  /*  []]  ASCII =  0x5D */
{0x00, 0x18, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [^]  ASCII =  0x5E */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},  /*  [_]  ASCII =  0x5F */
{0x00, 0x38, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [`]  ASCII =  0x60 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x06, 0x06,
 0x3E, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00},  /*  [a]  ASCII =  0x61 *///66
{0x00, 0x00, 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00},  /*  [b]  ASCII =  0x62 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x60,
 0x60, 0x60, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [c]  ASCII =  0x63 */
{0x00, 0x00, 0x00, 0x06, 0x06, 0x3E, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00},  /*  [d]  ASCII =  0x64 *///69
{0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66,
 0x7E, 0x60, 0x60, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [e]  ASCII =  0x65 */
{0x00, 0x00, 0x00, 0x1E, 0x30, 0x30, 0x30, 0x7E,
 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00},  /*  [f]  ASCII =  0x66 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x7C, 0x7C},  /*  [g]  ASCII =  0x67 */
{0x00, 0x00, 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [h]  ASCII =  0x68 */
{0x00, 0x00, 0x18, 0x18, 0x00, 0x78, 0x18, 0x18,
 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00},  /*  [i]  ASCII =  0x69 *///74
{0x00, 0x00, 0x0C, 0x0C, 0x00, 0x3C, 0x0C, 0x0C,
 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x78, 0x78},  /*  [j]  ASCII =  0x6A */
{0x00, 0x00, 0x00, 0x60, 0x60, 0x66, 0x66, 0x6C,
 0x78, 0x6C, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [k]  ASCII =  0x6B */
{0x00, 0x00, 0x00, 0x78, 0x18, 0x18, 0x18, 0x18,
 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00},  /*  [l]  ASCII =  0x6C */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x6B, 0x6B,
 0x6B, 0x6B, 0x6B, 0x63, 0x00, 0x00, 0x00, 0x00},  /*  [m]  ASCII =  0x6D */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [n]  ASCII =  0x6E */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},  /*  [o]  ASCII =  0x6F *///80
{0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60},  /*  [p]  ASCII =  0x70 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x06},  /*  [q]  ASCII =  0x71 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x6E, 0x70,
 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00},  /*  [r]  ASCII =  0x72 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x60, 0x60,
 0x3C, 0x06, 0x06, 0x7C, 0x00, 0x00, 0x00, 0x00},  /*  [s]  ASCII =  0x73 */
{0x00, 0x00, 0x00, 0x30, 0x30, 0x7E, 0x30, 0x30,
 0x30, 0x30, 0x30, 0x1E, 0x00, 0x00, 0x00, 0x00},  /*  [t]  ASCII =  0x74 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3E, 0x00, 0x00, 0x00, 0x00},  /*  [u]  ASCII =  0x75 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00},  /*  [v]  ASCII =  0x76 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x6B, 0x6B,
 0x6B, 0x6B, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00},  /*  [w]  ASCII =  0x77 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x3C,
 0x18, 0x3C, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},  /*  [x]  ASCII =  0x78 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
 0x66, 0x66, 0x66, 0x3C, 0x0C, 0x18, 0xF0, 0xF0},  /*  [y]  ASCII =  0x79 */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x06, 0x0C,
 0x18, 0x30, 0x60, 0x7E, 0x00, 0x00, 0x00, 0x00},  /*  [z]  ASCII =  0x7A */
{0x00, 0x00, 0x00, 0x0C, 0x18, 0x18, 0x18, 0x30,
 0x60, 0x30, 0x18, 0x18, 0x18, 0x0C, 0x00, 0x00},  /*  [{]  ASCII =  0x7B */
{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18,
 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},  /*  [|]  ASCII =  0x7C */
{0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x18, 0x0C,
 0x06, 0x0C, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00},  /*  [}]  ASCII =  0x7D */
{0x00, 0x00, 0x00, 0x71, 0xDB, 0x8E, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  [~]  ASCII =  0x7E */
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  /*  []  ASCII =  0x7F */
};

//-----------------------------------------------------------------------------
//
BOOL InitGraphicsEngine(DWORD nWidth, DWORD nHeight, PUCHAR nBpp, DWORD dwFrameBuffer)
{
		
	g_nHeight = nHeight;
	g_nWidth  = nWidth;	
	g_nBpp = (DWORD)nBpp; 
    g_dwFrameBuffer = (volatile DWORD *) dwFrameBuffer;
	
	//FillASCII();
	
	return TRUE;
}
//
VOID FillASCII(BYTE showCharMode[][10])
{
    volatile DWORD *mem = g_dwFrameBuffer;
    //static DWORD nWidth	= 360;  
    static DWORD nWidth	= 180;
    int printMode = 0;
	int printN = 10;
	//BYTE showChar[] ={45,80,66,69,74,79,72,15,15,15};	//Loading...
	UINT16 tempbit, i, j;
    int p = 8;
    unsigned long offset;
    int offh = 0, offw = 0;
    unsigned char bit;
	BYTE c = 0; 
	static int cur_col  = 13;
	static int cur_row  = 7;        //shift a row place equal a pixel, Ray 131003  
	BYTE n = 0;
	int shiftPalce = 8;	

	
	offw =  FONT_WIDTH * cur_row ;  			// offw = 8
	//printN = sizeof(showChar);
	
	while(n < printN)
	{
		c = showCharMode[printMode][n];
		//c = showChar[n];
		for (i=0; i<16; i++) 
		{
        	offh =  FONT_HEIGHT * cur_col + i;     //offh =  16+0, 16+1.....
     
        	for (j=0; j<8; j++) 
        	{
            	tempbit = 1 << j; 
            	bit = (unsigned char)tempbit;
            	bit &= asciiFont[c][i];                 //do array location & bit 
                offset = nWidth * offh + offw;
                if (bit) {							//Non-zero is Running
        	  		mem[ p + offset ] = g_wFontColor;
                	p--;
            	}else {
                	mem[ p + offset ] = g_wBkColor;
                	p--;
            	}
 
            	if(p == 0){
                	p = 8;
            	}
        	}
    	}
		offw += shiftPalce;
		n++;
	}
}
//
VOID FillASCIIMode(int mode)
{
    BYTE showCharMode[4][10] = {{54,81,69,66,85,70,84,15,15,15},	  //Updates... 
                                {51,70,84,70,83,87,70,69,15,15},      //Reserved..
                                {51,70,84,70,83,87,70,69,15,15},
                                {51,70,84,70,83,87,70,69,15,15}};
   switch(mode)
   {
        case 1:
            FillASCII((showCharMode+0));
            break;
        case 2:
            FillASCII((showCharMode+1));
            break;
        case 3:
            FillASCII((showCharMode+2));
            break;
        case 4:
            FillASCII((showCharMode+3));
            break;
   }
}

//-----------------------------------------------------------------------------
HANDLE hGpio = NULL;


void spiWrBitHigh(void)
{
    DWORD delay = 10;

    GPIOSetBit(hGpio, SPI_DOUT_PIN);

    GPIOClrBit(hGpio, SPI_CLK_PIN);
    LcdStall(delay);
    GPIOSetBit(hGpio, SPI_CLK_PIN);
    LcdStall(delay);
    GPIOClrBit(hGpio, SPI_CLK_PIN);
}

void spiWrBitLow(void)
{
    int delay = 10;
    
    GPIOClrBit(hGpio, SPI_DOUT_PIN);

    GPIOClrBit(hGpio, SPI_CLK_PIN);
    LcdStall(delay);
    GPIOSetBit(hGpio, SPI_CLK_PIN);
    LcdStall(delay);
    GPIOClrBit(hGpio, SPI_CLK_PIN);
}

void spi_SendData(short iSendData)
{
    short iCount = 0;
	
	GPIOClrBit(hGpio, SPI_CS0_PIN);
    for(iCount=8; iCount>=0; iCount--)
    {
		if(iSendData & (1<<iCount)) 
			spiWrBitHigh();
		else 
			spiWrBitLow();
	}
	GPIOSetBit(hGpio, SPI_CS0_PIN);
}

void WMLCDCOMD(short cmd)
{
    spi_SendData(cmd);
}

void WMLCDDATA(short dat)
{
	dat |= 0x0100;
	spi_SendData(dat);
}

void LCD_Initial_Code(void)
{
	WMLCDCOMD(0xB0); // Manufacturer Command Access Protect
    WMLCDDATA(0x3F);
	WMLCDDATA(0x3F);
	LcdSleep(5);
	
	WMLCDCOMD(0xFE);
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0x21);
	WMLCDDATA(0xB4);
	
	WMLCDCOMD(0xB3); // Frame Memory Access and Interface Setting
	WMLCDDATA(0x00);
	WMLCDDATA(0x10);
	
	WMLCDCOMD(0xE0); // NVM Access Control
	WMLCDDATA(0x00); // NVAE: NVM access enable register. NVM access is enabled when NVAE=1
	WMLCDDATA(0x40); // FTT: NVM control bit.
	//Sleep(10);
	LcdSleep(10);
	WMLCDCOMD(0xB3); // Frame Memory Access and Interface Setting
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	
	WMLCDCOMD(0xFE); // MAGIC - TODO
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0x21);
	WMLCDDATA(0x30);
	
	WMLCDCOMD(0xB0); // Manufacturer Command Access Protect
	WMLCDDATA(0x3F);
	WMLCDDATA(0x3F); 
	
	WMLCDCOMD(0xB3); // Frame Memory Access and Interface Setting
	WMLCDDATA(0x02);
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	        	   	
	WMLCDCOMD(0xB4); //SET interface
	WMLCDDATA(0x10);
	
	WMLCDCOMD(0xC0); //Panel Driving Setting
	WMLCDDATA(0x03); //GIP REV  SM GS BGR SS
	WMLCDDATA(0x4F);
	WMLCDDATA(0x00);
	WMLCDDATA(0x10);
	WMLCDDATA(0xA2); //BLV=0 LINE
	WMLCDDATA(0x00);
	WMLCDDATA(0x01);
	WMLCDDATA(0x00);
	
	WMLCDCOMD(0xC1); //Display Timing Setting for Normal/Partial Mode
	WMLCDDATA(0x01);
	WMLCDDATA(0x02);
	WMLCDDATA(0x19);
	WMLCDDATA(0x08);
	WMLCDDATA(0x08);

    LcdSleep(25);
	WMLCDCOMD(0xC3); //PRTIAL MODE
	WMLCDDATA(0x01);
	WMLCDDATA(0x00);
	WMLCDDATA(0x28);
	WMLCDDATA(0x08);
	WMLCDDATA(0x08);

	LcdSleep(25);
	WMLCDCOMD(0xC4);
	WMLCDDATA(0x11);
	WMLCDDATA(0x01);
	WMLCDDATA(0x43);
	WMLCDDATA(0x04);
	
	WMLCDCOMD(0xC8); //set gamma
	WMLCDDATA(0x0C);
	WMLCDDATA(0x0C);
	WMLCDDATA(0x0D);
	WMLCDDATA(0x14);
	WMLCDDATA(0x18);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x09);
	WMLCDDATA(0x09);
	WMLCDDATA(0x03);
	WMLCDDATA(0x05);
	WMLCDDATA(0x00);
	WMLCDDATA(0x03);
	WMLCDDATA(0x08);
	WMLCDDATA(0x07);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x15);
	WMLCDDATA(0x12);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x00);
	
	WMLCDCOMD(0xC9); //set gamma
	WMLCDDATA(0x0C);
	WMLCDDATA(0x0C);
	WMLCDDATA(0x0D);
	WMLCDDATA(0x14);
	WMLCDDATA(0x18);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x09);
	WMLCDDATA(0x09);
	WMLCDDATA(0x03);
	WMLCDDATA(0x05);
	WMLCDDATA(0x00);
	WMLCDDATA(0x03);
	WMLCDDATA(0x08);
	WMLCDDATA(0x07);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x15);
	WMLCDDATA(0x12);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x00);
	
	WMLCDCOMD(0xCA); //set gamma
	WMLCDDATA(0x0C);
	WMLCDDATA(0x0C);
	WMLCDDATA(0x0D);
	WMLCDDATA(0x14);
	WMLCDDATA(0x18);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x09);
	WMLCDDATA(0x09);
	WMLCDDATA(0x03);
	WMLCDDATA(0x05);
	WMLCDDATA(0x00);
	WMLCDDATA(0x03);
	WMLCDDATA(0x08);
	WMLCDDATA(0x07);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x15);
	WMLCDDATA(0x12);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x0E);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x0A);
	WMLCDDATA(0x00);
	
	WMLCDCOMD(0xD0); //Power Setting
	WMLCDDATA(0x63); //BT[2:0]=110  VCI+VCI2��2  :5   -(VCI2��2):
	WMLCDDATA(0x53);
	WMLCDDATA(0x82); //VC2[2:0]=010,VCI2=5V
	WMLCDDATA(0x3F); //VREG=5.0V 
	
	WMLCDCOMD(0xD1); //set vcom
	WMLCDDATA(0x6A); //VCOMH
	WMLCDDATA(0x64); //VDV
	
	WMLCDCOMD(0xD2); //Power Setting (Note 1) for Normal/Partial Mode
	WMLCDDATA(0x03);
	WMLCDDATA(0x24);
	
	WMLCDCOMD(0xD4); //Power Setting (Note 1) for Idle Mode
	WMLCDDATA(0x03);
	WMLCDDATA(0x24);
	
	WMLCDCOMD(0xE2); //NVM Load Control
	WMLCDDATA(0x3F);

	WMLCDCOMD(0x35); //set_tear_on
	WMLCDDATA(0x00);
	
	WMLCDCOMD(0x36);
	WMLCDDATA(0x00);
	
	WMLCDCOMD(0x3A); //set_pixel_format
	WMLCDDATA(0x66); // 66 18-bits
	
	WMLCDCOMD(0x2A); //set_column_address
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0xEF);
	
	WMLCDCOMD(0x2B); //set_page_address:
	WMLCDDATA(0x00);
	WMLCDDATA(0x00);
	WMLCDDATA(0x01);
	WMLCDDATA(0x3F);
	\
	WMLCDCOMD(0x11); //exit_sleep_mode
	LcdSleep(120);
	WMLCDCOMD(0x29); //set_display_on
	LcdSleep(30);
	WMLCDCOMD(0x2C); //send DDRAM set
}
//-----------------------------------------------------------------------------
void ClearDisplayBuffer()
{
	DWORD framebufferPA;
	DWORD i, len;
	
	len = (320 * 240 * 3) / 4;
	LcdPdd_GetMemory( NULL, &framebufferPA );
	for( i=0 ; i<len ; i++ )
		*((DWORD *)framebufferPA + i) = 0x0;
}
//-----------------------------------------------------------------------------
static void FlipFrameBuffer(PUCHAR fb, DWORD h, DWORD lineSize,PUCHAR temporaryBuffer)
{
    DWORD y;
    PUCHAR top;
    PUCHAR bottom;

    top = fb;
    bottom = fb + ((h-1)*lineSize);
    
    for (y=0;y<h/2;y++)
    {
        memcpy(temporaryBuffer,top,lineSize);
        memcpy(top,bottom,lineSize);
        memcpy(bottom,temporaryBuffer,lineSize);
        top += lineSize;
        bottom -= lineSize;
    }
}
//------------------------------------------------------------------------------
//
//  Function:  ShowLogo
//
//  This function shows the logo splash screen
//
VOID ShowLogo(UINT32 flashAddr, UINT32 offset)
{
    HANDLE  hFlash = NULL;
    DWORD	framebuffer;
    DWORD	framebufferPA;
    PUCHAR  pChar;
    ULONG   x, y;
    WORD	wSignature = 0;
    DWORD   dwOffset = 0;
    DWORD   dwLcdWidth, dwLcdHeight;
    DWORD   dwLength;

    //  Get the LCD width and height
    LcdPdd_LCD_GetMode( NULL, &dwLcdWidth, &dwLcdHeight, NULL );
	
    dwLength = BYTES_PER_PIXEL * LOGO_WIDTH * LOGO_HEIGHT;

    //  Get the video memory
    LcdPdd_GetMemory( NULL, &framebufferPA );
    framebuffer = (DWORD) OALPAtoUA(framebufferPA);
    pChar = (PUCHAR)framebuffer;
    
    if (flashAddr != -1)
    {
        // Open flash storage
        hFlash = OALFlashStoreOpen(flashAddr);
        if( hFlash != NULL )
        {
            //  The LOGO reserved NAND flash region contains the BMP file
            OALFlashStoreBufferedRead( hFlash, offset, (UCHAR*) &wSignature, sizeof(wSignature), FALSE );

            //  Check for 'BM' signature
            if( wSignature == 0x4D42 )  
            {
                //  Read the offset to the pixel data
                OALFlashStoreBufferedRead( hFlash, offset + 10, (UCHAR*) &dwOffset, sizeof(dwOffset), FALSE );

                //  Read the pixel data with the given offset
                OALFlashStoreBufferedRead( hFlash, offset + dwOffset, pChar, dwLength, FALSE );
            }
           
            //  Close store
            OALFlashStoreClose(hFlash);
        
            //  Compute position and size of logo image 
            g_dwLogoPosX   = (dwLcdWidth - LOGO_WIDTH)/2;
            g_dwLogoPosY   = (dwLcdHeight - LOGO_HEIGHT)/2;
            g_dwLogoWidth  = LOGO_WIDTH;
            g_dwLogoHeight = LOGO_HEIGHT;
            
            //As BMP are stored upside down, we need to flip the frame buffer's content
            FlipFrameBuffer((PUCHAR)framebuffer,LOGO_HEIGHT,LOGO_WIDTH*BYTES_PER_PIXEL,(PUCHAR)framebuffer + dwLength);
        }
    }

    //  If bitmap signature is valid, display the logo, otherwise fill screen with pattern
    if( wSignature != 0x4D42 )
    {
        //  Adjust color bars to LCD size
        g_dwLogoPosX   = 0;
        g_dwLogoPosY   = 0;
        g_dwLogoWidth  = dwLcdWidth;
        g_dwLogoHeight = dwLcdHeight;
        
        for (y= 0; y < dwLcdHeight; y++)
        {
            for( x = 0; x < dwLcdWidth; x++ )
            {
                if( y < dwLcdHeight/2 )
                {
                    if( x < dwLcdWidth/2 )
                    {
                        *pChar++ = 0x00;    //  Blue
                        *pChar++ = 0x00;    //  Green
                        *pChar++ = 0xFF;    //  Red
                    }
                    else
                    {
                        *pChar++ = 0x00;    //  Blue
                        *pChar++ = 0xFF;    //  Green
                        *pChar++ = 0x00;    //  Red
                    }
                }
                else
                {
                    if( x < dwLcdWidth/2 )
                    {
                        *pChar++ = 0xFF;    //  Blue
                        *pChar++ = 0x00;    //  Green
                        *pChar++ = 0x00;    //  Red
                    }
                    else
                    {
                        *pChar++ = 0x00;    //  Blue
                        *pChar++ = 0xFF;    //  Green
                        *pChar++ = 0xFF;    //  Red
                    }
                }
            }
        }
    }
	
    //  Fire up the LCD
    lcd_config(framebufferPA);       
}

//------------------------------------------------------------------------------
//
//  Function:   ShowSDLogo
//
//  This function is called to display the splaschreen bitmap from the SDCard
//
//
BOOL ShowSDLogo()
{
    DWORD framebuffer = 0;
    DWORD framebufferPA = 0;
    DWORD dwLcdWidth = 0;
    DWORD dwLcdHeight = 0;
	DWORD dwLength = 0;
	PUCHAR 	pChar;          //Ray 131028
    // Get the LCD width and height
    LcdPdd_LCD_GetMode( NULL, &dwLcdWidth, &dwLcdHeight, NULL );
	//OALMSG(OAL_INFO, (L"ShowSDLogo: dwLcdWidth = %d, dwLcdHeight = %d\r\n",dwLcdWidth,dwLcdHeight));
	
	// Get the frame buffer
	LcdPdd_GetMemory( NULL, &framebufferPA );
	//OALMSG(OAL_INFO, (L"ShowSDLogo: framebuffer = 0x%x\r\n",framebufferPA));
    framebuffer = (DWORD) OALPAtoUA(framebufferPA);
	//OALMSG(OAL_INFO, (L"ShowSDLogo: framebuffer OALPAtoUA = 0x%x\r\n",framebuffer));
	pChar = (PUCHAR)framebuffer;     //Ray 131028
	
	// Compute the size
	dwLength = BYTES_PER_PIXEL * LOGO_WIDTH * LOGO_HEIGHT;
	OALMSG(OAL_INFO, (L"ShowSDLogo: BYTES_PER_PIXEL = %d\r\n",BYTES_PER_PIXEL));
	OALMSG(OAL_INFO, (L"ShowSDLogo: LOGO_WIDTH = %d\r\n",LOGO_WIDTH));
	OALMSG(OAL_INFO, (L"ShowSDLogo: LOGO_HEIGHT = %d\r\n",LOGO_HEIGHT));
	OALMSG(OAL_INFO, (L"ShowSDLogo: size = %d\r\n",dwLength));
	
#ifdef BSP_Z2000    //Ray 131024	
	if (!BLSDCardReadLogo(L"Logo.bmp", (UCHAR*)framebuffer, dwLength))
	{
		return FALSE;
	}
#else
    if (!BLSDCardReadLogo(L"LOGO_VER.bmp", (UCHAR*)framebuffer, dwLength))
	{
		return FALSE;
	}
#endif

    //  Compute position and size of logo image 
    g_dwLogoPosX   = (dwLcdWidth - LOGO_WIDTH)/2;
    g_dwLogoPosY   = (dwLcdHeight - LOGO_HEIGHT)/2;
    g_dwLogoWidth  = LOGO_WIDTH;
    g_dwLogoHeight = LOGO_HEIGHT;
	OALMSG(OAL_INFO, (L"ShowSDLogo: g_dwLogoPosX = %d,g_dwLogoPosY = %d\r\n",g_dwLogoPosX,g_dwLogoPosY));
	OALMSG(OAL_INFO, (L"ShowSDLogo: g_dwLogoWidth = %d,g_dwLogoHeight = %d\r\n",g_dwLogoWidth,g_dwLogoHeight));
    //As BMP are stored upside down, we need to flip the frame buffer's content
    FlipFrameBuffer((PUCHAR)framebuffer,LOGO_HEIGHT,LOGO_WIDTH*BYTES_PER_PIXEL,(PUCHAR)framebuffer + dwLength);

    //Initial graphics engine and draw ASCII function, Ray 131025
    //Check them size is right dwLcdWidth, dwLcdHeight?, Ray 131028
	InitGraphicsEngine(LOGO_WIDTH, LOGO_HEIGHT, pChar, framebuffer);

    //  Fire up the LCD
    lcd_config(framebufferPA);

	return TRUE;
}

//------------------------------------------------------------------------------
//
//  Function:  HideLogo
//
//  This function hides the logo splash screen
//
VOID HideLogo(VOID)
{
    lcd_shutdown();
}

//------------------------------------------------------------------------------
//
//  Function:  reset_display_controller
//
//  This function resets the Display Sub System on omap24xx
//
void reset_display_controller( void )
{
    
    UINT32 reg_val;
    UINT16 count;
    UINT32 timeout;
    UINT32 fclk, iclk;
    OMAP_PRCM_DSS_CM_REGS *pPrcmRegs = OALPAtoUA(OMAP_PRCM_DSS_CM_REGS_PA);
    OMAP_DISPC_REGS  *pDisplayRegs = OALPAtoUA(OMAP_DISC1_REGS_PA);
    
	//OALMSG(OAL_INFO, (L"reset_display_controller+\r\n"));

    // enable all display clocks
    fclk = INREG32(&pPrcmRegs->CM_FCLKEN_DSS); // functional clock
    iclk = INREG32(&pPrcmRegs->CM_ICLKEN_DSS); // interconnect clock

    OUTREG32(&pPrcmRegs->CM_FCLKEN_DSS, (fclk | CM_CLKEN_DSS1 | CM_CLKEN_DSS2));
    OUTREG32(&pPrcmRegs->CM_ICLKEN_DSS, (iclk | CM_CLKEN_DSS));
  
    // disable the display controller
    //disable_dss();

    // reset the display controller
    OUTREG32(&pDisplayRegs->DISPC_SYSCONFIG, DISPC_SYSCONFIG_SOFTRESET);
    
    // wait until reset completes OR timeout occurs
    timeout=10000;
    while(!((reg_val=INREG32(&pDisplayRegs->DISPC_SYSSTATUS)) & DISPC_SYSSTATUS_RESETDONE) && (timeout > 0))
    {
        // delay
        for(count=0;count<DELAY_COUNT;++count);
        timeout--;
    }

    if(!(reg_val & DISPC_SYSSTATUS_RESETDONE))
    {
        // OALMSG(OAL_INFO, (L"reset_display_controller: DSS reset timeout\r\n"));
    }
    
    reg_val=INREG32(&pDisplayRegs->DISPC_SYSCONFIG);
    reg_val &=~(DISPC_SYSCONFIG_SOFTRESET);
    OUTREG32(&pDisplayRegs->DISPC_SYSCONFIG,reg_val);

    // restore old clock settings
    OUTREG32(&pPrcmRegs->CM_FCLKEN_DSS, fclk);
    OUTREG32(&pPrcmRegs->CM_ICLKEN_DSS, iclk);
    
	//OALMSG(OAL_INFO, (L"reset_display_controller-\r\n"));
}

//------------------------------------------------------------------------------
//
//  Function:  lcd_config
//
//  This function configures the LCD
//
void lcd_config(UINT32 framebuffer)
{
	OALMSG(OAL_INFO, (L"lcd_config\r\n"));
	
    reset_display_controller();

    // Enable LCD clocks
    enable_lcd_power();

    // Configure the DSS registers
    configure_dss(framebuffer);
      
    // Display data on LCD
    display_lcd_image() ;
    
	// Turn on backlight last
	enable_lcd_backlight();

}

//------------------------------------------------------------------------------
//
//  Function:  lcd_shutdown
//
//  This function disables the backlight and power of the LCD controller
//
void lcd_shutdown()
{
    disable_lcd_backlight();
    LcdPdd_SetPowerLevel(D4);
    disable_lcd_power();
}   

//------------------------------------------------------------------------------
//
//  Function:  disable_dss
//
//  This function disables the Display Sub System on omap24xx
//
void disable_dss( void )
{
     //OALMSG(OAL_INFO, (L"disable_dss+\r\n"));

     //OALMSG(OAL_INFO, (L"disable_dss-\r\n"));
}
     
//------------------------------------------------------------------------------
//
//  Function:  enable_lcd_power
//
//  This function enables the power for the LCD controller
//
UINT32 enable_lcd_power( void )
{
    OMAP_PRCM_DSS_CM_REGS *pPrcmRegs = OALPAtoUA(OMAP_PRCM_DSS_CM_REGS_PA);
    
	//OALMSG(OAL_INFO, (L"enable DSS1_ALWON_FCLK, DSS2_ALWON_FCLK,\r\n"));
	//OALMSG(OAL_INFO, (L"enable DSS_L3_ICLK, DSS_L4_ICLK,\r\n"));
    SETREG32(&pPrcmRegs->CM_FCLKEN_DSS, (CM_CLKEN_DSS1 | CM_CLKEN_DSS2));
    SETREG32(&pPrcmRegs->CM_ICLKEN_DSS, (CM_CLKEN_DSS));

	//OALMSG(OAL_INFO, (L"enable_lcd_power-\r\n"));

    return ERROR_SUCCESS;
}

//------------------------------------------------------------------------------
//
//  Function:  disable_lcd_power
//
//  This function disables the power for the LCD controller
//
UINT32 disable_lcd_power( void )
{
    OMAP_PRCM_DSS_CM_REGS *pPrcmRegs = OALPAtoUA(OMAP_PRCM_DSS_CM_REGS_PA);

     //OALMSG(OAL_INFO, (L"disable_lcd_power+\r\n"));

    CLRREG32(&pPrcmRegs->CM_FCLKEN_DSS, (CM_CLKEN_DSS1 | CM_CLKEN_DSS2));
    CLRREG32(&pPrcmRegs->CM_ICLKEN_DSS, (CM_CLKEN_DSS));
    
     //OALMSG(OAL_INFO, (L"disable_lcd_power-\r\n"));
    return ERROR_SUCCESS;
}
      
//------------------------------------------------------------------------------
//
//  Function:  enable_lcd_backlight
//
//  This function enables the backlight for the LCD controller
//
UINT32 enable_lcd_backlight( void )
{
#ifdef BSP_Z2000
    void* hTwl;

    // Enable LEDA on TPS659XX
    hTwl = TWLOpen();
	TWLWriteByteReg(hTwl, TWL_PMBR1, 0x04); // PWM0 function is enabled
	TWLWriteByteReg(hTwl, TWL_GPBR1, 0x05); // PWM0_ENABLE & PWM0_CLK_ENABLE
	TWLWriteByteReg(hTwl, TWL_PWM0OFF, 0x7F);
	TWLWriteByteReg(hTwl, TWL_PWM0ON, 0x40);

    TWLClose(hTwl);
#else
	int i = 25;
	
	hGpio = GPIOOpen();

	GPIOSetMode(hGpio, SPI_CS0_PIN, GPIO_DIR_OUTPUT);
	GPIOSetMode(hGpio, SPI_DOUT_PIN, GPIO_DIR_OUTPUT);
	GPIOSetMode(hGpio, SPI_CLK_PIN, GPIO_DIR_OUTPUT);
	
    GPIOSetBit(hGpio, SPI_CS0_PIN);
    GPIOSetBit(hGpio, SPI_CLK_PIN);
    GPIOSetBit(hGpio, SPI_DOUT_PIN);
    
    OALMSG(1, (L"LCD send Initial Code\r\n"));
    LCD_Initial_Code();
    
    GPIOClrBit(hGpio, BL_EN_SET_GPIO);
    OALStall(1000);
	GPIOSetBit(hGpio, BL_EN_SET_GPIO);
	OALStall(1);
    for(; i > 0; i --)
    {
        GPIOClrBit(hGpio, BL_EN_SET_GPIO);
        OALStall(1);
        GPIOSetBit(hGpio, BL_EN_SET_GPIO);
        OALStall(1);
    }
	GPIOClose(hGpio);
	hGpio = NULL;
#endif

    return ERROR_SUCCESS;
}

//------------------------------------------------------------------------------
//
//  Function:  disable_lcd_backlight
//
//  This function disables the backlight for the LCD controller
//
UINT32 disable_lcd_backlight( void )
{
    HANDLE hTwl;

	//OALMSG(OAL_INFO, (L"disable_lcd_backlight+\r\n"));
    // Enable LEDA on TPS659XX
    hTwl = TWLOpen();
	TWLWriteByteReg(hTwl, TWL_PMBR1, 0x00);
	TWLWriteByteReg(hTwl, TWL_GPBR1, 0x00);
	TWLWriteByteReg(hTwl, TWL_PWM0OFF, 0x7F);
/*#ifdef BSP_EVM2
    TWLWriteByteReg(hTwl, TWL_LEDEN, 0x00);
#else
    // The hardware design is completely backwards.  In order
    // to disable the LED control signal, the LEDPWM signal must 
    // be enabled 100%
    // Set LEDAON, LEDAPWM
    TWLWriteByteReg(hTwl, TWL_LEDEN, 0x11);
    // Set PWM registers to same value to trigger 100% duty cycle
    TWLWriteByteReg(hTwl, TWL_PWMAOFF, 0x00);
    TWLWriteByteReg(hTwl, TWL_PWMAON, 0x00);
#endif
*/    
    TWLClose(hTwl);
     //OALMSG(OAL_INFO, (L"disable_lcd_backlight-\r\n"));
    return ERROR_SUCCESS;
}

//------------------------------------------------------------------------------
//
//  Function:  configure_dss
//
//  This function configures the Display Sub System on omap35xx
//
void configure_dss( UINT32 framebuffer )
{
    OMAP_DSS_REGS   *pDSSRegs = OALPAtoUA(OMAP_DSS1_REGS_PA);
    OMAP_DISPC_REGS *pDisplayRegs = OALPAtoUA(OMAP_DISC1_REGS_PA);
    OMAP_RFBI_REGS *pRfbiRegs = OALPAtoUA(OMAP_RFBI1_REGS_PA);
    //HANDLE hGpio = GPIOOpen();

	//OALMSG(OAL_INFO, (L"\r\nconfigure_dss+\r\n"));

    //  Configure the clock source
    OUTREG32( &pDSSRegs->DSS_CONTROL, 
			DSS_CONTROL_DISPC_CLK_SWITCH_DSS1_ALWON | DSS_CONTROL_DSI_CLK_SWITCH_DSS1_ALWON);
    
    //  Configure interconnect parameters
    OUTREG32( &pDSSRegs->DSS_SYSCONFIG, DISPC_SYSCONFIG_AUTOIDLE );
    OUTREG32( &pDisplayRegs->DISPC_SYSCONFIG, DISPC_SYSCONFIG_AUTOIDLE|SYSCONFIG_NOIDLE|SYSCONFIG_NOSTANDBY );

    // Not enabling any interrupts
    OUTREG32( &pDisplayRegs->DISPC_IRQENABLE, 0x00000000 );
    
    //  Configure the LCD
    LcdPdd_LCD_Initialize( pDSSRegs, pDisplayRegs, pRfbiRegs, NULL);
    	
    //  Over-ride default LCD config
    OUTREG32(&pDisplayRegs->DISPC_CONFIG,BSP_LCD_CONFIG);
    
    
    // Configure Graphics Window
    //--------------------------
    
    OUTREG32(&pDisplayRegs->DISPC_GFX_BA0 ,framebuffer );
 
    // configure the position of graphics window
    OUTREG32(&pDisplayRegs->DISPC_GFX_POSITION,BSP_GFX_POS);
    
    // size of graphics window
    OUTREG32(&pDisplayRegs->DISPC_GFX_SIZE,BSP_GFX_SIZE);
    
    // GW Enabled, RGB24 packed, Little Endian
    OUTREG32(&pDisplayRegs->DISPC_GFX_ATTRIBUTES,LOGO_GFX_ATTRIBUTES);
    
    OUTREG32(&pDisplayRegs->DISPC_GFX_FIFO_THRESHOLD,BSP_GFX_FIFO_THRESHOLD);
    OUTREG32(&pDisplayRegs->DISPC_GFX_ROW_INC,BSP_GFX_ROW_INC); 
    OUTREG32(&pDisplayRegs->DISPC_GFX_PIXEL_INC,BSP_GFX_PIXEL_INC); 
    OUTREG32(&pDisplayRegs->DISPC_GFX_WINDOW_SKIP,BSP_GFX_WINDOW_SKIP);

    //OALMSG(OAL_INFO, (L"configure_dss-\r\n"));

	//GPIOClose(hGpio);
}

//------------------------------------------------------------------------------
//
//  Function:  display_lcd_image
//
//  This function displays the image in the frame buffer on the LCD
//
void display_lcd_image( void )
{
    UINT8  count, timeout = DELAY_COUNT  ;
    UINT16 ctrl;
    OMAP_DISPC_REGS *pDisplayRegs = OALPAtoUA(OMAP_DISC1_REGS_PA);
    
    // Apply display configuration
    SETREG32(&pDisplayRegs->DISPC_CONTROL,DISPC_CONTROL_GOLCD);
    
    // wait for configuration to take effect
    do
    {
        for(count=0;count<DELAY_COUNT;++count);
        ctrl=INREG16(&pDisplayRegs->DISPC_CONTROL);
        timeout-- ;
    }
    while((ctrl & DISPC_CONTROL_GOLCD) && (timeout > 0));
    
	// Power up and start scanning
    // LcdPdd_SetPowerLevel(D0);   
    
	// OALMSG(OAL_INFO, (L"display_lcd_image-\r\n"));
}


void LcdStall(DWORD dwMicroseconds)
{
    OALStall(dwMicroseconds);
}

void LcdSleep(DWORD dwMilliseconds)
{
    OALStall(1000 * dwMilliseconds);
}

//------------------------------------------------------------------------------
//
// end of bsp_logo.c
