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
int g_convertNum[10] = {0};
int size=0;

VOID InitI2CWithBQ27510FW(HANDLE hGPIO_I2C)
{   
    int j;
    int /*temp = 0x0F,*/ value = 0;
    int temp;
    //UNREFERENCED_PARAMETER();
    
    OALLog(L"\tsize: %d\r\n",size);

    I2C_START_FW(hGPIO_I2C);
    switch(size)
    {
        case 1:
            value = *(g_convertNum+0); 
            OALLog(L"OUTPUT:%d.\r\n",value-48);
            /*for(j=0; j<size; j++){
                value = g_convertNum[j]; 
                OALLog(L"----%d", value); 
            }*/
            break;
        case 2:
            /*for(j=0; j<size; j++){
                value = (g_convertNum[j]<<4);       
                if(j!=0){
                    temp &= g_convertNum[j];
                    value |= temp;
                }
                OALLog(L"----%d", value); 
            }*/
            for(j=0; j<size; j++){
                temp = g_convertNum[j];       
                    //for(i=7; i>=0; i--)
                       // value&(1<<);

                if(temp >= 65)
                    temp-=55;
                else
                    temp-=48;
                    
                OALLog(L"OUTPUT:%d.\r\n",temp);
   

                if(j==0){
                    value = temp<<4;
                    OALLog(L"value1=%d.\r\n",value);
                }else{
                    value |= temp;
                    OALLog(L"value2=%d.\r\n",value);
                }
            }

            
            break;
        /*case 3:
            for
            
            break;*/
        default:
            OALLog(L"It is not range.\r\n");
    }  
}
//
//  This function are used in Update the bq275xx Firmware at Production
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
         
    rc = BLSDCardDownload(L"test.txt");

    while( BLSDCardReadData(sizeof(char), (UCHAR *)&ch) )
    {
        if( (ch==0x20) || (ch==0x0D) || (ch==0x0A) )    //Space, Carriage Return, Line Feed
        {
            //OALLog(L"--Entry if\r\n");
            if(i >0)
            {
                for(j=0; j<i; j++){
                    g_convertNum[j] = str[j];
                    OALLog(L"READ=>%c|%d ",str[j], g_convertNum[j]);
                    size = i;
                }
                
                if((ch==0x0D) || (ch==0x20)){
                    InitI2CWithBQ27510FW(hGPIO_I2C);
                }

                /*if(ch==0x0D)
                    OALLog(L"\r\n");
                else if(ch==0x20)
                    OALLog(L" ");*/
                
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

