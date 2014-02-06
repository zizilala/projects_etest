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
//  ACKnowledge are processor(master) sent data after bq27XXX(slave) device must be return a value     
//
VOID I2C_ACKnowledge_FW(HANDLE hGPIO_I2C)
{

    GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_INPUT);      //Get slave a ACK value for SDA line  
    LcdStall(50);
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);
    LcdStall(50); 
    GPIOGetBit(hGPIO_I2C, I2C3_SDA_GPIO); 
    LcdStall(50);

    GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_OUTPUT); 
    //LcdStall(50);
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);   
    LcdStall(50);   
    //Doesnot Needs SDA Line falling time!!
    /*GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO); 
    LcdStall(50);*/
}
//
//  Write CMD
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
//  Stop condition
//
VOID I2C_STOP_FW(HANDLE hGPIO_I2C)
{   
    //GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);
    GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   //P1
    LcdStall(50);
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);   //P2
    LcdStall(50);
    GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);   //P3
    LcdStall(50);    
}
//
//
// 
int  g_value[100];
int  g_total;

VOID InitI2CWithBQ27510FW(HANDLE hGPIO_I2C)
{    
    int i, waitCMD;
    UNREFERENCED_PARAMETER(hGPIO_I2C);    
    
    if( 0x20A == (*(g_value+0)) ){      //Equal "Write Command"
        I2C_START_FW(hGPIO_I2C);
        for(i=1;i<=g_total;i++){
            if(i==1)
                //I2C_setSlaveAddress_FW(hGPIO_I2C, g_value[i]);
                OALLog(L"setSlaveAddress:%#02X ", g_value[i]);
            else
                I2C_WRITE_FW(hGPIO_I2C, g_value[i]);
                OALLog(L"Write:%#02X ", g_value[i]);
                
            //I2C_ACKnowledge_FW(hGPIO_I2C);
            OALLog(L"ACKnowledge\r\n");
         }  
         I2C_STOP_FW(hGPIO_I2C);
    }

    if( 0xCA == (*(g_value+0))){    //Equal "Read and Compare Command", How to send data??
        OALLog(L"C:¡Vindicates that the row is a command to read and compare one or more bytes of data.\r\n");
        /*I2C_START_FW(hGPIO_I2C);
        for(i=1;i<=g_total;i++){
            if(i==1)
                I2C_setSlaveAddress_FW(hGPIO_I2C, g_value[i]);
            else
                I2C_WRITE_FW(hGPIO_I2C, g_value[i]);

            I2C_ACKnowledge_FW(hGPIO_I2C);
        }*/
    }
    
    if( 0x21A == (*(g_value+0))){       //Equal "Wait command"
        waitCMD = g_value[1];
        LcdStall(waitCMD*1000);
        //OALLog(L"Wait CMD:%#02X\r\n",waitCMD);
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
    int  size = 0, j, count = 0;
    int  temp, byte = 0;
    //int  convertNum[10], size=0;         //ASCII convert integer

    HANDLE hGPIO_I2C;
    
    UNREFERENCED_PARAMETER(pMenu);
    OALBLMenuHeader(L"Updating Fuel Gauge Firmware");

    if((hGPIO_I2C = GPIOOpen()) == NULL) 
	{
        RETAILMSG(1,(L"ERROR: BQ27510_init - Open I2C device Failed!!\r\n"));
        goto I2COpenFailed;
	}/*else{
        RETAILMSG(1,(L"SUCCESS: BQ27510_init - Open I2C device OK!!\r\n"));
	}*/
	
    GPIOClrBit(hGPIO_I2C,  184);  
    GPIOSetMode(hGPIO_I2C, 184, GPIO_DIR_OUTPUT);   //I2C3_CLK, Ray 140128
    GPIOClrBit(hGPIO_I2C,  185);
    GPIOSetMode(hGPIO_I2C, 185, GPIO_DIR_OUTPUT);   //I2C3_SDA, Ray 140128
         
    rc = BLSDCardDownload(L"test.txt");
    //rc = BLSDCardDownload(L"BQ275xFW.txt");

    while( BLSDCardReadData(sizeof(char), (UCHAR *)&ch) )
    {
        if( (ch==0x20) || (ch==0x0D) || (ch==0x0A) )    //Space, Carriage Return, Line Feed
        {
            //OALLog(L"--Entry if\r\n");
            if(size >0)
            {            
                /*for(j=0; j<size; j++){
                    convertNum[j] = str[j];         //using "(int)str[j]" to converting data type
                    OALLog(L"Reading=>%c|%d ",str[j], g_convertNum[j]);
                }*/
                //OALLog(L" size: %d\r\n",size);
                //OALLog(L"\r\n");
                
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
                            //temp = convertNum[j];      
                            temp = (int)str[j];
                            
                            if(temp >= 65)
                                temp-=55;
                            else
                                temp-=48;                               
                            //OALLog(L"OUTPUT: %d.\r\n",temp);
                   
                            if(j==0){
                                byte = temp<<4;
                                //OALLog(L"value1=%d.\r\n",value);
                            }else{
                                byte |= temp;
                                //OALLog(L"value2=%d.\r\n",value);
                            }
                         }
                         break;
                       default:
                        OALLog(L"It is not range.\r\n");
                    }
                    //OALLog(L"Output Byte: %#02X.\r\n",byte);

                    /*if(ch==0x0D)
                        OALLog(L"\r\n");
                    else if(ch==0x20)
                        OALLog(L" ");*/
                    size=0;
                }
                
                //if((ch==0x0D) || (ch==0x20)){
                    //InitI2CWithBQ27510FW(hGPIO_I2C);
                //}
                /*if((ch==0x0A)){
                    InitI2CWithBQ27510FW(hGPIO_I2C);
                }*/
            if((ch!=0x0A)){
                g_value[count] = byte;
                g_total = count++;
            }

            if((ch==0x0A)) {
                for(count=0; count<=g_total; count++){
                    OALLog(L"%02X ",g_value[count]);
                }
                OALLog(L"\r\n");
                InitI2CWithBQ27510FW(hGPIO_I2C);
                count = 0;
            }
                    
        }else{
            //OALLog(L"--Entry else\r\n");
            if((ch>'z')||(ch<'0')){
                break;
            }
            str[size++] = ch;      //i = W¡B:¡B1¡B6¡B0¡B0¡B0¡B3
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

