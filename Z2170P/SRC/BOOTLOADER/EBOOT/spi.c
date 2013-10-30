#include "bsp.h"
#include "omap_mcspi_regs.h"

OMAP_MCSPI_REGS *pSPIRegs;

BOOL omap_mcspi_init()
{
	BOOL rc = FALSE;
	DWORD dwCount = 0;
	
	// Map SPI controller
	pSPIRegs = OALPAtoUA(OMAP_MCSPI1_REGS_PA);
	
	// Reset the SPI controller
    SETREG32(&pSPIRegs->MCSPI_SYSCONFIG, MCSPI_SYSCONFIG_SOFTRESET);

    // Wait until resetting is done
    while ( !(INREG32(&pSPIRegs->MCSPI_SYSSTATUS) & MCSPI_SYSSTATUS_RESETDONE)) 
    {
        OALStall(100);
        if (dwCount++>0x100)
        {
            // Break out dead lock, something is wrong.
            OALLog(L"omap_mcspi_init: ERROR holding in reset.\r\n");
            goto cleanUp;
        }
    }
    // Setup Module Control as master
    OUTREG32(&pSPIRegs->MCSPI_MODULCTRL, 0);
    
    // Set configuration
    OUTREG32(&pSPIRegs->MCSPI_SYSCONFIG, MCSPI_SYSCONFIG_DISABLEIDLE); 
    
    OUTREG32(&pSPIRegs->MCSPI_CHCONF0,	MCSPI_PHA_EVEN_EDGES | 
    									MCSPI_POL_ACTIVELOW |
										MCSPI_CHCONF_CLKD(3) | 
										MCSPI_CHCONF_WL(9) |
										MCSPI_CHCONF_TRM_TXRX |
										MCSPI_CSPOLARITY_ACTIVELOW |
										MCSPI_CHCONF_DPE0);
    
	rc = TRUE;
	
cleanUp:

	return rc;
}
