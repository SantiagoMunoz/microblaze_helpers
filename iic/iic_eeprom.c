#include "iic_eeprom.h"

/**
 *  WRITE_BYTE
 *  ----------
 *
 *  The instruction follows the following sequence 
 *
 *  Start Condition -> Address MSB + Address LSB -> (Ack from Slave) -> Data Byte -> (Ack from Slave) -> Stop Condition
 *
 **/
XStatus eeprom_write_byte(XIic *pstIic, u8 address_high, u8 address_low, u8 write_byte)
{
    XStatus iStatus;
    //u8 address;
    u8 control_byte; // Last byte means "Not Write"
    u8 message[4];

    control_byte = 0xA0 | ((address_low & 0x07) << 1);
    message[0] = control_byte;
    message[1] = address_high;
    message[2] = address_low;
    message[3] = write_byte;

    //iStatus = IicSend(pstIic, AT_ADDR_EEPROM, message, 4);
    //address = AT_ADDR_EEPROM | (address_low & 0x07);
    //iStatus = IicSend(pstIic, AT_ADDR_EEPROM, &message[1], 3);
    iStatus = IicSend(pstIic, AT_ADDR_EEPROM, &message[1], 4);
    if(iStatus != XST_SUCCESS)
    	return XST_FAILURE;

    timerdelay(2); //Pause for .5 secs

    //Write previous address so the internal address points to the correct one.
    address_low -= 1;
    control_byte = 0xA0 | ((address_low & 0x07) << 1);
    message[0] = control_byte;
    message[2] = address_low;
    //return IicSend(pstIic, AT_ADDR_EEPROM, message, 4);
    //address = AT_ADDR_EEPROM | (address_low & 0x07);
    //iStatus = IicSend(pstIic, AT_ADDR_EEPROM, &message[1], 3);
    iStatus = IicSend(pstIic, AT_ADDR_EEPROM, &message[1], 4);

    timerdelay(2);

    return iStatus;
}

/**
 * READ_BYTE
 * ---------
 *
 * The EEPROM remembers the last used address. 
 *
 * This address can be read again by issuing a control byte with the last byte to 1.
 *
 */ 
XStatus eeprom_read_current(XIic *pstIic, u8 address_high, u8 address_low, u8 *read_byte)
{
    XStatus iStatus;
    u8 address[2];

    address[0] = address_high;
    address[1] = address_low;

   // iStatus = IicSend(pstIic, AT_ADDR_EEPROM, &address, 2);
   // if(iStatus != XST_SUCCESS)
    //    return XST_FAILURE;
    
    iStatus =  IicRecv(pstIic, AT_ADDR_EEPROM, read_byte, 1);

    timerdelay(2);

    return iStatus;
}
