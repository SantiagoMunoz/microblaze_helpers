#include "spi_flash.h"
#include "spi.h"
#include "uart.h"

//Locally used defines

u8 WriteBuffer[MAX_DATA + DATA_OFFSET];
u8 ReadBuffer[MAX_DATA + DATA_OFFSET];

XStatus spiSetupFlash(XSpi *SpiPtr)
{

    memset(WriteBuffer, 0x00, sizeof(WriteBuffer));
    memset(ReadBuffer, 0x00, sizeof(ReadBuffer));

    XSpi_SetSlaveSelect(SpiPtr, FLASH_SPI_SELECT);

    return XST_SUCCESS;
}

XStatus FlashWrite(XSpi *SpiPtr, u32 Address, uint8_t *data, u32 size)
{
    XStatus iStatus;
    u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
	u8 WriteDisableCmd = { WRITE_DISABLE_CMD };
    memset(WriteBuffer,0x00, MAX_DATA+DATA_OFFSET);
   
    if(size > 32)           //This maximum data size 32 is arbitrary
       return XST_FAILURE; 

    //Write enable
	TransferInProgress = TRUE;
	XSpi_Transfer(SpiPtr, &WriteEnableCmd, NULL,
			 sizeof(WriteEnableCmd));
	while (TransferInProgress);

	//Write setup and send
    WriteBuffer[COMMAND_OFFSET] = WRITE_CMD;
	WriteBuffer[ADDRESS_1_OFFSET] = (u8)((Address & 0xFF000000) >> 24);
	WriteBuffer[ADDRESS_2_OFFSET] =  (u8)((Address & 0xFF0000) >> 16);
	WriteBuffer[ADDRESS_3_OFFSET] =  (u8)((Address & 0xFF00) >> 8);
    WriteBuffer[ADDRESS_4_OFFSET] =  (u8)(Address & 0xFF);
    //Dump data at correct offset
    memcpy(&(WriteBuffer[DATA_OFFSET]), data, size);
    //Send over SPI
	TransferInProgress = TRUE;
    iStatus = XSpi_Transfer(SpiPtr, WriteBuffer, NULL, 5+size);
	while (TransferInProgress);

    //Write disable
	TransferInProgress = TRUE;
	XSpi_Transfer(SpiPtr, &WriteDisableCmd, NULL,
			 sizeof(WriteDisableCmd));
	while (TransferInProgress);

    return iStatus;
}

XStatus FlashRead(XSpi *SpiPtr, u32 Address, u8 *data, u8 size)
{
    XStatus iStatus;
    memset(ReadBuffer, 0x00, MAX_DATA + DATA_OFFSET);
    memset(WriteBuffer, 0x00, MAX_DATA + DATA_OFFSET);
    WriteBuffer[COMMAND_OFFSET]   = READ_CMD;
	WriteBuffer[ADDRESS_1_OFFSET] = (u8)((Address & 0xFF000000) >> 24);
	WriteBuffer[ADDRESS_2_OFFSET] = (u8)((Address & 0xFF0000) >> 16);
	WriteBuffer[ADDRESS_3_OFFSET] = (u8)((Address & 0xFF00) >> 8);
    WriteBuffer[ADDRESS_4_OFFSET] = (u8)(Address & 0xFF);

	TransferInProgress = TRUE;
    iStatus = XSpi_Transfer(SpiPtr, WriteBuffer, ReadBuffer, 5);
    memcpy(data, &ReadBuffer[DATA_OFFSET], size);
	while (TransferInProgress);
    return iStatus;
}
