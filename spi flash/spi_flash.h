#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_

#include "spi.h"

#define S25FL_COMMAND_READ_ID           0x9F /* Read ID command */
#define S25FL_COMMAND_WRITE_ENABLE      0x06 /* Write Enable command */
#define S25FL_COMMAND_WRITE_DISABLE     0x04 /* Write Disable command */
#define S25FL_COMMAND_WRITE             0x12 /* Page Program */
#define S25FL_COMMAND_READ              0x13 /* Output Read */

//-------------
#define WRITE_STATUS_CMD	0x01
#define WRITE_CMD		    S25FL_COMMAND_WRITE
#define READ_CMD		    S25FL_COMMAND_READ
#define WRITE_DISABLE_CMD	S25FL_COMMAND_WRITE_DISABLE
#define READ_STATUS_CMD		0x05
#define WRITE_ENABLE_CMD	S25FL_COMMAND_WRITE_ENABLE
#define READ_ID			    S25FL_COMMAND_READ_ID

#define COMMAND_OFFSET          0
#define ADDRESS_1_OFFSET        1
#define ADDRESS_2_OFFSET        2
#define ADDRESS_3_OFFSET        3
#define ADDRESS_4_OFFSET        4
#define DATA_OFFSET             5
//-------------

#define S25FL_READ_ID_BYTES           4 /* Read ID bytes count */
#define ID_CODE                       0x00010220

#define MAX_DATA                      1024*1024
#define FLASH_SPI_SELECT              0x01

extern u8 WriteBuffer[MAX_DATA + DATA_OFFSET + DUMMY_SIZE];
extern u8 ReadBuffer[MAX_DATA + DATA_OFFSET];

XStatus spiSetupFlash(XSpi *SpiPtr);
XStatus FlashWrite(XSpi *SpiPtr, u32 Address, uint8_t *data, u32 size);
XStatus FlashRead(XSpi *SpiPtr, u32 Address, u8 *data, u8 size);

#endif
