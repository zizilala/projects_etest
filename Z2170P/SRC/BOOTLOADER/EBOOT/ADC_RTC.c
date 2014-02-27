//------------------------------------------------------------------------------
//  File:  ADC_RTC.c by Ray
//
//
#include <eboot.h>
#include "twl.h"
#include "tps659xx_internals.h"
#include "tps659xx.h"


//------------------------------------------------------------------------------
#define BCD2BIN(b) ( ((b) >> 4)*10 + ((b)&0xF) )
#define RTC_BASE_YEAR_MIN       2000            // must be divisible by 4
#define RTC_BASE_YEAR_MAX       2099


//------------------------------------------------------------------------------
//Device_for_ADCRTC *pDevice = NULL;
DWORD   stall_1Sec = 1000000;
HANDLE  ghGPIO;


//------------------------------------------------------------------------------
VOID ADCRTCInit()
{   
    UCHAR   status;   
    UINT16  channels = 0x4;  // ADCIN2
 
    ghGPIO = GPIOOpen();
    //UNREFERENCED_PARAMETER(pMenu);  
    
    // Backup battery & ADC initial
    GPIOSetBit(ghGPIO, EN_SB_CHARGE);             // Backup battery charge 
    TWLReadByteReg(ghTwl, TWL_GPBR1, &status);  //
    status |= 0x90;                             // MADC_HFCLK_EN & DEFAULT_MADC_CLK_EN
    TWLWriteRegs(ghTwl, TWL_GPBR1, &status, 1);
    TWLWriteRegs(ghTwl, TWL_SW1AVERAGE_LSB, &channels, 2);
    
    // RTC initial, how to setup value
    status = ((1 << 4)|(1 << 2)|(2 << 0)); 		// (1<<4)enable BBCharge, 01:3V ,10:500uA(01:150uA)  
    TWLWriteByteReg(ghTwl, TWL_BB_CFG, status);	
    
}

//------------------------------------------------------------------------------
VOID ReadBBVoltage()        
{
    UCHAR   status;
    UINT16  channels = 0x4;  // ADCIN2
    UINT8   counter = 0;
    UINT16  ADCValue;

    
    //UNREFERENCED_PARAMETER(pMenu);
    GPIOClrBit(ghGPIO, EN_SB_CHARGE);      //Cutoff Main battery
    LcdStall(stall_1Sec*2);
    
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
	TWLReadRegs(ghTwl, TWL_GPCH2_LSB, &ADCValue, sizeof(ADCValue));

	ADCValue = (ADCValue >> 6) * 5000 / 0x3ff;      //UINT16 acquire 10bit, 2.5V*2 = 5000mV, 1024 

    if(ADCValue < 4000)
        GPIOSetBit(ghGPIO, EN_SB_CHARGE); 
    
	OALLog(L"Value: %d mV\r\n", ADCValue);
        
	// power off MADC
	status = 0x00; // MADCON
	TWLWriteRegs(ghTwl, TWL_CTRL1, &status, 1);
}

//------------------------------------------------------------------------------
VOID ReadRTC()
{
    UCHAR status;
	UCHAR bcdTime[6];
    char  showChar[10][30];
    int   i,x;
    char *str;
    
	//UNREFERENCED_PARAMETER(pMenu);

	TWLReadByteReg(ghTwl, TWL_RTC_CTRL_REG, &status);
	status |= TWL_RTC_CTRL_RUN | TWL_RTC_CTRL_GET_TIME; //RTC is running, TC register values are stored in latches
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

   for(i=0,x=3; i<=5; i++, x+2)
   {
      str = (char *)BCD2BIN(bcdTime[i]);
      OALLog(L"i%d, str: %d\n", i, str);
      if(i == 4){
       showChar[i][0] = *str;
      }
       OALLog(L"~~%d\n", showChar[i][0]);
        //else
	    //showChar[i][0] = BCD2BIN(bcdTime[i]);
       printStringMode(8, x, RED_COLOR, GREEN_COLOR, showChar, i);
   }    

   GPIOClose(ghGPIO);
   TWLOpen(ghTwl);
}    



