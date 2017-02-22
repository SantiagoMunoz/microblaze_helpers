#ifndef IIC_EEPROM_H_
#define IIC_EEPROM_H_

#include "iic.h"
#include "timer.h"
#include "aux_functions.h"

/* -- Specific functions to deal with the iic EEPROM -- */

XStatus eeprom_write_byte(XIic *pstIic, u8 address_high, u8 address_low, u8 write_byte);
XStatus eeprom_read_current(XIic *pstIic, u8 address_high, u8 address_low, u8 *read_byte);

#endif
