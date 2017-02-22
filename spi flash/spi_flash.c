#include "string.h"
#include "spi_flash.h"
#include "spi.h"
#include "s_uart.h"
#include <string.h>

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

XStatus FlashWrite(XSpi *SpiPtr, u32 Address, u8 *data, u32 size)
{
    XStatus iStatus;
    u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
    memset(WriteBuffer,0x00, MAX_DATA+DATA_OFFSET);
   
    if(size > MAX_DATA + DATA_OFFSET)           //This maximum data size 32 is arbitrary
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

    return iStatus;
}

XStatus FlashRead(XSpi *SpiPtr, u32 Address, u8 *data, u32 size)
{
    XStatus iStatus;
    u32 transfer_size = MAX_DATA + DATA_OFFSET;
    memset(ReadBuffer, 0x00, MAX_DATA + DATA_OFFSET);
    memset(WriteBuffer, 0x00, MAX_DATA + DATA_OFFSET);
    WriteBuffer[COMMAND_OFFSET]   = READ_CMD;
	WriteBuffer[ADDRESS_1_OFFSET] = (u8)((Address & 0xFF000000) >> 24);
	WriteBuffer[ADDRESS_2_OFFSET] = (u8)((Address & 0xFF0000) >> 16);
	WriteBuffer[ADDRESS_3_OFFSET] = (u8)((Address & 0xFF00) >> 8);
    WriteBuffer[ADDRESS_4_OFFSET] = (u8)(Address & 0xFF);
    //Send over SPI
	TransferInProgress = TRUE;
    iStatus = XSpi_Transfer(SpiPtr, WriteBuffer, ReadBuffer, transfer_size);
	//while (TransferInProgress);
	memcpy(data, &ReadBuffer[DATA_OFFSET], size);
    return iStatus;
}

u16 spiFlashGetIDRegister(XSpi *FlashSpiPtr, u32 *regPtr){
  u8 WriteDisableCmd = { WRITE_DISABLE_CMD };

  int16_t status = XST_SUCCESS;
  uint32_t transferBytes = S25FL_READ_ID_BYTES;
  u8 intReadBuffer[MAX_DATA + DATA_OFFSET];
  if(regPtr == NULL) {
    return XST_FAILURE;
  }


  /** Clear the writeBuffer*/
  memset(WriteBuffer, 0x0, MAX_DATA + DATA_OFFSET);
  memset(ReadBuffer, 0x0, MAX_DATA + DATA_OFFSET);
  memset(intReadBuffer, 0x0, MAX_DATA + DATA_OFFSET);
  //Write enable
  TransferInProgress = TRUE;
  status = XSpi_Transfer(FlashSpiPtr, &WriteDisableCmd, NULL, sizeof(WriteDisableCmd));
  while (TransferInProgress);
  if(status != XST_SUCCESS){
    return XST_FAILURE;
  }
  /**
   * Prepare the Write Buffer.
   */
  WriteBuffer[0] = S25FL_COMMAND_READ_ID;
  TransferInProgress = TRUE;
  status = XSpi_Transfer(FlashSpiPtr, WriteBuffer, intReadBuffer, transferBytes);
  while (TransferInProgress);
  if(status != XST_SUCCESS){
    return XST_FAILURE;
  }
  // Copy status register
  *regPtr = ((uint32_t)ReadBuffer[1] << 16) | ((uint32_t)ReadBuffer[2] << 8) | ReadBuffer[3];

  return XST_SUCCESS;
}

//Sample 0x00 instruction for test purposes
XStatus TestInstruction(XSpi *SpiPtr)
{

    XStatus iStatus;
    memset(WriteBuffer,0x00, MAX_DATA+DATA_OFFSET);

	//Write setup and send
    WriteBuffer[COMMAND_OFFSET] = 0x00;

    //Send over SPI
	TransferInProgress = TRUE;
    iStatus = XSpi_Transfer(SpiPtr, WriteBuffer, NULL, 1);
	while (TransferInProgress);

    return iStatus;
}

