#include "bsp.h"
#include "tps659xx.h"
#include "tps659xx_internals.h"
#include "twl.h"
//
#include <eboot.h>
#include <bsp_logo.h>
//
#define SIZE 8                      //Ray 131104 
#define shiftLeft(col) (1<<col)     //Ray 131104 
//-----------------------------------------------------------------------------
//
void FillASCII();
void FillASCIIMode(int);
//VOID BLMenu(BOOL);

//-----------------------------------------------------------------------------
//Global variable, Ray 131104 
//
UINT8 gMatrix[SIZE];

//-----------------------------------------------------------------------------
//Ray 131025
//
typedef struct _HOTKEY_COLD_RESET{
		DWORD	 	keyActive;
		LPCWSTR		keyName;
}HOTKEY_COLD_RESET;

HOTKEY_COLD_RESET hotkeyColdReset[] ={
	{Hotkey_F1,     L"File Update"},
	{Hotkey_F2,	    L"Reserved"},
    {Hotkey_F3,	    L"F1 + F2"},
	{Hotkey_F4,     L"Reserved"},
	{Hotkey_EXIT,   L"Exit and Continue"},
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
//3(0000_1000),2(0000_0010), Ray 131104
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
//     
//
//-----------------------------------------------------------------------------
void HotKeyFunction(HANDLE hTwl)
{
	//UINT8 matrix[8];
	ULONG ik, ix, row, column, i;
    USHORT state;
    BOOL keyPressed = FALSE;
    WCHAR key;
    /*int modeF1 = 1,
        modeF2 = 2,
        modeF4 = 4;
        //modeF3 = 3,*/
       
    //Ray 131104
    for(i=0; i<Hotkey_EXIT; i++)
    {
		OALLog(L" [F%d] %s\r\n", i+1, hotkeyColdReset[i].keyName);
	}
	OALLog(L" [0] Exit and Continue\r\n");
	OALLog(L"\r>>> Now entry cold-reset... \r\n"); 
	
    //printing matrix array, Ray
	for( ik=0 ; ik<3 ; ik++ )
	{
		TWLReadRegs(hTwl, TWL_LOGADDR_FULL_CODE_7_0, gMatrix, sizeof(gMatrix));
	    //OALLog(L"******hTwl: %X....\r\n", hTwl);    //address-4, Ray
	    
		OALLog(L" HotKeyFunction: matrix  ");
		for( ix=0 ; ix <8 ; ix++ )
			OALLog(L" [%d]",gMatrix[ix]);
		OALLog(L"\r\n");
	}
	
	
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
                RETAILMSG(TRUE, (L"HotKeyFunction: [%d,%d]\r\n",row ,column));
			}
		}
	}
    //OALLog(L"******HotKey: [%d,%d]\r\n",gRow ,gColumn);

    //Ray 131104
    if(matrixStatus(4, 2) && matrixStatus(4, 3)){ //F1 + F2
             FillASCIIMode(3);       
            keyPressed = TRUE;
    }else if( matrixStatus(3, 2)){    
            FillASCIIMode(1);       //F1
            keyPressed = TRUE;
    }else if( matrixStatus(4, 3)){
            FillASCIIMode(2);       //F2
            keyPressed = TRUE;
    }
        
	do{
       key = OALBLMenuReadKey(TRUE);
	}while(key == L'0');
}

//-----------------------------------------------------------------------------
void HotKeyColdReset(HANDLE hTwl)
{
   HotKeyFunction(hTwl);
   //OALLog(L"******hTwl: %X....\r\n", hTwl);       //address-3, Ray
}


