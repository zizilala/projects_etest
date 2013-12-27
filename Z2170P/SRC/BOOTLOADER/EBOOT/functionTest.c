//------------------------------------------------------------------------------
//  File:  functionTest.c
//
#include <eboot.h>
#include "sdk_i2c.h"
#include "bsp.h"
#include "bq27xxx_new.h"
#include "omap.h"
#include "oal_clock.h"      //




//------------------------------------------------------------------------------
//#define BSP_Z2170P      2170 
//#define BSP_Z2000       2000 
#define BK_SET_GPIO             61
#define KP_LED_SET_GPIO         155
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

//------------------------------------------------------------------------------
//  Prototype
//
static VOID AllFunctionTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID DisplayTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID BkTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID DRAMTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID KeypadBkTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID AllFunctionTest_Z2000(OAL_BLMENU_ITEM *pMenu);
static VOID DisplayTest_Z2000(OAL_BLMENU_ITEM *pMenu);
static VOID TouchPanelTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID BatteryTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID LEDTest_Z2170P(OAL_BLMENU_ITEM *pMenu);
static VOID BarcodeTest_Z2170P(OAL_BLMENU_ITEM *pMenu);   
static VOID AudioAndMIC_Z2170P(OAL_BLMENU_ITEM *pMenu); 
static VOID KeypadFunc_Z2170P(OAL_BLMENU_ITEM *pMenu); 
static VOID BurnIn_Z2170P(OAL_BLMENU_ITEM *pMenu); 

BOOL DisplayShow(void);
VOID SetBacklight(void);
VOID tsc2046Test(void);

//VOID SetKeypadBacklight(void);
VOID LcdStall(DWORD);
VOID LcdSleep(DWORD);

//------------------------------------------------------------------------------
//  Global variable
//
DWORD stall_1Sec = 1000000;      //Lcd Stall
DWORD sleep1Sec = 1000;         //Lcd Sleep 

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
    }, {
        L'A', L"Audio and MIC", AudioAndMIC_Z2170P,
        NULL, NULL, NULL
    },{
        L'B', L"Keypad functional", KeypadFunc_Z2170P,
        NULL, NULL, NULL
    },{
        L'C', L"Burn-In", BurnIn_Z2170P,
        NULL, NULL, NULL
    },{
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

//------------------------------------------------------------------------------
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
	LcdStall(stall_1Sec*2);                         //2
  
	BkTest_Z2170P(ptr);
	LcdStall(stall_1Sec*2);                         //3
         
	DRAMTest_Z2170P(ptr);                                  
	LcdStall(stall_1Sec*2);                         //4 
	   
	KeypadBkTest_Z2170P(ptr);                              
	LcdStall(stall_1Sec*2);                         //5 

	
	OALLog(L"\r\n >>>All function tested...\r\n");
}

//------------------------------------------------------------------------------
//  Red-Green-Blue-Black-White, Ray 131107
//
VOID DisplayTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    BLShowLogo();
    OALBLMenuHeader(L"Display Test");
	UNREFERENCED_PARAMETER(pMenu);
    OALLog(L"\r\n Running...\r\n");
	//DisplayShow(BSP_Z2170P);
	DisplayShow();
}

//------------------------------------------------------------------------------
//  Show brightness 0% ~ 100%, ON and OFF,  Ray 131111
//
VOID BkTest_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
	OALBLMenuHeader(L"LCM Backlight Test");
	UNREFERENCED_PARAMETER(pMenu);
    SetBacklight();
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
    ULONG  endAddress = startAddress|(256 *1024 *1024 -1); //endAddress = startAddress + size(256MB) 
    ULONG   test1_Address = (startAddress|0x0000FFFF);      //
    //ULONG   test2_Address = (startAddress|0x0FFFFF00);
    //ULONG   ebootAddress = IMAGE_EBOOT_CODE_CA ;
    ULONG   i, testAddress =  0xFFFFFFFF;
    BYTE    value, temp, pattern = 0x1A; 
    //ULONG   percentRange;
    ULONG   percent;        
     
	UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"DRAM Test");

	//percentRange = (endAddress - startAddress)/100;
    OALLog(L"\r\n !Start Address: 0X%08X", (startAddress&testAddress));
    OALLog(L"\r\n !Test1 Address: 0X%08X", (test1_Address&testAddress));
    //OALLog(L"\r\n !Test2 Address: 0X%08X", (test2_Address&test));
    OALLog(L"\r\n !End   Address: 0X%08X", (endAddress&testAddress));
    OALLog(L"\r\n------------------------------------------\n");
  
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
            LcdStall(stall_1Sec); 
        }
	}
    OALLog(L"\r\n");
    LcdStall(stall_1Sec*3);*/

    OALLog(L"\r\n");
    for(i=0, percent=1; i<(test1_Address - startAddress); i++, percent++)
	{
        temp = *(volatile BYTE *)(startAddress + i);        //read data     ;temp(0x8000_0000) to get a address 
        *(volatile BYTE *)(startAddress + i) = pattern;     //write pattern ;pattern write into address(1A) 
        value = *(volatile BYTE *)(startAddress + i);       //read pattern  ;value == 1A, setup value
        *(volatile BYTE *)(startAddress + i) = temp;        //store data
        
        if(value != pattern)                                //check value(in address value) equal pattern
        {
            OALLog(L"\r\n Error Address: 0X%X", startAddress+i);
        }else{
            OALLog(L"*");
            //percent++;
            //OALLog(L"\r\n Decoding Memoey percent: %d >>>  Address: 0X%X",percent, test2_Address+i);   
            //OALLog(L"\r\n Address: 0X%X", startAddress+i);
        }

        if( (percent%1024) == 0){   //scan 1k show tip 
            OALLog(L"\r\nScan size: %dk\r\n",percent/1024);
            LcdStall(stall_1Sec/1000); 
        }
	}
    OALLog(L"\r\n");

    //LcdStall(stall_1Sec*3); 
    
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

    GPIOClrBit(hGPIO, KP_LED_SET_GPIO);
    LcdStall(1);
    GPIOSetBit(hGPIO, KP_LED_SET_GPIO);
    OALLog(L"\r !Now Keypad LED first flash \r\n");
    LcdSleep(delay*2);
    
    
    GPIOClrBit(hGPIO, KP_LED_SET_GPIO);
    LcdStall(delay);
    GPIOSetBit(hGPIO, KP_LED_SET_GPIO);
    OALLog(L"\r !Now Keypad LED twice flash \r\n");
    LcdSleep(delay*2);

    GPIOClrBit(hGPIO, KP_LED_SET_GPIO);

    GPIOClose(hGPIO);

}

//------------------------------------------------------------------------------
VOID SetBacklight()
{
    HANDLE hGPIO;
    //WCHAR key;
    int i, value, k, scale = 0;    
    int AAT3123Code[11] = { 1, 4 , 7, 10, 13, 16, 19, 22, 25, 28, 32};
                          //0  10  20  30 40  50  60  70  80  90  100
   
    hGPIO = GPIOOpen();

    GPIOClrBit(hGPIO, BK_SET_GPIO);
    LcdStall(stall_1Sec); 
            
    for(i=1, k=0; i<=32; i++)
    {
        if(i == (value = AAT3123Code[k]))
        {
            GPIOClrBit(hGPIO, BK_SET_GPIO);
            LcdStall(1);
            GPIOSetBit(hGPIO, BK_SET_GPIO);
            LcdStall(stall_1Sec);
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
    /*for (i=0; i<BK_EXIT; i++)
    {
        OALLog(L" [%d] %s\r\n", i+1, lcmBacklight[i].bkName);
	}
    OALLog(L" [0] Exit and Continue\r\n");*/

    //OALLog(L"\r\n Selection : ");

    /*do{ 
        key = OALBLMenuReadKey(TRUE);
    }while(key < L'0' || key > L'0' + i);
    // Show selection
	OALLog(L"%c\r\n", key);*/
	
    /*switch(key)
    {
         case L'1':
             GPIOClrBit(hGPIO, BK_SET_GPIO);
             LcdStall(stall_1Sec); 
            
             for(i=1, k=0; i<=32; i++)
             {
                if(i == (value = AAT3123Code[k]))
                {
                    GPIOClrBit(hGPIO, BK_SET_GPIO);
                    LcdStall(1);
                    GPIOSetBit(hGPIO, BK_SET_GPIO);
                    LcdStall(stall_1Sec);
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
            break;
         case L'2':
             for(i=1; i<=22; i++)
             {
                 GPIOClrBit(hGPIO, BK_SET_GPIO);
                 LcdStall(1);
                 GPIOSetBit(hGPIO, BK_SET_GPIO);
                 LcdStall(1);
             }
             OALLog(L"\r !Now Backlight ON,default scale at 70%%\r\n");
             break;
         case L'3':
             GPIOClrBit(hGPIO, BK_SET_GPIO);
             LcdStall(1);
             OALLog(L"\r !Now Backlight OFF\r\n");
             break;
    }*/
    GPIOClose(hGPIO);
    
    //if (key == L'0') return;    
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
    OALLog(L"\r Touch Panel End...\r\n");  
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
    int LSB = 0, MSB = 0; 
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
    gaugeInformation(hGPIO_I2C,  0x00);         //This opreating means clean register
    data.Temp =(short) gaugeInformation(hGPIO_I2C,  bq27500CMD_TEMP_LSB);
    gaugeInformation(hGPIO_I2C,  0x00);
    data.Voltage =(short) gaugeInformation(hGPIO_I2C,  bq27500CMD_VOLT_LSB);
    gaugeInformation(hGPIO_I2C,  0x00);
    data.NomAvailCap =(short) gaugeInformation(hGPIO_I2C,  bq27500CMD_NAC_LSB);
    gaugeInformation(hGPIO_I2C,  0x00);
    
    OALLog(L" ~Reports the device type:0x%02X%02X \r\n",MSB ,LSB);
    OALLog(L" ~Battery Temperature = %d C\r\n",(data.Temp/10)-273);
    OALLog(L" ~Battery Voltages = %d mV\r\n",data.Voltage);
    OALLog(L" ~Battery Nominal Available Capacity = %d mAh\r\n",data.NomAvailCap);
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
	}else{
        RETAILMSG(1,(L"SUCCESS: BQ27510_init - Open I2C device OK!!\r\n"));
	}
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
        GPIOClrBit(hGPIO, BARCODE_LED_SET_GPIO);
        LcdStall(stall_1mSec);
        GPIOSetBit(hGPIO, BARCODE_LED_SET_GPIO);
        LcdStall(stall_1Sec);
        GPIOClrBit(hGPIO, BARCODE_LED_SET_GPIO);
        LcdStall(stall_1Sec);
        
        GPIOClrBit(hGPIO, GREEN_LED_SET_GPIO); //??
        LcdStall(stall_1mSec);
        GPIOSetBit(hGPIO, GREEN_LED_SET_GPIO);
        LcdStall(stall_1Sec);
        GPIOClrBit(hGPIO, GREEN_LED_SET_GPIO);
        LcdStall(stall_1Sec);
        
        GPIOClrBit(hGPIO, RED_LED_SET_GPIO);    //?
        LcdStall(stall_1mSec);
        GPIOSetBit(hGPIO, RED_LED_SET_GPIO);
        LcdStall(stall_1Sec);
        GPIOClrBit(hGPIO, RED_LED_SET_GPIO);
        LcdStall(stall_1Sec);
    //}
    GPIOClose(hGPIO);
}

//------------------------------------------------------------------------------
//  Barcode, Ray 131224
//
OMAP_UART_REGS *pUartRegs = NULL;
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
    //int     i = 0;
    WCHAR   key;
    WCHAR   scan[50];
       
	UNREFERENCED_PARAMETER(pMenu);	
	OALBLMenuHeader(L"LED Indicator Test");
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
    
    OALLog(L"\r The Cancel key '0'.\r\n");
    while(running)
    {
		//if(key == L'5')             // SCAN KEY
		//{
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
					scan[inNum++] = ch;
					//OALLog(L" %c\r\n",ch);
				}
				LcdSleep(100);
			}
			
			GPIOSetBit(hGPIO, BCR_ENG_TRIG);
			if( inNum > 0 )
			{
				/*while(scan[i] != L'\0'){
				    OALLog(L"%c ",scan[i]);
				    i++;
				}
				OALLog(L"\r\n");*/
				scan[inNum] = '\0';
				OALLog(L"->%s\r\n",scan);
				break;
			}
			
			key = OALBLMenuReadKey(TRUE);
			if(key == L'0')             // ESC KEY
		    {
			    OALLog(L"Cancel \r\n");
			    break;
		    }
		//}
		LcdSleep(150);
	}
	GPIOClrBit(hGPIO, BCR_ENG_PWEN);
	GPIOClose(hGPIO);
}

//------------------------------------------------------------------------------
//  Audio And MIC Testing, Ray 131227
//
VOID AudioAndMIC_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"Audio And MIC");
}

//------------------------------------------------------------------------------
//  Keypad functional Testing, Ray 131227
//
VOID KeypadFunc_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"Keypad Functional Test");
}

//------------------------------------------------------------------------------
//  Burn-In Testing, Ray 131227
//
VOID BurnIn_Z2170P(OAL_BLMENU_ITEM *pMenu)
{
    UNREFERENCED_PARAMETER(pMenu);
	OALBLMenuHeader(L"Burn-In Test");
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
	LcdStall(stall_1Sec*3);                         //2
  
	BkTest_Z2170P(ptr);
	LcdStall(stall_1Sec*3);                         //3
         
	DRAMTest_Z2170P(ptr);                                  
	LcdStall(stall_1Sec*3);                         //4 
	   
	KeypadBkTest_Z2170P(ptr);                              
	LcdStall(stall_1Sec*3);                         //5 */
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



