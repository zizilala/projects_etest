//------------------------------------------------------------------------------
//  File:  Hotkey.c by Ray
//
//
#include "bsp.h"
#include "tps659xx.h"
#include "tps659xx_internals.h"
#include "twl.h"
//
#include <eboot.h>
#include <bsp_logo.h>

//-----------------------------------------------------------------------------
//  Preprocessor
// 
#define SIZE 8                      //Ray 131104 
#define shiftLeft(col) (1<<col)     //Ray 131104 

//-----------------------------------------------------------------------------
//  Function prototype
//  
BOOL FillASCIIMode(int);            //Ray 131104
extern VOID AllFunctionTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID DisplayTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID BkTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID DRAMTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID KeypadBkTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID AllFunctionTest_Z2000(OAL_BLMENU_ITEM *pMenu);
extern VOID DisplayTest_Z2000(OAL_BLMENU_ITEM *pMenu);
extern VOID TouchPanelTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID BatteryTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID LEDTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID BarcodeTest_Z2170P(OAL_BLMENU_ITEM *pMenu);   
extern VOID AudioAndMIC_Z2170P(OAL_BLMENU_ITEM *pMenu); 
extern VOID KeypadFunc_Z2170P(OAL_BLMENU_ITEM *pMenu); 
extern VOID BurnIn_Z2170P(OAL_BLMENU_ITEM *pMenu);
extern VOID RAMAccessTest(OAL_BLMENU_ITEM *pMenu);
extern VOID AutoScanFunc(OAL_BLMENU_ITEM *pMenu);
extern VOID SecondaryBATFunc(OAL_BLMENU_ITEM *pMenu);
extern VOID RTCFunc(OAL_BLMENU_ITEM *pMenu);
VOID LcdStall(DWORD);
VOID LcdSleep(DWORD);

//-----------------------------------------------------------------------------
//  Global variable 
//
UINT8 gMatrix[SIZE];                //Ray 131104 
DWORD g_d1Sec = 1000000;
OAL_BLMENU_ITEM *pMenu = NULL;

//-----------------------------------------------------------------------------
//
//
typedef struct _HOTKEY_COLD_RESET{          //Ray 131025
		DWORD	 	keyActive;
		LPCWSTR		keyName;
}HOTKEY_COLD_RESET;

HOTKEY_COLD_RESET hotkeyColdReset[] ={      //Ray 131106
	{Hotkey_F1,        L"F1 Updates"},
	{Hotkey_F2,	       L"F2"},
    {Hotkey_F3,	       L"Reserved"},
	{Hotkey_F4,        L"F4"},
	{Hotkey_F1_F2,     L"F1 + F2"},
	{Hotkey_F1_F3,     L"F1 + F3"},
	{Hotkey_F1_F4,     L"F1 + F4"},
	{Hotkey_F2_F3,     L"F2 + F3"},
	{Hotkey_F2_F4,     L"F2 + F4"}, 
	{Hotkey_F3_F4,     L"F3 + F4"},
	{Hotkey_EXIT,      L"Exit and Continue"},
};

//-----------------------------------------------------------------------------
void HotKeyInit(HANDLE hTwl)
{
	UINT8 regval;
	
	regval = TWL_KBD_CTRL_KBD_ON | TWL_KBD_CTRL_NRESET | TWL_KBD_CTRL_NSOFT_MODE;
    TWLWriteRegs(hTwl, TWL_KEYP_CTRL_REG, &regval, sizeof(regval));
	regval = 0x07 << 5;                        // Prescaler timer value
    TWLWriteRegs(hTwl, TWL_LK_PTV_REG, &regval, sizeof(regval));
}

//-----------------------------------------------------------------------------
//Ray 131104
//
BOOL matrixStatus(int row, int col)
{
    int temp;
    BOOL event = FALSE;
    
    temp = gMatrix[row] & shiftLeft(col);
 
    if(temp)
        return event = TRUE;
    else
        return event;
}
    
//-----------------------------------------------------------------------------
void HotKeyFunction(HANDLE hTwl)
{
	//UINT8 matrix[8];
	ULONG ik, ix, row, column;
	//ULONG i;
    USHORT state;
    BOOL keyPressed = FALSE;
    //WCHAR key; 
    
       
    //Ray 131104
    /*for(i=0; i<Hotkey_EXIT; i++)
    {
		OALLog(L" [%d] %s\r\n", i+1, hotkeyColdReset[i].keyName);
	}
	OALLog(L" [0] Exit and Continue\r\n");
	
	OALLog(L"\r>>> Now entry cold-reset... \r\n");*/
	
    //printing matrix array, Ray
	/*for( ik=0 ; ik<3 ; ik++ )
	{*/
    TWLReadRegs(hTwl, TWL_LOGADDR_FULL_CODE_7_0, gMatrix, sizeof(gMatrix));    
	/*//OALLog(L"******hTwl: %X....\r\n", hTwl);    //address-4, Ray
	    
		OALLog(L" HotKeyFunction: matrix  ");
		for( ix=0 ; ix <8 ; ix++ )
			OALLog(L" [%d]",gMatrix[ix]);
		OALLog(L"\r\n");
	}*/
	
	for(row = 0, ik = 0; row < 8; row++)
	{
        // Get matrix state        
        ix = row;
        state = gMatrix[ix] & 0xFF;     

        // If no-key is pressed continue with new rows
        if (state == 0) 
		{
            ik += 8;
            continue;               //if state event desnot 
		}
        
        for (column = 0; column < 8; column++, ik++)
		{
            if ((state & (1 << column)) != 0)
			{
                //RETAILMSG(TRUE, (L"HotKeyFunction: [%d,%d]\r\n",row ,column));
			}
		}
	}
   
    //Ray 131106
    if(matrixStatus(4, 2) && matrixStatus(4, 3)){               	//F1 + F2
        keyPressed = FillASCIIMode(hotkeyColdReset[4].keyActive);    
    }else if(matrixStatus(3, 1) && matrixStatus(3, 2)){         	//F1 + F4
        keyPressed = FillASCIIMode(hotkeyColdReset[6].keyActive);              
    }else if( matrixStatus(3, 2)){                              	//F1   
        keyPressed = FillASCIIMode(hotkeyColdReset[0].keyActive);     
    }else if( matrixStatus(4, 3)){                              	//F2
        keyPressed = FillASCIIMode(hotkeyColdReset[1].keyActive);       
    }else if( matrixStatus(3, 1)){                              	//F4
        keyPressed = FillASCIIMode(hotkeyColdReset[3].keyActive);       
    }
        
    /*do{   
        key = OALBLMenuReadKey(TRUE);
	}while(key != L'0');*/

	if (/*key == L'0' || */keyPressed == TRUE) return;
}

//-----------------------------------------------------------------------------
void HotKeyColdReset(HANDLE hTwl)
{
   HotKeyFunction(hTwl);
   //OALLog(L"******hTwl: %X....\r\n", hTwl);       //address-3, Ray
}

//-----------------------------------------------------------------------------
//  Ray 140218
//
VOID MenuSelectFunction(HANDLE hTwl)
{
    //ULONG ik, ix, row, column;
    //USHORT state;
    //BOOL keyPressed = FALSE;
SHOW_MENU:
    BLShowMenu(); 
    for(;;)
    {
        TWLReadRegs(hTwl, TWL_LOGADDR_FULL_CODE_7_0, gMatrix, sizeof(gMatrix));  
                
        /*for(row = 0, ik = 0; row < 8; row++)
    	{
            // Get matrix state        
            ix = row;
            state = gMatrix[ix] & 0xFF;     

            // If no-key is pressed continue with new rows
            if (state == 0) 
    		{
                ik += 8;
                continue;               //if state event desnot 
    		}
            
            for (column = 0; column < 8; column++, ik++)
    		{
                if ((state & (1 << column)) != 0)
    			{
                    RETAILMSG(TRUE, (L"HotKeyFunction: [%d,%d]\r\n",row ,column));
    			}
    		}
    	}*/
        
        if(matrixStatus(2, 5) ){            //press number 1
            AllFunctionTest_Z2170P(pMenu);
        }else if(matrixStatus(1, 1) ){      //press number 2
            DisplayTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(1, 3) ){      //press number 3
            BkTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(2, 1) ){      //press number 4  
            DRAMTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(2, 4) ){      //press number 5
            KeypadBkTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(2, 3) ){      //press number 6
            TouchPanelTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(3, 5) ){      //press number 7
            BatteryTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(2, 0) ){      //press number 8
            LEDTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(2, 2) ){      //press number 9
            BarcodeTest_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(3, 3) ){      //press F1
            KeypadFunc_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(4, 3) ){      //press F2
            BurnIn_Z2170P(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(3, 0) ){      //press F3
            RAMAccessTest(pMenu);
            //break;
            goto SHOW_MENU; 
        }else if((matrixStatus(3, 2))||(matrixStatus(0, 4))){   //press Scan or F4
            AutoScanFunc(pMenu);
            //break;
            goto SHOW_MENU; 
        }else if(matrixStatus(4, 2) ){      //press SP
            SecondaryBATFunc(pMenu);
            goto SHOW_MENU; 
        }else if(matrixStatus(4, 4) ){      //press SF
            RTCFunc(pMenu);
            goto SHOW_MENU; 
        }                                   
        LcdSleep(500);              
   }
}

VOID HotkeyMenuSelect(HANDLE hTwl)
{
	
	UNREFERENCED_PARAMETER(hTwl);
	MenuSelectFunction(hTwl);
    //RTCFunc(pMenu);
}


