/*
 * iic.h
 *
 *  Created on: 25/10/2016
 *      Author: asserrano
 */

#ifndef IIC_H_
#define IIC_H_

#include "xiic.h"

typedef struct {
	int  EventStatus;
	int  RemainingSendBytes;
	int  RemainingRecvBytes;
	int  EventStatusUpdated;
	int  SendBytesUpdated;
	int  RecvBytesUpdated;
} HandlerInfo_type;

extern HandlerInfo_type HandlerInfo;

#define MAX_CPU_CYCLES 1000
#define EVENT_COUNTER_ADDR 0x6B
#define EEPROM_ADDR 0x50
#define TEMP_SENSE_ADDR 0x18


/* -- AT PERIPHALS  -- */
#define AT_ADDR_TEMP          0x18
#define AT_ADDR_EEPROM        0x50
#define AT_ADDR_EVENTCTR      0x6B
#define AT_ADDR_ADCTEMP       0x48  //0x4D is the other (temporarily disconnected) ADC

extern XIic stIic;

XStatus IicInit(XIic *pstIic, u16 usIicId);
XStatus IicConfigStart(XIic *pstIic);
XStatus IicSend(XIic *pstIic, u16 usIicAddr, u8 *pucDat, u8 ucNumDat);
XStatus IicRecv(XIic *pstIic, u16 usIicAddr, u8 *pucDatRx, u8 ucNumDatRx);
XStatus IicSendNReceive(XIic *pstIic, u16 usIicAddr, u8 address_high, u8 address_low, u8 *pucDatRx, u8 ucNumDatRx);
void SetHandler(XIic *pstIic);
void SendHandler(void *CallbackRef, int ByteCount);
void RecvHandler(void *CallbackRef, int ByteCount);
void StatusHandler(void *CallbackRef, int Status);

#endif /* IIC_H_ */
