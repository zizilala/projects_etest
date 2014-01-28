//------------------------------------------------------------------------------
//  File:  updatingFirmware.c , by Ray
//
//
#include <eboot.h>
#include "bsp.h"

#define I2C3_SCL_GPIO           184
#define I2C3_SDA_GPIO           185
//==============================================================================
VOID LcdStall(DWORD);
VOID LcdSleep(DWORD);

//==============================================================================
VOID I2C_Clk_FW(HANDLE hGPIO_I2C)
{
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);  
    LcdStall(50);
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO); 
    //LcdStall(50);
}
//
//  Set-up device Slave Addressing  
//
VOID I2C_setSlaveAddress_FW(HANDLE hGPIO_I2C, int addr)
{    
    int i;  
    for(i=7; i >=0; i--)
    {         
        if( addr&(1<<i) ){
            GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);  
            LcdStall(50);
            I2C_Clk_FW(hGPIO_I2C);
        }else{
            GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   
            LcdStall(50);
            I2C_Clk_FW(hGPIO_I2C);
        }
    }  
}
//
//  Start condition
//
VOID I2C_START_FW(HANDLE hGPIO_I2C)
{
    GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);   //S1, SDA = High
    LcdStall(100);                          
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);   //S2, SCL = High ; kept high   
    LcdStall(100);
    
    GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   //S3, When SDA changes High -> Low , start bit generating
    LcdStall(50);                           
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);   //S4, SCL = Low 
    LcdStall(50);                          
}
//
//
// 
VOID InitI2CWithBQ27510FW(HANDLE hGPIO_I2C,char str[])
{
    
    int size, j;
    int convertNum[10] = (int) *str;
    int temp = 0xFF, value;
    UNREFERENCED_PARAMETER(ch);
    I2C_START_FW(hGPIO_I2C);
    size = sizeof(str);
    OALLog(L"size: %d\r\n",size);

    switch(size){
        case 1:
            for(j=0; j<size; j++){
                temp = convertNum[j];       
            }
            break;
        case 2:
            for(j=0; j<size; j++){
                if(j==0)
                    value = convertNum[j<<4];       

                    temp &= convertNum[j];
                    value &= temp;
            }
            break;
        /*case 3:
            for
            
            break;*/


    }  
}
//
//  This function are used in Updating the bq275xx Firmware at Production
//
VOID UpdatingGaugeFW(OAL_BLMENU_ITEM *pMenu)
{
    ULONG rc = (ULONG) BL_ERROR;
    char ch;
    char str[10];
    int i = 0;
    int j;
    HANDLE hGPIO_I2C;
    
    UNREFERENCED_PARAMETER(pMenu);
    OALBLMenuHeader(L"Updating Fuel Gauge Firmware");

    if((hGPIO_I2C = GPIOOpen()) == NULL) 
	{
        RETAILMSG(1,(L"ERROR: BQ27510_init - Open I2C device Failed!!\r\n"));
        goto I2COpenFailed;
	}else{
        RETAILMSG(1,(L"SUCCESS: BQ27510_init - Open I2C device OK!!\r\n"));
	}
	
    GPIOClrBit(hGPIO_I2C,  184);  
    GPIOSetMode(hGPIO_I2C, 184, GPIO_DIR_OUTPUT);   //I2C3_CLK, Ray 140128
    GPIOClrBit(hGPIO_I2C,  185);
    GPIOSetMode(hGPIO_I2C, 185, GPIO_DIR_OUTPUT);   //I2C3_SDA, Ray 140128
         
    rc = BLSDCardDownload(L"BQ275xFW.txt");

    while( BLSDCardReadData(sizeof(char), (UCHAR *)&ch) )
    {
        if( (ch==0x20) || (ch==0x0D) || (ch==0x0A) )    //Space, Carriage Return, Line Feed
        {
            //OALLog(L"--Entry if\r\n");
            if(i >0)
            {
                for(j=0; j<i; j++){
                    OALLog(L"%c", str[j]);       
                }

                if(ch==0x0D)
                    OALLog(L"\r\n");
                else if(ch==0x20){
                    InitI2CWithBQ27510FW(hGPIO_I2C, str);
                    OALLog(L" ");
                }
                i=0;
            }
        }else{
            //OALLog(L"--Entry else\r\n");
            if((ch>'z')||(ch<'0')){
                break;
            }
            str[i++] = ch;      //i = W¡B:¡B1¡B6¡B0¡B0¡B0¡B3
        }
    }
    
    GPIOClose(hGPIO_I2C);
I2COpenFailed:
    return;  
}   
    
    //DWORD size = 10000;
    //UCHAR *pData = NULL;        
    /*if (!SDCardUpdatingFW(L"test.bmp", pData, size))
    //if (!BLSDCardReadLogo(L"test.dffs", pData, size))
    {
        OALMSG(OAL_ERROR, (L"Update fuel gauge firmware wrong!!\r\n"));
    }*/

