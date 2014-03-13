//------------------------------------------------------------------------------
//  File:  functionTest.c , by Ray
//
//
#include <eboot.h>
#include "sdk_i2c.h"
#include "bsp.h"
#include "bq27xxx_new.h"
#include "omap.h"
#include "oal_clock.h"          //
//#include "tps659xx_audio.h"
#include "tps659xx_internals.h"
#include "twl.h"                        //Use TWLReadRegs() and so on, Ray 140102

//------------------------------------------------------------------------------
//  Preprocessor Defination
//
//#define BSP_Z2170P      2170 
//#define BSP_Z2000       2000 
#define BK_SET_GPIO             61
#define KP_LED_SET_GPIO         155     //Keypad LED ,Low-active
#define I2C3_SCL_GPIO           184
#define I2C3_SDA_GPIO           185

#define BARCODE_LED_SET_GPIO    160     // high-active
#define GREEN_LED_SET_GPIO      34      // high-active
#define RED_LED_SET_GPIO        140     // high-active

//Barcode set-up
/*#define BCR_ENG_TRIG			36	    // nENG_TRIG(low-active)
#define BCR_ENG_PWEN			38	    // ENG_PWEN			
#define ENG_SET1_GPIO			152	    // ENG_SET1(IN)		//determine machine state
#define ENG_SET2_GPIO			153 	// ENG_SET2(IN)*/

#define MIC_PWR_EN              74
#define SPK_EN                  37
#define KEYPAD_SIZE             26

//------------------------------------------------------------------------------
//  Function Prototype
//
VOID AllFunctionTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID DisplayTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID BkTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID DRAMTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID KeypadBkTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID AllFunctionTest_Z2000(OAL_BLMENU_ITEM *pMenu);
VOID DisplayTest_Z2000(OAL_BLMENU_ITEM *pMenu);
VOID TouchPanelTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID BatteryTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID LEDTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID BarcodeTest_Z2170P(OAL_BLMENU_ITEM *pMenu);   
VOID AudioAndMIC_Z2170P(OAL_BLMENU_ITEM *pMenu); 
VOID KeypadFunc_Z2170P(OAL_BLMENU_ITEM *pMenu); 
VOID BurnIn_Z2170P(OAL_BLMENU_ITEM *pMenu);
VOID RAMAccessTest(OAL_BLMENU_ITEM *pMenu);
VOID AutoScanFunc(OAL_BLMENU_ITEM *pMenu);
VOID SecondaryBATFunc(OAL_BLMENU_ITEM *pMenu);
VOID RTCFunc(OAL_BLMENU_ITEM *pMenu);

// Invoke Extenal Functions
BOOL DisplayShow(void);
VOID SetBacklight(void);
VOID tsc2046Test(void);
VOID DisplayShowBackground(UCHAR backColor);        //Ray   140218
VOID ReadBBATVoltage(void);
VOID ReadRTC(void);

//VOID SetKeypadBacklight(void);

//------------------------------------------------------------------------------
//  Global variable
//
extern HANDLE ghTwl;
extern char   gTestPass[][30];  
extern DWORD  gd1Sec;
extern DWORD  gd3Sec;
UINT8   gKeypadMatrix[8];
DWORD   sleep1Sec = 1000;         //Lcd Sleep 
OMAP_UART_REGS *pUartRegs = NULL;

//------------------------------------------------------------------------------
//
//
/*typedef struct _LCM_BACKLIGHT {       //Ray 131107
	   DWORD       bkActive;
       LPCWSTR     bkName;
} LCM_BACKLIGHT;

LCM_BACKLIGHT lcmBacklight[] = {
    {BK_TEST,   L"Brightness Testing"},
    {BK_ON,     L"ON"},
    {BK_OFF,    L"OFF"},
    {BK_EXIT,   L"EXIT"},
};*/

//------------------------------------------------------------------------------
//
OAL_BLMENU_ITEM g_menu2170PTest[] = {
    {
        L'1', L"All Function Test", AllFunctionTest_Z2170P,
        NULL, NULL, NULL
    }, {
        L'2', L"Display Test", DisplayTest_Z2170P,
        NULL, NULL, NULL
    }, {
        L'3', L"LCM Backlight Test", BkTest_Z2170P,
        NULL, NULL, NULL
    }, {
        L'4', L"DRAM Test", DRAMTest_Z2170P,
        NULL, NULL, NULL
    }, {
        L'5', L"Keypad Backlight Test", KeypadBkTest_Z2170P,
        NULL, NULL, NULL
    }, {
        L'6', L"Touch Panel Test", TouchPanelTest_Z2170P,
        NULL, NULL, NULL
    },  {
        L'7', L"Battery Test", BatteryTest_Z2170P,
        NULL, NULL, NULL
    },  {
        L'8', L"LED Indicator", LEDTest_Z2170P,
        NULL, NULL, NULL
    }, {
        L'9', L"Barcode Scanning", BarcodeTest_Z2170P,
        NULL, NULL, NULL
    },/* {
        L'a', L"Audio and MIC", AudioAndMIC_Z2170P,
        NULL, NULL, NULL
    },*/{
        L'a', L"Keypad functional", KeypadFunc_Z2170P,
        NULL, NULL, NULL
    }, {
        L'b', L"Burn-In", BurnIn_Z2170P,
        NULL, NULL, NULL
    },  {
        L'c', L"RAM Access Test(Completely)", RAMAccessTest,
        NULL, NULL, 0
    }, {
        L'd', L"Auto Scan", AutoScanFunc,
        NULL, NULL, 0
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    },  {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};

OAL_BLMENU_ITEM g_menuZ2000Test[] ={
    {
        L'1', L"All Function Test", AllFunctionTest_Z2000,
        NULL, NULL, NULL
    }, {
        L'2', L"Display Test", DisplayTest_Z2000,
        NULL, NULL, NULL
    }, {
        L'0', L"Exit and Continue", NULL,
        NULL, NULL, NULL
    },  {
        0, NULL, NULL,
        NULL, NULL, NULL
    }
};

//==============================================================================
//  Running Z2170P...
//
VOID AllFunctionTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    OAL_BLMENU_ITEM *ptr = NULL;
	//ALL Test
	UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"All Function Test");
    
    OALLog(L"\r\n >>>All function testing...\r\n");    

	DisplayTest_Z2170P(ptr);
	LcdStall(gd1Sec*2);                         //2
  
	BkTest_Z2170P(ptr);
	LcdStall(gd1Sec*2);                         //3
         
	DRAMTest_Z2170P(ptr);                                  
	LcdStall(gd1Sec*2);                         //4 
	   
	KeypadBkTest_Z2170P(ptr);                              
	LcdStall(gd1Sec*2);                         //5 

	TouchPanelTest_Z2170P(ptr);                 //6 
    LcdStall(gd1Sec*2);
    
    BatteryTest_Z2170P(ptr);                    //7 
    LcdStall(gd1Sec*2);
    
    LEDTest_Z2170P(ptr);                        //8
    LcdStall(gd1Sec*2);
    
    BarcodeTest_Z2170P(ptr);                    //9 
    LcdStall(gd1Sec*2);
    
    /*AudioAndMIC_Z2170P(ptr);                  //
    LcdStall(gd1Sec*2);*/
    
    KeypadFunc_Z2170P(ptr);                      //F1
    LcdStall(gd1Sec*2);
    
    /*BurnIn_Z2170P(ptr);                       //
    LcdStall(gd1Sec*2);*/

    SecondaryBATFunc(ptr);                      //SP
    LcdStall(gd1Sec*2);
    
    RTCFunc(ptr);                               //SF
	LcdStall(gd1Sec*2);
}

//------------------------------------------------------------------------------
//  Red-Green-Blue-Black-White, Ray 131107
//
VOID DisplayTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    //BLShowLogo();
    OALBLMenuHeader(L"Display Test");
	UNREFERENCED_PARAMETER(pMenu);
    OALLog(L"\r\n Running...\r\n");
	DisplayShow();
	
	OALLog(L"\rTested ok!! \r\n");
}

//------------------------------------------------------------------------------
//  Show brightness 0% ~ 100%, ON and OFF,  Ray 131111
//
VOID BkTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    OALBLMenuHeader(L"LCM Backlight Test");
	UNREFERENCED_PARAMETER(pMenu);
	
	DisplayShowBackground(0xFF);
    SetBacklight();
    //BLShowLogo();
    //
    DisplayShowBackground(0xFF);
    printStringMode(9, 9, RED_COLOR, GREEN_COLOR, gTestPass, 0);
	LcdStall(gd1Sec*2);
    OALLog(L"\rTested ok!! \r\n");
    DisplayShowBackground(0xFF);
}
//
//
VOID SetBacklight()
{
    HANDLE hGPIO;
    //WCHAR key;
    int i, value, k, scale = 0;    
    int AAT3123Code[11] = { 1, 4 , 7, 10, 13, 16, 19, 22, 25, 28, 32};
                          //0  10  20  30 40  50  60  70  80  90  100
   
    hGPIO = GPIOOpen();

    GPIOClrBit(hGPIO, BK_SET_GPIO);
    LcdStall(gd1Sec); 
            
    for(i=1, k=0; i<=32; i++)
    {
        if(i == (value = AAT3123Code[k]))
        {
            GPIOClrBit(hGPIO, BK_SET_GPIO);
            LcdStall(1);
            GPIOSetBit(hGPIO, BK_SET_GPIO);
            //
            DisplayShowBackground(0xFF);
            printInt(9, 14, BLUE_COLOR, TRANSPARENT_COLOR, scale);
            LcdStall(gd1Sec);
            OALLog(L"\r !Plus after brightness scale at %d %%\r\n",scale);
            scale+=10;
            k++;
        }else{
            GPIOClrBit(hGPIO, BK_SET_GPIO);
            LcdStall(1);
            GPIOSetBit(hGPIO, BK_SET_GPIO);
            LcdStall(1);
        }
    }   
    GPIOClose(hGPIO);  
}

//------------------------------------------------------------------------------
//  Check DRAM start addressing ~ end addressing, Ray 131113
//  a. detail, b. quick
//  
//  CS0-SDRAM ranges at 0x8000_0000 ~ 0x9fff_ffff (512MByts), 
//  but in "oalAddressTable" memory allocation SIZE 256MB and
//  start addressing at 0x8000_0000, so... 
//
//     0x8FFF_FFFF - 0x8000_0000
//   = 0x0FFF_FFFF(28bit)
//   = 256MBtyes
//
//------------------------------------------------------------------------------
//
VOID DRAMTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    ULONG  startAddress = IMAGE_SHARE_ARGS_CA;
    ULONG  endAddress = startAddress|(256 *1024 *1024-1); //endAddress = startAddress + size(256MB) 
    ULONG   test1_Address = (startAddress|0x0000FFFF);      //
    //ULONG   test2_Address = (startAddress|0x0FFFFF00);
    //ULONG   ebootAddress = IMAGE_EBOOT_CODE_CA ;
    ULONG   i, testAddress =  0xFFFFFFFF;
    BYTE    value, temp, y=0, pattern = 0x1A; 
    //ULONG   percentRange;
    ULONG   percent, tip;        
    char    result[][30] = { "Scanning size per 1KB tip:",
                             "*",
                             "Failure"
                           };
    
	UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"DRAM Test");

	//percentRange = (endAddress - startAddress)/100;
    OALLog(L"\r\n !Start Address: 0X%08X", (startAddress&testAddress));
    OALLog(L"\r\n !Test1 Address: 0X%08X", (test1_Address&testAddress));
    //OALLog(L"\r\n !Test2 Address: 0X%08X", (test2_Address&test));
    OALLog(L"\r\n !End   Address: 0X%08X", (endAddress&testAddress));
    OALLog(L"\r\n------------------------------------------\n");
    //
    DisplayShowBackground(0xFF);
    printStringMode(y, 0, RED_COLOR, TRANSPARENT_COLOR, result, 0);
	/*for(i=0, percent=0; i<(test1_Address - startAddress); i++)
	{
        temp = *(volatile BYTE *)(startAddress + i);        //read data     ;temp(0x8000_0000) to get a address 
        *(volatile BYTE *)(startAddress + i) = pattern;     //write pattern ;pattern write into address(1A) 
        value = *(volatile BYTE *)(startAddress + i);       //read pattern  ;value == 1A, setup value
        *(volatile BYTE *)(startAddress + i) = temp;        //store data

        if(value != pattern)                                //check value(in address value) equal pattern
        {
            OALLog(L"\r\n Error Address: 0X%X", startAddress+i);
        }else{
            percent++;
            OALLog(L"\r\n Decoding Memoey percent: %d >>>  Address: 0X%X",percent, startAddress+i);   
            //OALLog(L"\r\n Address: 0X%X", startAddress+i);
        }

        if( (percent%10) == 0){
            OALLog(L"\r\n-----------------------------------\n");
            LcdStall(gd1Sec); 
        }
	}
    OALLog(L"\r\n");
    LcdStall(gd1Sec*3);*/

    //OALLog(L"\r\n");
    for(i=0, percent=1; i<(test1_Address - startAddress); i++, percent++)
	{
        temp = *(volatile BYTE *)(startAddress + i);        //read data     ;temp(0x8000_0000) to get a address 
        *(volatile BYTE *)(startAddress + i) = pattern;     //write pattern ;pattern write into address(1A) 
        value = *(volatile BYTE *)(startAddress + i);       //read pattern  ;value == 1A, setup value
        *(volatile BYTE *)(startAddress + i) = temp;        //store data
        
        if(value != pattern)                                //check value(in address value) equal pattern
        {
            OALLog(L"\r\n Error Address: 0X%X", startAddress+i);
            DisplayShowBackground(0xFF);
            printStringMode(0, 0, RED_COLOR, GREEN_COLOR, result,2);
            LcdStall(gd1Sec/1000);
        }/*else{
            OALLog(L"*");
            //percent++;
            //OALLog(L"\r\n Decoding Memoey percent: %d >>>  Address: 0X%X",percent, test2_Address+i);   
            //OALLog(L"\r\n Address: 0X%X", startAddress+i);
        }*/

        if( (percent%1024) == 0){   //scan 1kB show tip 
            OALLog(L"\rScanned size: %dk\r\n",percent/1024);
            //
            tip = (percent/1024-1);

            if((tip%30) == 0) ++y;
            LcdStall(gd1Sec/2);
            printStringMode(y, 0+tip, RED_COLOR, TRANSPARENT_COLOR, result, 1);
            /*else
                printStringMode(y+1, 0+tip, RED_COLOR, TRANSPARENT_COLOR, result, 1);*/ 
        }

        /*if(i == ((test1_Address - startAddress)-1)){   //scan 1kB show tip 
            result[i][30] = "Scanned size: 64KB...";
            DisplayShowBackground(0xFF);
            printStringMode(19, 1, RED_COLOR, GREEN_COLOR, result, i);
            LcdStall(gd1Sec/1000); 
        }*/
	}
	OALLog(L"\r\n");
    OALLog(L"\rTested ok!! \r\n");
    
    //
    DisplayShowBackground(0xFF);
    printStringMode(9, 9, RED_COLOR, GREEN_COLOR, gTestPass, 0);
    LcdStall(gd1Sec*2);
    DisplayShowBackground(0xFF);
	/*for(i=0, Round=1; i<0xFF; i++)                         //Testing eboot, Ray  131113
    {
        if(i==65)
        {
            temp = *(volatile BYTE *)(startAddress + i);        //read data     ;temp(0x8000_0000) to get a address 
            *(volatile BYTE *)(startAddress + i) = pattern;     //write pattern ;pattern write into address(1A) 
            value = *(volatile BYTE *)(startAddress + i);       //read pattern  ;value == 1A, setup value
            *(volatile BYTE *)(startAddress + i) = temp;        //store data   
            if(value != pattern)                                //check value(in address value) equal pattern
            {
                OALLog(L"\r\n Error Address: 0X%X", (ebootAddress+i));
            }
        }

        OALLog(L"\r\n Eboot Address: 0x%08x, Value = 0x%08x", (ebootAddress+i),*(volatile BYTE *)(ebootAddress+i));
        
        if((Round%16)== 0)
        {
            OALLog(L"\r\n Round %d",Round);
            OALLog(L"\r\n---------------------\n"); 
            Round++;
        }
    }*/
}

//------------------------------------------------------------------------------
//  The keypad Backlight Test, LED flash twice, Ray 131112
//
VOID KeypadBkTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
	DWORD delay = 1000;
	HANDLE hGPIO;
	UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"Keypad Backlight Test");

    hGPIO = GPIOOpen();
       
    GPIOSetBit(hGPIO, KP_LED_SET_GPIO);
    LcdStall(100);
    GPIOClrBit(hGPIO, KP_LED_SET_GPIO);         //Low-active
    OALLog(L"\r !Now Keypad LED first flash \r\n");
    LcdSleep(delay*2);
  
    GPIOSetBit(hGPIO, KP_LED_SET_GPIO);
    LcdStall(gd1Sec);
    GPIOClrBit(hGPIO, KP_LED_SET_GPIO);
    OALLog(L"\r !Now Keypad LED twice flash \r\n");
    LcdSleep(delay*2);

    GPIOSetBit(hGPIO, KP_LED_SET_GPIO);         //Keypad LED liht Close 

    GPIOClose(hGPIO);
    DisplayShowBackground(0xFF);
    printStringMode(9, 9, RED_COLOR, GREEN_COLOR, gTestPass, 0);
    LcdStall(gd1Sec*2);
    DisplayShowBackground(0xFF);
    OALLog(L"\rTested ok!! \r\n");
}

//------------------------------------------------------------------------------
//  The Touch Panel Test, Ray 131120
//
VOID TouchPanelTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);
    OALBLMenuHeader(L"Touch Panel Test");
    
    OALLog(L"\r Touch Panel Running...\r\n");  
    tsc2046Test();
    DisplayShowBackground(0xFF);
    printStringMode(9, 9, RED_COLOR, GREEN_COLOR, gTestPass, 0);
    LcdStall(gd1Sec*2);
    DisplayShowBackground(0xFF);
    OALLog(L"\rTested ok!! \r\n"); 
}

//------------------------------------------------------------------------------
//  The Battery Test, Ray 131128
//
//
//
VOID I2C_Clk(HANDLE hGPIO_I2C)
{
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);  
    LcdStall(50);
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO); 
    //LcdStall(50);
}
//
//  ACKnowledge are processor(master) sent data after bq27XXX(slave) device must be return a value     
//
VOID I2C_ACKnowledge(HANDLE hGPIO_I2C)
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
//  
//
VOID I2C_ACKnowledge_READ(HANDLE hGPIO_I2C)
{       

    //GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_INPUT);      //Get slave a ACK value for SDA line  
    //LcdStall(50);
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);
    LcdStall(50); 
    GPIOGetBit(hGPIO_I2C, I2C3_SDA_GPIO); 
    LcdStall(50);
    
    //GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_OUTPUT); 
    //LcdStall(50);
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);   
    LcdStall(50);         
}
//
//
//
int I2C_Clk_READ(HANDLE hGPIO_I2C)
{
    DWORD data=0;
    
    GPIOSetBit(hGPIO_I2C, I2C3_SCL_GPIO);
    LcdStall(50);
    data = GPIOGetBit(hGPIO_I2C, I2C3_SDA_GPIO); 
    LcdStall(10);
    
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);   
    LcdStall(50); 
    
    return (int)data;
}
//
//  Read Data
//
int I2C_READ(HANDLE hGPIO_I2C)
{
    int i, rx, value;
    
    GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_INPUT);      //Get slave a ACK value for SDA line  
    LcdStall(50);
    for(i=7, value=0; i>=0; i--)
    {
        rx = I2C_Clk_READ(hGPIO_I2C); 
        value += (rx<<i);
    }
    
    GPIOSetMode(hGPIO_I2C, I2C3_SDA_GPIO, GPIO_DIR_OUTPUT);      
    GPIOClrBit(hGPIO_I2C, I2C3_SCL_GPIO);    
    GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO); 
    //LcdStall(50); 
    
    return value;
}
//
//  Write CMD
//
VOID I2C_WRITE(HANDLE hGPIO_I2C, int CMD)
{
    int i;
    for(i=7; i>=0; i--)
    {
        if( CMD&(1<<i) ){
            GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);  
            LcdStall(50);
            I2C_Clk(hGPIO_I2C);
        }else{
            GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   
            LcdStall(50);
            I2C_Clk(hGPIO_I2C);
        }
    }           
}
//
//
//
VOID I2C_setSlaveAddress_READ(HANDLE hGPIO_I2C, int addr)
{    
    int i; 
    for(i=7; i >=0; i--)
    {         
        if( addr&(1<<i) ){
            GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);  
            LcdStall(50);
            I2C_Clk(hGPIO_I2C);
        }else{
            GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   
            LcdStall(50);
            I2C_Clk(hGPIO_I2C);
        }
    }
    GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO); 
    LcdStall(50);
}
//
//  Set-up device Slave Addressing  
//
VOID I2C_setSlaveAddress(HANDLE hGPIO_I2C, int addr)
{    
    int i;  
    for(i=7; i >=0; i--)
    {         
        if( addr&(1<<i) ){
            GPIOSetBit(hGPIO_I2C, I2C3_SDA_GPIO);  
            LcdStall(50);
            I2C_Clk(hGPIO_I2C);
        }else{
            GPIOClrBit(hGPIO_I2C, I2C3_SDA_GPIO);   
            LcdStall(50);
            I2C_Clk(hGPIO_I2C);
        }
    }  
}
//
//  Start condition
//
VOID I2C_START(HANDLE hGPIO_I2C)
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
VOID I2C_STOP(HANDLE hGPIO_I2C)
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
int gaugeInformation(HANDLE hGPIO_I2C,int stdCMD)
{
    int LSB = 0, MSB = 0; 
    int i, temp, sum, a[16];

    LcdStall(12500);
    //*************************
    I2C_START(hGPIO_I2C);  
    I2C_setSlaveAddress(hGPIO_I2C, BQ27510_ADDRESS_WRITE);   //ADDR[7:1] + R/W[0] 
    LcdStall(100);    
    I2C_ACKnowledge(hGPIO_I2C);
    
    I2C_WRITE(hGPIO_I2C,  stdCMD);              //CMD[7:0]
    //I2C_WRITE(hGPIO_I2C, bq27500CMD_VOLT_LSB);
    LcdStall(300);                                          //100 200 500-ok 50, Maybe trouble        
    I2C_ACKnowledge(hGPIO_I2C);
    //*************************
     
    //Sr
    LcdStall(1000); 

    //*************************    
    I2C_START(hGPIO_I2C);
    I2C_setSlaveAddress_READ(hGPIO_I2C, BQ27510_ADDRESS_READ);
    LcdStall(300);  
    I2C_ACKnowledge(hGPIO_I2C);

    //prepare get Data LSB
    LcdStall(300); 
    LSB  = I2C_READ(hGPIO_I2C);
    LcdStall(300);
    I2C_ACKnowledge_READ(hGPIO_I2C);

    //prepare get Data MSB
    LcdStall(1000); 
    MSB = I2C_READ(hGPIO_I2C);
    LcdStall(300);
    I2C_ACKnowledge_READ(hGPIO_I2C);
    LcdStall(500);
    I2C_STOP(hGPIO_I2C);
    //*************************
    
    //RETAILMSG(1, (L" ~Value = 0x%02x 0x%02x \r\n",MSB ,LSB));

    temp = (MSB<<8);
    temp |= LSB;

    for(i=0, sum=0; i<16; i++){
		if(i<8)
			a[i] = temp & (1<<i);
		else
			a[i] = temp & (1<<i);
			
		sum += a[i];
	}
    return sum;
}
//
//  Initialize I2C interface with BQ27510 communications
//
VOID InitI2CWithBQ27510(HANDLE hGPIO_I2C)
{
    bq_dataram data;
    int  LSB = 0, MSB = 0;
    int  row = 1, col = 3;
    char unit[10][30] = {"Battery Voltages: ",
                         "Battery Temp: ",
                         "C",
                         "mV"
                         };
    //int i, temp, sum, a[16];
    
    //*************************
    I2C_START(hGPIO_I2C);  
    I2C_setSlaveAddress(hGPIO_I2C, BQ27510_ADDRESS_WRITE);   //ADDR[7:1] + R/W[0] 
    LcdStall(100);    
    I2C_ACKnowledge(hGPIO_I2C);
    
    I2C_WRITE(hGPIO_I2C,  bq27500CMD_CNTL_LSB);              //CMD[7:0] 
    LcdStall(300);                                          
    I2C_ACKnowledge(hGPIO_I2C);

    I2C_WRITE(hGPIO_I2C,  0x01);          
    LcdStall(300);                                                  
    I2C_ACKnowledge(hGPIO_I2C);

    I2C_WRITE(hGPIO_I2C,  0x00);          
    LcdStall(300);                                                  
    I2C_ACKnowledge(hGPIO_I2C);
    I2C_STOP(hGPIO_I2C);
    //*************************
    
    LcdStall(1000); 
    //*************************    
    I2C_START(hGPIO_I2C);                                    //Sr 
    I2C_setSlaveAddress(hGPIO_I2C, BQ27510_ADDRESS_WRITE);   //ADDR[7:1] + R/W[0] 
    LcdStall(100);    
    I2C_ACKnowledge(hGPIO_I2C);
    
    I2C_WRITE(hGPIO_I2C,  bq27500CMD_CNTL_LSB);              //CMD[7:0] 
    LcdStall(300);                                         
    I2C_ACKnowledge(hGPIO_I2C);

    I2C_START(hGPIO_I2C);
    I2C_setSlaveAddress_READ(hGPIO_I2C, BQ27510_ADDRESS_READ);
    LcdStall(300);  
    I2C_ACKnowledge(hGPIO_I2C);

    //prepare get Data LSB
    LcdStall(300); 
    LSB  = I2C_READ(hGPIO_I2C);
    LcdStall(300);
    I2C_ACKnowledge_READ(hGPIO_I2C);

    //prepare get Data MSB
    LcdStall(1000); 
    MSB = I2C_READ(hGPIO_I2C);
    LcdStall(300);
    I2C_ACKnowledge_READ(hGPIO_I2C);
    LcdStall(500);
    I2C_STOP(hGPIO_I2C);
    //*************************
    
    
    I2C_START(hGPIO_I2C);
    I2C_STOP(hGPIO_I2C);
    gaugeInformation(hGPIO_I2C,  0x00);                 //This opreating means clean register
    data.Temp =(short) gaugeInformation(hGPIO_I2C,  bq27500CMD_TEMP_LSB);
    /*I2C_START(hGPIO_I2C);
    I2C_STOP(hGPIO_I2C);
    gaugeInformation(hGPIO_I2C,  0x00);*/
    data.Voltage =(short) gaugeInformation(hGPIO_I2C,  bq27500CMD_VOLT_LSB);
    gaugeInformation(hGPIO_I2C,  0x00);        
    //data.NomAvailCap =(short) gaugeInformation(hGPIO_I2C,  bq27500CMD_NAC_LSB);
    //gaugeInformation(hGPIO_I2C,  0x00);

    DisplayShowBackground(0xFF);
    printStringMode(row, col, BLUE_COLOR, TRANSPARENT_COLOR, unit, 0); 
    printInt(row, col+=18, RED_COLOR, TRANSPARENT_COLOR,  ((int)data.Voltage) );
    printStringMode(row, col+=5, BLUE_COLOR, TRANSPARENT_COLOR, unit, 3);
    //
    col = 3;
    printStringMode(++row, col, BLUE_COLOR, TRANSPARENT_COLOR, unit, 1);
    printInt(row, col+=14, RED_COLOR, TRANSPARENT_COLOR,  ((int)(data.Temp/10-273)) );
    printStringMode(row, col+=3, BLUE_COLOR, TRANSPARENT_COLOR, unit, 2);
    LcdStall(gd1Sec*3);  
    DisplayShowBackground(0xFF);  
    
    OALLog(L" ~Reports the device type:0x%02X%02X \r\n",MSB ,LSB);
    OALLog(L" ~Battery Temperature = %d C\r\n",(data.Temp/10)-273);
    OALLog(L" ~Battery Voltages = %d mV\r\n",data.Voltage);
    //OALLog(L" ~Battery Nominal Available Capacity = %d mAh\r\n",data.NomAvailCap);
}
//
//
//
VOID BatteryTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    HANDLE hGPIO_I2C;
    
    UNREFERENCED_PARAMETER(pMenu);
    OALBLMenuHeader(L"Battery Test");
    
    if((hGPIO_I2C = GPIOOpen()) == NULL) 
	{
        RETAILMSG(1,(L"ERROR: BQ27510_init - Open I2C device Failed!!\r\n"));
        goto I2COpenFalse;
	}/*else{
        RETAILMSG(1,(L"SUCCESS: BQ27510_init - Open I2C device OK!!\r\n"));
	}*/
    //OALLog(L"\r\n >>>Set hGPIO_I2C:%X \r\n",hGPIO_I2C);  
    
    GPIOClrBit(hGPIO_I2C,  184);  
    GPIOSetMode(hGPIO_I2C, 184, GPIO_DIR_OUTPUT);   //I2C3_CLK, Ray 131129 
    GPIOClrBit(hGPIO_I2C,  185);
    GPIOSetMode(hGPIO_I2C, 185, GPIO_DIR_OUTPUT);   //I2C3_SDA, Ray 131129 
    
    InitI2CWithBQ27510(hGPIO_I2C);                               
    GPIOClose(hGPIO_I2C);
I2COpenFalse: 
    return;    
}

//------------------------------------------------------------------------------
//   Barcode & Charge LED Indicator, Ray 131220 
//    
//
VOID LEDTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    DWORD stall_1mSec = 1000;
//    int i;
	HANDLE hGPIO;
	UNREFERENCED_PARAMETER(pMenu);
	
	OALBLMenuHeader(L"LED Indicator Test");
    hGPIO = GPIOOpen();
    
    //for(i=0; i<2; i++){
        GPIOClrBit(hGPIO, GREEN_LED_SET_GPIO);  
        LcdStall(stall_1mSec);
        GPIOSetBit(hGPIO, GREEN_LED_SET_GPIO);
        LcdStall(gd1Sec);
        GPIOClrBit(hGPIO, GREEN_LED_SET_GPIO);
        LcdStall(gd1Sec);
        
        GPIOClrBit(hGPIO, RED_LED_SET_GPIO);  
        LcdStall(stall_1mSec);
        GPIOSetBit(hGPIO, RED_LED_SET_GPIO);
        LcdStall(gd1Sec);
        GPIOClrBit(hGPIO, RED_LED_SET_GPIO);
        LcdStall(gd1Sec);

        GPIOClrBit(hGPIO, BARCODE_LED_SET_GPIO);
        LcdStall(stall_1mSec);
        GPIOSetBit(hGPIO, BARCODE_LED_SET_GPIO);
        LcdStall(gd1Sec);
        GPIOClrBit(hGPIO, BARCODE_LED_SET_GPIO);
        LcdStall(gd1Sec);
    //}
    DisplayShowBackground(0xFF);
    printStringMode(9, 9, RED_COLOR, GREEN_COLOR, gTestPass, 0);
    LcdStall(gd1Sec*2);
    DisplayShowBackground(0xFF);
    GPIOClose(hGPIO);
    
    OALLog(L"\rTested ok!! \r\n");    
}

//------------------------------------------------------------------------------
//  Barcode, Ray 131224
//
//
VOID BCRSetRTS(BOOL bSet)
{
    UINT bData = INREG8(&pUartRegs->MCR);

    if(bSet)
        bData |= UART_MCR_RTS;
    else
        bData &= ~UART_MCR_RTS;     //

    OUTREG8(&pUartRegs->MCR, bData);      
}
//
//
VOID BarcodeTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    HANDLE  hGPIO;
    UINT8   status, ch, running = 1;
    int     count = 30, inNum = 0;
//    int     i = 0;
    //WCHAR   key;
    //WCHAR   scan[50];
    char      scan[10][30] = {"Did not any scan !"};
    
	OALBLMenuHeader(L"Barcode Test");
	UNREFERENCED_PARAMETER(pMenu);	
	
    hGPIO = GPIOOpen();
    GPIOSetMode(hGPIO, BCR_ENG_PWEN, GPIO_DIR_OUTPUT);         
    GPIOSetMode(hGPIO, BCR_ENG_TRIG, GPIO_DIR_OUTPUT);
    
    GPIOSetBit(hGPIO, BCR_ENG_PWEN);                    //Power ON
    EnableDeviceClocks(OMAP_DEVICE_UART1, TRUE); 
    pUartRegs = OALPAtoUA(GetAddressByDevice(OMAP_DEVICE_UART1));
           
    // reset uart
	OUTREG8(&pUartRegs->SYSC, UART_SYSC_RST);
    while ((INREG8(&pUartRegs->SYSS) & UART_SYSS_RST_DONE) == 0)
        ;

    // Set baud rate
    OUTREG8(&pUartRegs->LCR, UART_LCR_DLAB);    //Line control register, DIV_EN
    OUTREG8(&pUartRegs->DLL, 0x38);             //9.6kbps
    OUTREG8(&pUartRegs->DLH, 0x01);
    OUTREG8(&pUartRegs->LCR, 0x00);

    // 8 bit, 1 stop bit, no parity
    OUTREG8(&pUartRegs->LCR, 0x03);     
    
    // Enable FIFO
    OUTREG8(&pUartRegs->FCR, UART_FCR_FIFO_EN);
    OUTREG8(&pUartRegs->FCR, UART_FCR_FIFO_EN|UART_FCR_RX_FIFO_CLEAR|UART_FCR_TX_FIFO_CLEAR);

    // Pool
    OUTREG8(&pUartRegs->IER, 0);

    // Set DTR/RTS signals
    OUTREG8(&pUartRegs->MCR, 0); //UART_MCR_DTR|UART_MCR_RTS);

    // Configuration complete so select UART 16x mode
	OUTREG8(&pUartRegs->MDR1, UART_MDR1_UART16);
    BCRSetRTS(TRUE);
    //OALLog(L"\r Scan Mode key '5', if Cancel '0'.\r\n");
    
    //OALLog(L"\r The Cancel key are '0'.\r\n");
    while(running)
    {
		//if(key == L'5')             // SCAN KEY
		//{
		    //start after barcode engine ON
		    LcdSleep(500);
			GPIOClrBit(hGPIO, BCR_ENG_TRIG);
			BCRSetRTS(FALSE);
			LcdSleep(500);
            
			while( count-- )
			{
				status = INREG8(&pUartRegs->LSR);
				if ((status & UART_LSR_RX_FIFO_E) != 0)
				{
					ch = INREG8(&pUartRegs->RHR);
					scan[1][inNum++] = ch;
					//OALLog(L" %c\r\n",ch);
				}
				LcdSleep(100);
			}
			//
			DisplayShowBackground(0xFF);
			GPIOSetBit(hGPIO, BCR_ENG_TRIG);
			if( inNum > 0 )
			{
				/*while(scan[i] != L'\0'){
				    OALLog(L"%c ",scan[i]);
				    i++;
				}
				OALLog(L"\r\n");*/
				scan[1][inNum] = '\0';
				//
                printStringMode(1, 12, BLUE_COLOR, TRANSPARENT_COLOR, scan, 1);
                LcdStall(gd1Sec*3);
                DisplayShowBackground(0xFF);
                //
                OALLog(L"->%s\r\n",scan[1]);
				LcdSleep(150);
				break;
			}else{
			    printStringMode(1, 6, BLUE_COLOR, TRANSPARENT_COLOR, scan, 0);
			    LcdStall(gd1Sec*3);
                DisplayShowBackground(0xFF);
                OALLog(L"\rDid not any scan !\r\n");
				break;
			}
			
			/*key = OALBLMenuReadKey(TRUE);
			if(key == L'0')             // ESC KEY
		    {
			    OALLog(L"Cancel \r\n");
			    break;
		    }*/
		//}
		//LcdSleep(150);
	}
	GPIOClrBit(hGPIO, BCR_ENG_PWEN);
	GPIOClose(hGPIO);
}

//------------------------------------------------------------------------------
//  Audio And MIC Testing, Ray 131227
//
/*void SetAudioI2SProfile(HANDLE handle)
{
    UINT8 regVal;
    
    if(handle)
    {
        TWLReadRegs(handle, TWL_CODEC_MODE, &regVal, sizeof(regVal));   //System mode control register
        regVal &= ~(CODEC_MODE_CODEC_OPT_MODE);                         //Audio and voice option selection
        TWLWriteRegs(handle, TWL_CODEC_MODE, &regVal, sizeof(regVal));  

        TWLReadRegs(handle, TWL_AUDIO_IF, &regVal, sizeof(regVal));     //Control mode for audio interface
        regVal &= ~( AUDIO_IF_AIF_FORMAT(3) | AUDIO_IF_DATA_WIDTH(0));  //TDM mode data format ,16-bit word width
        TWLWriteRegs(handle, TWL_AUDIO_IF, &regVal, sizeof(regVal));    

        // Side tone gain
        //
        regVal = 0x0;      // Mute
        TWLWriteRegs(handle, TWL_VSTPGA, &regVal, sizeof(regVal));      //Voice side tone gain control register
    }else{
        RETAILMSG(TRUE, (L"Handle is NULL!!!\r\n"));     
    }  
}*/


//
//
VOID AudioAndMIC_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"Audio And MIC");	
}

//------------------------------------------------------------------------------
//  Keypad functional Testing, Ray 131227
//
static int keyStatus[KEYPAD_SIZE] = {0};

VOID clear()
{
    int i;
    int keyClear[KEYPAD_SIZE] = {0};
    
    for(i=0; i<KEYPAD_SIZE; i++)
        keyStatus[i] = keyClear[i];
}
//
//
BOOL KeypadMatrixStatus(int row, int col)
{
    //UINT8   matrix[8];
    int  temp=0;
    BOOL event = FALSE;
    
    temp = gKeypadMatrix[row] & (1<<col);
 
    if(temp)
        return event = TRUE;
    else
        return event;
}
//
//
BOOL KeypadFuncMatrix()
{
    ULONG   ik, ix, row, column;
    USHORT  state=0;
    int     k, entry, keyAction = 0, rowPlace=2, colPlace=2; 
    BOOL    EXIT = FALSE;
    static int count = 26;
    char    presskey[][30] = { "[ENTR]","[SCAN]","[ENTL]",
                               "[ESC]","[BS]",
                               "[L]","[UP]","[DOWN]","[R]",
                               "[1]","[2]","[3]",
                               "[4]","[5]","[6]",
                               "[7]","[8]","[9]",
                               "[ . ]","[ 0 ]","[+-*/]",
                               "[F1]","[F2]","[F3]","[F4]",
                               "[TAB]","[FN]","[SP]"
                               };
    
    /*static int  ENTL=0, SCAN=0, ENTR=0, ESC=0, BS=0, L=0,
                U=0, D=0, R=0, ONE=0, TWO=0, THREE=0, 
                FOUR=0, FIVE=0, SIX=0, SEVEN=0, NINE=0, ZERO=0,
                DOT=0, PLUS=0, F1=0, F2=0, F4=0, SP=0, FN=0, TAB=0;*/
    
    //BOOL    ENTL=FALSE, SCAN=FALSE, ENTR=FALSE, ESC=FALSE;
                 
    //printing matrix array, Ray
    //for(;;){                   
        //for(ik=0; ik<3; ik++)
        //{
        TWLReadRegs(ghTwl, TWL_LOGADDR_FULL_CODE_7_0, gKeypadMatrix, sizeof(gKeypadMatrix));  //it is function read value, Ray 140103   
        //OALLog(L" %d\r\n",sizeof(gKeypadMatrix));         //8
        
        /*OALLog(L" HotKeyFunction: matrix  ");
            for(ix=0; ix<8; ix++)
                 OALLog(L"[%d]", matrix[ix]);
            OALLog(L"\r\n");      
        }*/
        for(row=0, ik=0; row<8; row++)
        {
            // Get matrix state 
            ix = row;
            state = gKeypadMatrix[ix] & 0xFF;     
            //OALLog(L"1%d\n",state);
            // If no-key is pressed continue with new rows
            if(state == 0)
            {
                ik += 8;
                continue;           //if state event desnot 
            }
            
            for(column=0; column<8; column++, ik++)
            {
                if((state & (1<<column)) !=0 ){
                    //RETAILMSG(TRUE, (L" HotKeyFunction: [%d,%d]\r\n",row ,column));
                    keyAction = 1;          //has values active
                    //OALLog(L"2%d\n",state);
                }
            }
        }
    //}   
     
    if(keyAction){
        //OALLog(L"keyAction:%d\n",keyAction);
        OALLog(L"\r");
        if( KeypadMatrixStatus(0, 5)){
            OALLog(L"[ENTL]         ");
            keyStatus[0]=1;
            printStringMode(rowPlace, colPlace, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 0);
            //count = count++;
            //ENTL = 1;
        }else if( KeypadMatrixStatus(0, 4)){    //trouble
            OALLog(L"[SCAN]         ");
            keyStatus[1]=1;
            printStringMode(rowPlace, colPlace+7, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 1);
            //count = count-1;
            //SCAN = 1;
        }else if( KeypadMatrixStatus(0, 2)){
            OALLog(L"[ENTR]         ");
            keyStatus[2]=1;
            printStringMode(rowPlace, colPlace+14, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 2);
            //count = count-1;
            //ENTR = 1;
        }else if( KeypadMatrixStatus(0, 1)){
            OALLog(L"[ESC]  ");
            keyStatus[3]=1;
            printStringMode(rowPlace+1, colPlace, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 3);
           //ESC = 1;
        }else if( KeypadMatrixStatus(0, 3)){
            OALLog(L"[BS]           ");
            keyStatus[4]=1;
            printStringMode(rowPlace+1, colPlace+16, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 4);
            //BS = 1;
        }else if( KeypadMatrixStatus(1, 5)){
            OALLog(L"[L]   ");
            keyStatus[5]=1;
            printStringMode(rowPlace+2, colPlace, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 5);
            //L = 1;
        }else if( KeypadMatrixStatus(1, 4)){
            OALLog(L"[UP]           ");
            keyStatus[6]=1;
            printStringMode(rowPlace+2, colPlace+5, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 6);
            //U = 1;
        }else if( KeypadMatrixStatus(1, 0)){
            OALLog(L"[DOWN]         ");
            keyStatus[7]=1;
            printStringMode(rowPlace+2, colPlace+10, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 7);
            //D = 1;
        }else if( KeypadMatrixStatus(1, 2)){
            OALLog(L"[R]          ");
            keyStatus[8]=1;
            printStringMode(rowPlace+2, colPlace+17, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 8);
            //R = 1;
        }else if( KeypadMatrixStatus(2, 5)){
            OALLog(L"[1]          ");
            keyStatus[9]=1;
            printStringMode(rowPlace+3, colPlace+2, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 9);
            //ONE = 1;
        }else if( KeypadMatrixStatus(1, 1)){
            OALLog(L"[2]          ");
            keyStatus[10]=1;
            printStringMode(rowPlace+3, colPlace+8, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 10);
            //TWO = 1;
        }else if( KeypadMatrixStatus(1, 3)){
            OALLog(L"[3]          ");
            keyStatus[11]=1;
            printStringMode(rowPlace+3, colPlace+15, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 11);
            //THREE = 1;
        }else if( KeypadMatrixStatus(2, 1)){
            OALLog(L"[4]          ");
            keyStatus[12]=1;
            printStringMode(rowPlace+4, colPlace+2, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 12);
            //FOUR = 1;
        }else if( KeypadMatrixStatus(2, 4)){
            OALLog(L"[5]          ");
            keyStatus[13]=1;
            printStringMode(rowPlace+4, colPlace+8, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 13);
            //FIVE = 1;
        }else if( KeypadMatrixStatus(2, 3)){
            OALLog(L"[6]          ");
            keyStatus[14]=1;
            printStringMode(rowPlace+4, colPlace+15, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 14);
            //SIX = 1;
        }else if( KeypadMatrixStatus(3, 5)){
            OALLog(L"[7]          ");
            keyStatus[15]=1;
            printStringMode(rowPlace+5, colPlace+2, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 15);
            //SEVEN = 1;
        }else if( KeypadMatrixStatus(2, 0)){
            OALLog(L"\r[8]          ");
            printStringMode(rowPlace+5, colPlace+8, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 16);
            //EIGHT = 1
        }else if( KeypadMatrixStatus(2, 2)){
            OALLog(L"[9]   ");
            keyStatus[16]=1;
            printStringMode(rowPlace+5, colPlace+15, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 17);
            //NINE = 1;
        }else if( KeypadMatrixStatus(3, 4)){
            OALLog(L"[0]          ");
            keyStatus[17]=1;
            printStringMode(rowPlace+6, colPlace+7, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 19);
            //ZERO = 1;
        }else if( KeypadMatrixStatus(3, 1)){
            OALLog(L"[.]          ");
            keyStatus[18]=1;
            printStringMode(rowPlace+6, colPlace, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 18);
            //DOT = 1;
        }else if( KeypadMatrixStatus(4, 1)){
            OALLog(L"[+-*/]");
            keyStatus[19]=1;
            printStringMode(rowPlace+6, colPlace+15, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 20);
            //PLUS = 1;
        }else if( KeypadMatrixStatus(3, 3)){
            OALLog(L"[F1]          ");
            keyStatus[20]=1;
            printStringMode(rowPlace+7, colPlace, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 21);
            //F1 = 1;
        }else if( KeypadMatrixStatus(4, 3)){
            OALLog(L"[F2]          ");
            keyStatus[21]=1;
            printStringMode(rowPlace+7, colPlace+5, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 22);
            //F2 = 1;
        }else if( KeypadMatrixStatus(3, 0)){
            OALLog(L"[F3]          ");
            printStringMode(rowPlace+7, colPlace+12, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 23);
            //F3 = 1;
        }else if( KeypadMatrixStatus(3, 2)){
            OALLog(L"[F4]          ");
            keyStatus[22]=1;
            printStringMode(rowPlace+7, colPlace+16, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 24);
            //F4 = 1;
        }else if( KeypadMatrixStatus(4, 2)){
            OALLog(L"[SP]          ");
            keyStatus[23]=1;
            printStringMode(rowPlace+8, colPlace+16, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 27);
            //SP = 1;
        }else if( KeypadMatrixStatus(4, 4)){
            OALLog(L"[FN]            ");
            keyStatus[24]=1;
            printStringMode(rowPlace+8, colPlace+9, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 26);
            //FN = 1;
        }else if( KeypadMatrixStatus(4, 5)){    //KeypadMatrixStatus(4, 5)
            OALLog(L"[TAB]          ");
            keyStatus[25]=1;
            //TAB = 1;
            printStringMode(rowPlace+8, colPlace+2, BLUE_COLOR, TRANSPARENT_COLOR, presskey, 25);
        }else{
            OALLog(L"Doesnt range. ");
        }
      
        for(k=0, entry=1; k<KEYPAD_SIZE; k++){
            entry &= keyStatus[k];         
        }

        if(entry){
            clear();
            LcdStall(gd1Sec);
            return EXIT = TRUE;
        }      
    }

    /*count--;
    if(count == 0)
        count = 26;*/

    /*OALLog(L"%d\n",keyStatus[0]);
    OALLog(L"%d\n",*(keyStatus+1));
    OALLog(L"%d\n",*(keyStatus+2));
    OALLog(L"%d\n",*(keyStatus+3));*/
      
    return EXIT;
}
//
//
VOID KeypadFunc_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    int     num;
    //BOOL    LOOP = TRUE;
    //WCHAR   key;
    BOOL    EXIT;
    char    message[10][30] =  {"You can press any key start,",
                                "[ESC]+[BS] are cancel>>",
                                ">>Test Break!!"
                               };
    
    UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"Keypad Functional Test");
	//OALLog(L"!!!!!!hTwl: %X....\r\n", ghTwl);
    
	//printing keypad 
	OALLog(L"\r [ENT] [SCAN] [ENT]\r\n");
	OALLog(L"\r [ESC]         [BS]\r\n");
	OALLog(L"\r [L] [UP][DOWN] [R]\r\n");
		
	for(num=1; num<10; num++){
        OALLog(L"   [%d]",num);
        if((num%3) == 0)
            OALLog(L"\r\n");
    }
	OALLog(L"\r [ . ] [ 0 ] [+-*/]\r\n");
	OALLog(L" "); 
	for(num=1; num<5; num++){
        OALLog(L"[F%d]",num);
        if((num%2) == 0)
            OALLog(L"  ");
    }
	OALLog(L"\n");   
    OALLog(L"\r [PWR][TAB][FN][SP] \r\n");
	OALLog(L"\r\n");
    OALLog(L"\r Press any key the keypad testing, [ESC]+[BS] are cancel>>>\r\n");
    DisplayShowBackground(0xFF);
    printStringMode(0, 1, RED_COLOR, TRANSPARENT_COLOR, message, 0);
    printStringMode(1, 1, RED_COLOR, TRANSPARENT_COLOR, message, 1);
    LcdStall(gd1Sec);
	//while(){
    do{
	    EXIT = KeypadFuncMatrix();
        LcdStall(gd1Sec/2); 
        if(EXIT){
            DisplayShowBackground(0xFF);
            printStringMode(9, 9, RED_COLOR, GREEN_COLOR, gTestPass, 0);
	        LcdStall(gd1Sec*2);
            OALLog(L"\r Tested ok!! \r\n");
            DisplayShowBackground(0xFF);
            return;
	    }	    
	    /*if( (key = OALBLMenuReadKey(TRUE)) == L'0')
            goto BREAK;*/
    }while((KeypadMatrixStatus(0, 1) & KeypadMatrixStatus(0, 3)) != TRUE);  //Breaking
    //}
//BREAK:
    printStringMode(17, 1, RED_COLOR, TRANSPARENT_COLOR, message, 2);
    LcdStall(gd1Sec*3);
    DisplayShowBackground(0xFF);
    OALLog(L"\r Test Break!! \r\n");
    return;
}

//------------------------------------------------------------------------------
//  Burn-In Testing, Ray 131227
//
VOID BurnIn_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    OAL_BLMENU_ITEM *ptr = NULL;
    
    UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"Burn-In Test");
	
	do{
	    //OALLog(L"\r\n >>>Display Testing...\r\n");
	    DisplayTest_Z2170P(ptr);                    //2
        LcdStall(gd1Sec*1); 

        //OALLog(L"\r\n >>>LCM Backlight Testing...\r\n");
	    BkTest_Z2170P(ptr);                         //3
        LcdStall(gd1Sec*1); 

        //OALLog(L"\r\n >>>DRAM Testing...\r\n");
        DRAMTest_Z2170P(ptr);                       //4
        LcdStall(gd1Sec*1); 

        //OALLog(L"\r\n >>>Keypad Backlight Testing...\r\n");
        KeypadBkTest_Z2170P(ptr);                   //5
        LcdStall(gd1Sec*1); 

        BatteryTest_Z2170P(ptr);                    //7 
        LcdStall(gd1Sec*1);
        
        LEDTest_Z2170P(ptr);                        //8
        LcdStall(gd1Sec*1);

        BarcodeTest_Z2170P(ptr);                    //9 
        LcdStall(gd1Sec*1);

        SecondaryBATFunc(ptr);                      //SP
        LcdStall(gd1Sec*1);
    
        RTCFunc(ptr);                               //SF
        LcdStall(gd1Sec*1);
	}while((KeypadMatrixStatus(0, 1) & KeypadMatrixStatus(0, 3)) != TRUE);
}

//------------------------------------------------------------------------------
//  Check DRAM all addressing ~ end addressing, Ray 140212
//  This a function vs DRAMTest_Z2170P between diference,that testing for detail. 
//  
//  CS0-SDRAM ranges at 0x8000_0000 ~ 0x9fff_ffff (512MByts), 
//  but in "oalAddressTable" memory allocation SIZE 256MB and
//  start addressing at 0x8000_0000, so... 
//
//     0x8FFF_FFFF - 0x8000_0000
//   = 0x0FFF_FFFF(28bit)
//   = 256MBtyes
//
VOID RAMAccessTest(OAL_BLMENU_ITEM *pMenu)
{
    ULONG  startAddress = IMAGE_SHARE_ARGS_CA;
    ULONG  endAddress = startAddress|(256 *1024 *1024-1); //endAddress = startAddress + size(256MB) 
    ULONG  i, checkAddress =  0xFFFFFFFF;
    BYTE   value, temp, pattern = 0xAA, pattern2 = 0x55;
    //BOOL   DOING = TRUE;
    ULONG  percent;
    int    row=0, col = 1;
    char   scan[10][30] = { "Test pattern:0xAA ok!!",
                            "Test pattern:0x55 ok!!" 
                            };
     
	UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"RAM Access Test(Completely)");

    OALLog(L"\r\n Start Address: 0X%08X", startAddress&checkAddress );
    OALLog(L"\r\n End   Address: 0X%08X", endAddress&checkAddress );
    OALLog(L"\r\n scan 100kB tip \".\""); 
    OALLog(L"\r\n------------------------------------------\r\n");
    
    //while(DOING){
    do{
        TWLReadRegs(ghTwl, TWL_LOGADDR_FULL_CODE_7_0, gKeypadMatrix, sizeof(gKeypadMatrix));    //cancel
        if((row%20)==0)                 //Ray   140305
        {
            DisplayShowBackground(0xFF);    
            row=0;
		}
		
        for(i=0, percent=1; i<(endAddress - startAddress); i++, percent++)
	    {
            temp = *(volatile BYTE *)(startAddress + i);        //read data     ;temp(0x8000_0000) to get a address 
            *(volatile BYTE *)(startAddress + i) = pattern;     //write pattern ;pattern write into address(if is AA) 
            value = *(volatile BYTE *)(startAddress + i);       //read pattern  ;value == AA, setup value
            *(volatile BYTE *)(startAddress + i) = temp;        //store data
        
            if(value != pattern)                                //check value(in address value) equal pattern
            {
                OALLog(L"\r\n Error Address: 0X%X", startAddress+i);
                break;
            }

            if( (percent%(1024*100)) == 0){   //scan 100kB show tip 
                //OALLog(L"\rScanned size: %dk\r\n", percent/1024); 
                OALLog(L"."); 
            }
        }
        OALLog(L"\r\nTest pattern:0xAA ok!! \r\n");
        printStringMode(row++, col, BLUE_COLOR, TRANSPARENT_COLOR, scan, 0);
        //OALLog(L"--------------------------------------------------\r\n");
        LcdStall(1);
        
        for(i=0, percent=1; i<(endAddress - startAddress); i++, percent++)
	    {
            temp = *(volatile BYTE *)(startAddress + i);        //read data     ;temp(0x8000_0000) to get a address 
            *(volatile BYTE *)(startAddress + i) = pattern2;     //write pattern ;pattern write into address(if is AA) 
            value = *(volatile BYTE *)(startAddress + i);       //read pattern  ;value == 1A, setup value
            *(volatile BYTE *)(startAddress + i) = temp;        //store data
        
            if(value != pattern2)                                //check value(in address value) equal pattern
            {
                OALLog(L"\r\n Error Address: 0X%X", startAddress+i);
                break;
            }

            if( (percent%(1024*100)) == 0){   //scan 100kB show tip 
                //OALLog(L"\rScanned size: %dk\r\n", percent/1024);
                OALLog(L"."); 
            }         
        }
        OALLog(L"\r\nTest pattern:0x55 ok!! \r\n");
        printStringMode(row++, col, BLUE_COLOR, TRANSPARENT_COLOR, scan, 1);
        //OALLog(L"--------------------------------------------------\r\n");
        LcdStall(1);
    }while((KeypadMatrixStatus(0, 1) & KeypadMatrixStatus(0, 3)) != TRUE);  
    DisplayShowBackground(0xFF);    
}

//------------------------------------------------------------------------------
//  This is function are barcode the auto scanning, Ray 140212
//
//
VOID AutoScanFunc(OAL_BLMENU_ITEM *pMenu)
{
	HANDLE  hGPIO;
    UINT8   status, ch;
    //UINT8   DOING = 1;
    int     count, inNum, value = 1,
            row = 0, col = 3;
    //int     i = 0;
    //WCHAR   key;
    //WCHAR   scan[50];
    char      scan[10][30] = {"Did not any scan !",
                              "\0" };
       	
	OALBLMenuHeader(L"Auto Scan");
	UNREFERENCED_PARAMETER(pMenu);	
	
    hGPIO = GPIOOpen();
    GPIOSetMode(hGPIO, BCR_ENG_PWEN, GPIO_DIR_OUTPUT);   
    GPIOSetMode(hGPIO, BCR_ENG_TRIG, GPIO_DIR_OUTPUT);
    
    GPIOSetBit(hGPIO, BCR_ENG_PWEN);                    //Power ON
    EnableDeviceClocks(OMAP_DEVICE_UART1, TRUE); 
    pUartRegs = OALPAtoUA(GetAddressByDevice(OMAP_DEVICE_UART1));
        
    // reset uart
	OUTREG8(&pUartRegs->SYSC, UART_SYSC_RST);
    while ((INREG8(&pUartRegs->SYSS) & UART_SYSS_RST_DONE) == 0)
        ;

    // Set baud rate
    OUTREG8(&pUartRegs->LCR, UART_LCR_DLAB);    //Line control register, DIV_EN
    OUTREG8(&pUartRegs->DLL, 0x38);             //9.6kbps
    OUTREG8(&pUartRegs->DLH, 0x01);
    OUTREG8(&pUartRegs->LCR, 0x00);

    // 8 bit, 1 stop bit, no parity
    OUTREG8(&pUartRegs->LCR, 0x03);             
    // Enable FIFO
    OUTREG8(&pUartRegs->FCR, UART_FCR_FIFO_EN);
    OUTREG8(&pUartRegs->FCR, UART_FCR_FIFO_EN|UART_FCR_RX_FIFO_CLEAR|UART_FCR_TX_FIFO_CLEAR);
    // Pool
    OUTREG8(&pUartRegs->IER, 0);
    // Set DTR/RTS signals
    OUTREG8(&pUartRegs->MCR, 0); //UART_MCR_DTR|UART_MCR_RTS);
    // Configuration complete so select UART 16x mode
	OUTREG8(&pUartRegs->MDR1, UART_MDR1_UART16);
    BCRSetRTS(TRUE);
    
    //while(DOING)
    do{
        TWLReadRegs(ghTwl, TWL_LOGADDR_FULL_CODE_7_0, gKeypadMatrix, sizeof(gKeypadMatrix));    //cancel
        inNum = 0;
        count = 30;
        LcdSleep(500);
		GPIOClrBit(hGPIO, BCR_ENG_TRIG);
		BCRSetRTS(FALSE);
		LcdSleep(500);
		
		//OALLog(L"count %d\r\n",count);	
		while( count-- )
		{
		    status = INREG8(&pUartRegs->LSR);
		    if ((status & UART_LSR_RX_FIFO_E) != 0)
			{
			    ch = INREG8(&pUartRegs->RHR);
			    scan[1][inNum++] = ch;
				//OALLog(L" %c\r\n",ch);
			}
			LcdSleep(100);
			//OALLog(L"count %d\r\n",count);	
	    }
        //
        if((row%20)==0)                 //Ray   140305
        {
            DisplayShowBackground(0xFF);    
            row=0;
		}   
		GPIOSetBit(hGPIO, BCR_ENG_TRIG);
		
		if( inNum > 0 )
		{
		    scan[1][inNum] = '\0';
		    printInt(row, 1, RED_COLOR, TRANSPARENT_COLOR, value); 
		    printStringMode(row++, col, BLUE_COLOR, TRANSPARENT_COLOR, scan, 1);
			LcdStall(250000);
			OALLog(L"\rScan %d time: %s\r\n", value, scan[1]);
			//break;
		}else if(inNum == 0){
		    printInt(row, 1, RED_COLOR, TRANSPARENT_COLOR, value);
		    printStringMode(row++, col, BLUE_COLOR, TRANSPARENT_COLOR, scan, 0);
			LcdStall(250000);
            OALLog(L"\rDid not any scan !\r\n");
		}
	    value+=1;
	    if(value == 10) col=4;
	        
		LcdSleep(300);
	}while((KeypadMatrixStatus(0, 1) & KeypadMatrixStatus(0, 3)) != TRUE);
	GPIOClrBit(hGPIO, BCR_ENG_PWEN);
	DisplayShowBackground(0xFF);
	GPIOClose(hGPIO);
}

//------------------------------------------------------------------------------
//  This is function are measure secondary battery, Ray 140212
//
//
VOID SecondaryBATFunc(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);
    ReadBBATVoltage();                //Backup battery

    //Check value 
}

//------------------------------------------------------------------------------
//  This is function are apply on RTC, Ray 140212
//
//
VOID RTCFunc(OAL_BLMENU_ITEM *pMenu)
{
    
    OALLog(L"\r\n Running...\r\n"); 
    UNREFERENCED_PARAMETER(pMenu);  

    DisplayShowBackground(0xFF); 
    ReadRTC();
    LcdStall(gd1Sec*3);
    DisplayShowBackground(0x00);
     
}

//==============================================================================
//  Running Z2000...
//
VOID AllFunctionTest_Z2000(OAL_BLMENU_ITEM *pMenu)
{
    //OAL_BLMENU_ITEM *ptr = NULL;
	//ALL Test
	UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"All Function Test");
    
    OALLog(L"\r\n >>>All function testing...\r\n");    

	/*DisplayTest_Z2170P(ptr);
	LcdStall(gd1Sec*3);                         //2
  
	BkTest_Z2170P(ptr);
	LcdStall(gd1Sec*3);                         //3
         
	DRAMTest_Z2170P(ptr);                                  
	LcdStall(gd1Sec*3);                         //4 
	   
	KeypadBkTest_Z2170P(ptr);                              
	LcdStall(gd1Sec*3);                         //5 */
	OALLog(L"\r\n >>>All function tested...\r\n");
}

//------------------------------------------------------------------------------
//  Red-Green-Blue-Black-White, Ray 131115
//
VOID DisplayTest_Z2000(OAL_BLMENU_ITEM *pMenu)
{
    BLShowLogo();
    OALBLMenuHeader(L"Display Test");
	UNREFERENCED_PARAMETER(pMenu);
    OALLog(L"\r\n Running...\r\n");
	//DisplayShow(BSP_Z2000);
	DisplayShow();
}
