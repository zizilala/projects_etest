#include "bsp.h"
#include "tps659xx.h"
#include "tps659xx_internals.h"
#include "twl.h"
//
#include <eboot.h>
#include <bsp_logo.h>
//
void FillASCII();
void FillASCIIMode(int);
//-----------------------------------------------------------------------------
/*struct status{
    unsigned int gEventF1
}*/
ULONG gRow = 0,
      gColumn = 0;
      
UINT8 gEvent; 
    /*gEventF1,
      gEventF2,
      gEventF4;*/
//UINT8 readMatrix[8][8]; 

//-----------------------------------------------------------------------------
//
//Ray 131025
typedef struct _HOTKEY_COLD_RESET{
		DWORD	 	keyActive;
		LPCWSTR		keyName;
}HOTKEY_COLD_RESET;

HOTKEY_COLD_RESET hotkeyColdReset[] ={
	{Hotkey_F1,     L"File Update"},
	{Hotkey_F2,	    L"Reserved"},
    {Hotkey_F3,	    L"Reserved"},
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
//
//3(0000_1000),2(0000_0010)
BOOL matrixKeypadState(int row, int column)
{
     /*BOOL rowEvent = FALSE,
          colEvent = FALSE;*/
     UINT8 rowEvent,
           colEvent;
     UINT8 i,j,result;
     UINT8 check = 0; 
     

     /*switch(row)
    {
        case 0:
            check = 0;
            break;       
        case 1:
            check = 1;
            break;  
        case 2:
            check = 2;
            break;       
        case 3:
            check = 3;
            break;  
        case 4:
            check = 4;
            break;       
        case 5:
            check = 5;
            break;  
        case 6:
            check = 6;
            break;      
        case 7:
            check = 7;
            break;  
        default:
            break;
    }*/

    /*if(check == 3 && column ==2 || (check == 3 && column ==2)&&(check == 4 && column ==3)){
        event1 = TRUE;
    }else if(check == 4 && column ==3){
        event2 = TRUE;
    }else if(check == 3 && column ==1){
        event3 = TRUE;
    }*/

    for(i=0; i<8; i++){
        column &= (0x01 << row);        
        if(column)
        {
            rowEvent = row;
        }
    }

    for(j=0; j<8; j++){
        row &= (0x01 << column);
        if(row)
        {
            colEvent = column;
        }
    }

    if(rowEvent == row && colEvent == column){
        return TRUE;
    }else{
        return FALSE;
    }
    
    
    /*if(check & (column & 0xFF)){
        colEvent = TRUE;
    }else{
        colEvent = FALSE; 
    }*/
    
    //return event; 
}

//-----------------------------------------------------------------------------
//     
//
//-----------------------------------------------------------------------------
void HotKeyFunction(HANDLE hTwl)
{
	UINT8 matrix[8];
	ULONG ik, ix, row, column;
    USHORT state;
    /*int i;

    for(i=0; i<Hotkey_EXIT; i++){
		OALLog(L" [F%d] %s\r\n", i+1, hotkeyColdReset[i].keyName);
	}*/

    //printing matrix array, Ray
	for( ik=0 ; ik<3 ; ik++ )
	{
		TWLReadRegs(hTwl, TWL_LOGADDR_FULL_CODE_7_0, matrix, sizeof(matrix));
	    //OALLog(L"******hTwl: %X....\r\n", hTwl);    //address-4, Ray
	    
		OALLog(L" HotKeyFunction: matrix  ");
		for( ix=0 ; ix <8 ; ix++ )
			OALLog(L" [%d]",matrix[ix]);
		OALLog(L"\r\n");
	}
	
	for(row = 0, ik = 0; row < 8; row++)
	{
        // Get matrix state        
        ix = row;
        state = matrix[ix] & 0xFF;     

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
                gRow    = row;
                gColumn = column;
			}
		}
	}
    //OALLog(L"******HotKey: [%d,%d]\r\n",gRow ,gColumn);
       //3(0000_1000),2(0000_0010)      
   if( matrixKeypadState(3, 2) || (matrixKeypadState(3, 2)&&matrixKeypadState(4, 3))){  
        FillASCIIMode(1);
    }

	 /*if((gRow == 3) && (gColumn == 2)){         //Ray 131029
        gEvent = 1;
	}else if((gRow == 4) && (gColumn == 3)){
        gEvent = 2;
	}else if((gRow == 3) && (gColumn == 1)){
        gEvent = 4;
	}*/
    
    /*if((row == 0)&&(column == 0)){ 
        OALLog(L" \rWait key-in a value...\r\n");
    }else{
         
    }*/
   	
}

//-----------------------------------------------------------------------------
// 
//  Functin: When operate on cold-reset, Ray 131025 
//
/*void HotKeyColdReset()
{
    //WCHAR key;
    //UINT8 entry = 0;
    UINT8 key;
    int i;
    int modeF1 = 1,
        modeF2 = 2,
        modeF4 = 4;
        //modeF3 = 3,
        
    //OALLog(L" \r\n>>> Now entry cold-reset <<< \r\n");
    
    for(i=0; i<Hotkey_EXIT; i++){
		OALLog(L" [F%d] %s\r\n", i+1, hotkeyColdReset[i].keyName);
	}
	//OALLog(L" [0] Exit and Continue\r\n");
    
	do{
        key = OALBLMenuReadKey(TRUE);
	}while (key < L'0' || key > L'0' + i);  
 
	/*do{
        entry = gEvent;
	}while(entry == modeF1 ); */
	
    
    //switch(**checkMatrix)
    /*switch(gEvent)
    {
       //case L'1':
        case 1:
                OALLog(L" \r>>> F1: File Update... \r\n");
                FillASCIIMode(modeF1);
            break;
        //case L'2':
        case 2:
                OALLog(L" \r>>> F2: Reserved... \r\n");
                FillASCIIMode(modeF2);
            break;
        /*case L'3':
                OALLog(L" \r>>> F3: Reserved... \r\n");
                FillASCIIMode(modeF3);
            break;*/
        /*case 4:
                OALLog(L" \r>>> F4: Reserved... \r\n");
                FillASCIIMode(modeF4);
            break;
        default:
                OALLog(L" \r>>> Exit and Continue... \r\n");
            break;
    }
    
    //if(key == L'0')return;
     
}*/
//
void HotKeyColdReset(HANDLE hTwl)
{
   HotKeyFunction(hTwl);
   //OALLog(L"******hTwl: %X....\r\n", hTwl);       //address-3, Ray
}


