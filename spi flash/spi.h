#ifndef SPI_H_
#define SPI_H_

#include "xparameters.h"
#include "xspi.h"


extern XSpi_Config stSpi_config;
extern XSpi stSpi;
extern int SPI_Error;
extern int TransferInProgress;

XStatus spi_initialize(XSpi *pstSpi);

XStatus spi_config(XSpi *pstSpi);

void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount);


#endif

