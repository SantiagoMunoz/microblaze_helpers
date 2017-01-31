#include "spi.h"
#include "intc.h"
#include "xparameters.h"

XSpi_Config stSpi_config;
XSpi stSpi;

int SPI_Error;
int TransferInProgress;

XStatus spi_initialize(XSpi *pstSpi)
{
    XStatus iStatus;

    &stSpi_config = XSpi_LookupConfig(XPAR_AXI_QUAD_SPI_0_DEVICE_ID);
	if (stSpi_config == NULL) 
		return XST_FAILURE;
    
    iStatus = XSpi_CfgInitialize(pstSpi, &stSpi_config,
					&stSpi_config>BaseAddress);
	if (iStatus != XST_SUCCESS) {
		return XST_FAILURE;
	}

    return XST_SUCCESS;
}

XStatus spi_config(XSpi *pstSpi)
{
    XStatus iStatus;

    XSpi_SetStatusHandler(pstSpi, pstSpi,
				 (XSpi_StatusHandler) SpiHandler);

    iStatus = XSpi_SetOptions(pstSpi, XSP_MANUAL_SSELECT_OPTION | XSP_MASTER_OPTION | XSP_CLK_PHASE_1_OPTION | XSP_CLK_ACTIVE_LOW_OPTION);

    if(iStatus != XST_SUCCESS)
        return XST_FAILURE;

    return XST_SUCCESS;
}

void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount)
{
	/*
	 * Indicate the transfer on the SPI bus is no longer in progress
	 * regardless of the status event
	 */

	TransferInProgress = FALSE;

	/*
	 * If the event was not transfer done, then track it as an error
	 */
	if (StatusEvent != XST_SPI_TRANSFER_DONE) {
		SPI_Error++;
	}
}

