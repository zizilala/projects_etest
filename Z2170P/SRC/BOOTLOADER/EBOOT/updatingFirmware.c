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
    
}
//  ACKnowledge are processor(master) sent data after bq27XXX(slave) device must be return a value     
//
VOID I2C_ACKnowledge_FW(HANDLE hGPIO_I2C)
{
    //GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);
    //LcdStall(1);
    GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_INPUT);      //Get slave a ACK value for SDA line  
    LcdStall(1);                                             
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);
    LcdStall(50);
    GPIOGetBit(hGPIO_I2C, I2C3_SDA_GPIO); 
    LcdStall(50);
        
    GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_OUTPUT); 
    LcdStall(1);
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);   
    LcdStall(50);
    //GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);
    //LcdStall(1);
}
//  Write Data
//
VOID I2C_WRITE_FW(HANDLE hGPIO_I2C, int data)
{
    int i;
    for(i=7; i>=0; i--)
    {
        if( data&(1<<i) ){
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
//  Start condition
//
VOID I2C_START_FW(HANDLE hGPIO_I2C)
{
    GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);   //S1, SDA = High
    LcdStall(1);                          
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);   //S2, SCL = High ; kept high   
    LcdStall(100);
    
    GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   //S3, When SDA changes High -> Low , start bit generating
    LcdStall(10);                           
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);   //S4, SCL = Low 
    LcdStall(50);                          
}
//  Stop condition
//
VOID I2C_STOP_FW(HANDLE hGPIO_I2C)
{   
    GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   //P1
    LcdStall(50);
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);   //P2
    LcdStall(50);
    GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);   //P3
    LcdStall(50);    
}
//
// 
int  g_value[100];
int  g_total;
//
VOID InitI2CWithBQ27510FW(HANDLE hGPIO_I2C)
{    
    int i, waitCMD;
    UNREFERENCED_PARAMETER(hGPIO_I2C);    
    
    if( 0x20A == (*(g_value+0)) ){      //Equal "Write Command"
        I2C_START_FW(hGPIO_I2C);        
        for(i=1;i<=g_total;i++){
            if(i==1){ 
                I2C_setSlaveAddress_FW(hGPIO_I2C, g_value[i]);  
                //OALLog(L"setSlaveAddress:%#02X ", g_value[i]);
                //I2C_setSlaveAddress_FW(hGPIO_I2C, 0XAA);
            }else{
                I2C_WRITE_FW(hGPIO_I2C, g_value[i]);    
                //OALLog(L"Write:%#02X ", g_value[i]);
            }

            LcdStall(100);    
            I2C_ACKnowledge_FW(hGPIO_I2C);              //OALLog(L"ACKnowledge\r\n");                 
            LcdStall(200);
         } 
         LcdStall(250);
    }

    if( 0xCA == (*(g_value+0))){    //Equal "Read and Compare Command", How to send data??
        I2C_START_FW(hGPIO_I2C);
        for(i=1;i<=g_total;i++){
            if(i==1)
                I2C_setSlaveAddress_FW(hGPIO_I2C, g_value[i]);
            else
                I2C_WRITE_FW(hGPIO_I2C, g_value[i]);

            LcdStall(100); 
            I2C_ACKnowledge_FW(hGPIO_I2C);
            LcdStall(200);
        }
        LcdStall(250);
        //OALLog(L"C:�Vindicates that the row is a command to read and compare one or more bytes of data.\r\n");
    }
    
    if( 0x21A == (*(g_value+0))){       //Equal "Wait command"
        waitCMD = g_value[1];
        LcdStall(waitCMD*2000);   //Because 1us at oscilloscpoe are 0.8us(x1.25), So => value*1.25*ms        
        //OALLog(L"WaitCMD:%d\r\n",waitCMD);
    }
}
//  This function are used in Update the bq275xx Firmware at Production
//
VOID UpdatingGaugeFW(OAL_BLMENU_ITEM *pMenu)
{
    ULONG rc = (ULONG) BL_ERROR;
    char ch;
    char str[10];
    int  size = 0, j, count = 0;
    int  temp, byte = 0;
    HANDLE hGPIO_I2C;
    
    OALBLMenuHeader(L"Updating Fuel Gauge Firmware");
    UNREFERENCED_PARAMETER(pMenu);
    
    if((hGPIO_I2C = GPIOOpen()) == NULL) 
	{
        RETAILMSG(1,(L"ERROR: BQ27510_init - Open I2C device Failed!!\r\n"));
        goto I2COpenFailed;
	}
	
    GPIOClrBit(hGPIO_I2C,  184);  
    GPIOSetMode(hGPIO_I2C, 184, GPIO_DIR_OUTPUT);   //I2C3_CLK, Ray 
    GPIOClrBit(hGPIO_I2C,  185);
    GPIOSetMode(hGPIO_I2C, 185, GPIO_DIR_OUTPUT);   //I2C3_SDA, Ray
         
    rc = BLSDCardDownload(L"test.txt");
    //rc = BLSDCardDownload(L"BQ275xFW.txt");

    while( BLSDCardReadData(sizeof(char), (UCHAR *)&ch) )
    {
        if( (ch==0x20) || (ch==0x0D) || (ch==0x0A) )    //Space, Carriage Return, Line Feed
        {
            if(size >0)
            {            
                switch(size)
                {
                    case 1:
                        byte = (int)(*(str+0));
                        
                        if(byte >= 65)
                            byte-=55;
                        else
                            byte-=48;  
                        break;   
                    case 2:
                        for(j=0; j<size; j++)
                        {    
                            temp = (int)str[j];
                            
                            if(temp >= 65)
                                temp-=55;
                            else
                                temp-=48;                               
                   
                            if(j==0){
                                byte = temp<<4;
                            }else{
                                byte |= temp;
                            }
                         }
                         break;
                    default:
                        OALLog(L"It is not range.\r\n");
                 }
                size=0;
            }
                
            if( ch!=0x0A){
                g_value[count] = byte;
                g_total = count++;
            }

            if( ch==0x0A){
                for(count=0; count<=g_total; count++){
                    OALLog(L"%02X ",g_value[count]);
                }
                OALLog(L"\r\n");
                InitI2CWithBQ27510FW(hGPIO_I2C);
                count = 0;
            }             
        }else{
            if((ch>'z')||(ch<'0')){
                break;
            }
            str[size++] = ch;      //i = (W�B:), (1�B6), (0�B0), (0�B3)
        }
    }
    I2C_STOP_FW(hGPIO_I2C);
    GPIOClose(hGPIO_I2C);
I2COpenFailed:
    return;  
}   

