//------------------------------------------------------------------------------
//  File:  ADC_RTC.c by Ray
//
//
#include <eboot.h>
#include "twl.h"
#include "tps659xx_internals.h"
#include "tps659xx.h"

//------------------------------------------------------------------------------
#define BCD2BIN(b) ( ((b) >> 4)*10 + ((b)&0xF) )        // [3:0]*10 + (b)&1111b, Ray 
#define RTC_BASE_YEAR_MIN       2000                    // must be divisible by 4
#define RTC_BASE_YEAR_MAX       2099
#define SIZE                    6
//------------------------------------------------------------------------------
VOID DisplayShowBackground(UCHAR backColor); 

//------------------------------------------------------------------------------
//Device_for_ADCRTC *pDevice = NULL;
DWORD   stall1Sec = 1000000;
HANDLE  ghGPIO;
extern  DWORD gd1Sec;

//------------------------------------------------------------------------------
VOID ADCRTCInit()
{   
    UCHAR   status;   
    UINT16  channels = 0x4;  // ADCIN2
 
    ghGPIO = GPIOOpen();
    //UNREFERENCED_PARAMETER(pMenu);  
    
    // Backup battery & ADC initial
    GPIOSetBit(ghGPIO, EN_SB_CHARGE);              // Backup battery charge enable 

    TWLReadByteReg(ghTwl, TWL_GPBR1, &status);  
    status |= 0x90;                             // MADC_HFCLK_EN & DEFAULT_MADC_CLK_EN
    TWLWriteRegs(ghTwl, TWL_GPBR1, &status, 1);
    TWLWriteRegs(ghTwl, TWL_SW1AVERAGE_LSB, &channels, 2);
    
    // RTC initial
    status = ((1 << 4)|(1 << 2)|(2 << 0)); 	    //BKBAT Charge Enable, 01:3V ,10:500uA(01:150uA)	  
    TWLWriteByteReg(ghTwl, TWL_BB_CFG, status);	
}

//------------------------------------------------------------------------------
VOID ReadBBATVoltage()                                                                                                        
{
    UCHAR   status;
    UINT16  channels = 0x4;  // ADCIN2
    UINT8   counter = 0;
    UINT16  ADCValue;
    char    unit[1][30] = {"mV"};

    //UNREFERENCED_PARAMETER(pMenu);
    GPIOClrBit(ghGPIO, EN_SB_CHARGE);           //Cutoff Main battery
    LcdStall(stall1Sec*3);
       
    //power on MADC
    status = 0x01; // MADC ON
	TWLWriteRegs(ghTwl, TWL_CTRL1, &status, 1);

	// select the channels
	TWLWriteRegs(ghTwl, TWL_SW1SELECT_LSB, &channels, 2);   //Using Asynchronous Conversion by SW1

	//start conversion	
    status = (1<<5);            // TWL_MADC_CTRL_SW_TOGGLE;
	TWLWriteRegs(ghTwl, TWL_CTRL_SW1, &status, 1);

    // waits for conversion complete
    status = 0;
    while(counter++ < 0xFF)
    {
        TWLReadRegs(ghTwl, TWL_CTRL_SW1, &status, 1);
        if(status & 0x02)
        {
            break;
        }
        LcdSleep(10);
    }
	TWLReadRegs(ghTwl, TWL_GPCH2_LSB, &ADCValue, sizeof(ADCValue));     //ADIN2

    /*//First, read backup battery charged for ADCValue, Ray 140303 
	ADCValue = (ADCValue >> 6) * 5000 / 0x3ff;      //UINT16 acquire 10bit, 2.5V*2 = 5000mV, 1024 
    OALLog(L"Backup battery charged value: %d mV\r\n", ADCValue);    

    //Second, EN_SB_CHARGE pins OFF, Ray 140303 
    GPIOClrBit(ghGPIO, EN_SB_CHARGE);           //Cutoff Main battery
    LcdStall(stall1Sec*3);  */ 

    /*ADCValue = 0;                               //Clear value
    TWLReadRegs(ghTwl, TWL_GPCH2_LSB, &ADCValue, sizeof(ADCValue));     //ADIN2*/
    ADCValue = (ADCValue >> 6) * 5000 / 0x3ff;

    DisplayShowBackground(0xFF);
    printInt(1, 11, BLUE_COLOR, TRANSPARENT_COLOR, (int)ADCValue);
    printStringMode(1, 16, BLUE_COLOR, TRANSPARENT_COLOR, unit, 0);
    OALLog(L"Backup battery Value: %d mV\r\n", ADCValue);
    LcdStall(gd1Sec*2);  
    DisplayShowBackground(0xFF);
    
    if(ADCValue < 4000)
        GPIOSetBit(ghGPIO, EN_SB_CHARGE); 
        
    LcdStall(stall1Sec);    
	// power off MADC
	status = 0x00; // MADCON
	TWLWriteRegs(ghTwl, TWL_CTRL1, &status, 1);

    return; 
}

//------------------------------------------------------------------------------
VOID ReadRTC()
{
    UCHAR  status;
	UCHAR  bcdTime[SIZE];
    int    i, col=4, row=2;
    char   ReadBcdTime[10][30] = {  "/",
                                    ":",
                                    "RTC:"
                                 };  
    
	//UNREFERENCED_PARAMETER(pMenu);
	TWLReadByteReg(ghTwl, TWL_RTC_CTRL_REG, &status);
	status |= TWL_RTC_CTRL_RUN | TWL_RTC_CTRL_GET_TIME;     //RTC is running, TC register values are stored in latches, Ray
	TWLWriteByteReg(ghTwl, TWL_RTC_CTRL_REG, status);

    TWLReadByteReg(ghTwl, TWL_YEARS_REG,    &bcdTime[5]);
    TWLReadByteReg(ghTwl, TWL_MONTHS_REG,   &bcdTime[4]);
    TWLReadByteReg(ghTwl, TWL_DAYS_REG,     &bcdTime[3]);
    TWLReadByteReg(ghTwl, TWL_HOURS_REG,    &bcdTime[2]);
    TWLReadByteReg(ghTwl, TWL_MINUTES_REG,  &bcdTime[1]);
    TWLReadByteReg(ghTwl, TWL_SECONDS_REG,  &bcdTime[0]);

    OALLog(L"RTC: %04d-%02d-%02d %02d:%02d:%02d\r\n" , 
            BCD2BIN(bcdTime[5]) + RTC_BASE_YEAR_MIN,
			BCD2BIN(bcdTime[4]), BCD2BIN(bcdTime[3]) , BCD2BIN(bcdTime[2]),
			BCD2BIN(bcdTime[1]), BCD2BIN(bcdTime[0]));

   
   //print at screen
   printStringMode(row, col, BLUE_COLOR, TRANSPARENT_COLOR, ReadBcdTime, 2); 
   col +=5;
   for(i=5; i>=0; i--)
   {    
        printInt(row, col, BLUE_COLOR, TRANSPARENT_COLOR,  (0xFF&(int)BCD2BIN(bcdTime[i])) );
        if(i > 3){
            printStringMode(row, col+=2, BLUE_COLOR, TRANSPARENT_COLOR, ReadBcdTime, 0); 
        }else if(i <3 && i > 0){
            printStringMode(row, col+=2, BLUE_COLOR, TRANSPARENT_COLOR, ReadBcdTime, 1); 
        }

        if(i == 3) col++;

        col+=1;    
    }

   GPIOClose(ghGPIO);
   TWLOpen(ghTwl);
}    

