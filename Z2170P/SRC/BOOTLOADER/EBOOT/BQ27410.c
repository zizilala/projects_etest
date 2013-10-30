#include "bsp.h"
#include "sdk_i2c.h"

#define BQ27410_SLAVE_ADDRESS	0x55
#define bq27410CMD_CNTL_LSB		0x00
#define bq27410CMD_VOLT_LSB		0x04

HANDLE hI2C;
//-----------------------------------------------------------------------------
BOOL BQ27410_WriteReg(UINT8 slaveaddress,UINT16 value)
{
    BOOL rc = FALSE;

    if (hI2C)
    {
        DWORD len = I2CWrite(hI2C, slaveaddress, &value, sizeof(UINT16));
        if ( len != sizeof(UINT16))
        	RETAILMSG(1,(L"ERROR: BQ27410_WriteReg Failed!!\r\n"));
		else
			rc = TRUE;
	}
	
    return rc;
}
//-----------------------------------------------------------------------------
BOOL BQ27410_ReadReg(UINT8 slaveaddress,UINT16* data)
{
    BOOL rc = FALSE;
    
    if (hI2C)
    {
        DWORD len = I2CRead(hI2C, slaveaddress, data, 2);
        if ( len != sizeof(UINT16))
        	RETAILMSG(1,(L"ERROR: BQ27410_ReadReg Failed!!\r\n"));
		else
			rc = TRUE;
	}
	
    return rc;
}
//-----------------------------------------------------------------------------
BOOL bq27410_init()
{
	BOOL rc = FALSE;
	USHORT volt = 0;
	
	if((hI2C = I2COpen(OMAP_DEVICE_I2C3)) == NULL) 
	{
        RETAILMSG(1,(L"ERROR: bq27410_init Failed open I2C device driver\r\n"));
        goto cleanUp;
	}
	
	if( I2CSetSlaveAddress(hI2C,  BQ27410_SLAVE_ADDRESS) == FALSE )
		RETAILMSG(1,(L"ERROR: bq27410_init - I2CSetSlaveAddress Failed\r\n"));
	
	I2CSetSubAddressMode(hI2C, I2C_SUBADDRESS_MODE_8);
	I2CSetBaudIndex(hI2C, FULLSPEED_MODE);
	
	if( BQ27410_WriteReg(bq27410CMD_CNTL_LSB,  0x0C) == FALSE ) // send battery insert
		RETAILMSG(1,(L"ERROR: BatteryPDDInitialize: Battery insert Failed\r\n"));
	
	if( BQ27410_ReadReg(bq27410CMD_VOLT_LSB, &volt) )
		RETAILMSG(1,(L"bq27410_init: volt = %d\r\n", volt));
	
	I2CClose(hI2C);
	rc = TRUE;
cleanUp:

	return rc;
}