/*
 * iic.c
 *
 *  Created on: 25/10/2016
 *      Author: asserrano
 */

#include "xiic.h"
#include "iic.h"

XIic stIic;

HandlerInfo_type HandlerInfo;

XStatus IicInit(XIic *pstIic, u16 usIicId)
{
	int Status;
	XIic_Config *ConfigPtr;

	ConfigPtr = XIic_LookupConfig(usIicId);
	if (ConfigPtr == NULL)
		return XST_FAILURE;

	Status = XIic_CfgInitialize(pstIic, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	return XST_SUCCESS;
}

XStatus IicConfigStart(XIic *pstIic)
{
	int Status;

	SetHandler(pstIic);
	Status = XIic_Start(pstIic);

	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	HandlerInfo.EventStatusUpdated = FALSE;
	HandlerInfo.RecvBytesUpdated = FALSE;
	Status = XST_FAILURE;

	return XST_SUCCESS;

}

XStatus IicSend(XIic *pstIic, u16 usIicAddr, u8 *pucDat, u8 ucNumDat)
{
  XStatus iStatus;
  u16 usCpuCycles = 0;

  iStatus = XIic_SetAddress(pstIic, XII_ADDR_TO_SEND_TYPE,usIicAddr);
  if (iStatus != XST_SUCCESS)
    return XST_FAILURE;

  HandlerInfo.EventStatusUpdated = FALSE;
  HandlerInfo.SendBytesUpdated = FALSE;
  iStatus = XST_FAILURE;

  iStatus = XIic_Start(pstIic);
  if (iStatus != XST_SUCCESS)
    return XST_FAILURE;

  iStatus = XIic_MasterSend(pstIic, pucDat, ucNumDat);
  if (iStatus != XST_SUCCESS)
    return XST_FAILURE;

	while(1) {
		if(HandlerInfo.SendBytesUpdated == TRUE) {
			HandlerInfo.SendBytesUpdated = FALSE;
			/*
			 * The device information has been updated for receive
			 * processing,if all bytes received (1), indicate
			 * success
			 */
			if (HandlerInfo.RemainingRecvBytes == 0) {
				iStatus = XST_SUCCESS;
			}
			break;
		}
		/*
		 * Any event status which occurs indicates there was an error,
		 * so return unsuccessful, for this example there should be no
		 * status events since there is a single master on the bus
		 */
		if (HandlerInfo.EventStatusUpdated == TRUE) {
			return XST_FAILURE;
		}
	}


  while (XIic_IsIicBusy(pstIic)== TRUE)
  {
	usCpuCycles++;
  }

  iStatus = XIic_Stop(pstIic);

 /* if (pstIic->Stats.TxErrors != 0)
  {
    pstIic->Stats.TxErrors = 0;
    return XST_FAILURE;
  }*/

  return XST_SUCCESS;

}

XStatus IicRecv(XIic *pstIic, u16 usIicAddr, u8 *pucDatRx, u8 ucNumDatRx)
{
  XStatus iStatus;
  u16 usCpuCycles = 0;

  iStatus = XIic_SetAddress(pstIic, XII_ADDR_TO_SEND_TYPE,usIicAddr);
  if (iStatus != XST_SUCCESS)
    return XST_FAILURE;

  iStatus = XIic_MasterRecv(pstIic, pucDatRx, ucNumDatRx);
  if (iStatus != XST_SUCCESS)
    return XST_FAILURE;

  while (XIic_IsIicBusy(pstIic)== TRUE)
  {
	usCpuCycles++;

	//if (usCpuCycles==MAX_CPU_CYCLES)
	  //return XST_FAILURE;
  };

  iStatus = XIic_Stop(pstIic);


 /* if (pstIic->Stats.TxErrors != 0)
  {
    pstIic->Stats.TxErrors = 0;
    return XST_FAILURE;
  }
 */

  return XST_SUCCESS;
}

XStatus IicSendNReceive(XIic *pstIic, u16 usIicAddr, u8 address_high, u8 address_low, u8 *pucDatRx, u8 ucNumDatRx)
{
	// This method sends the Iic address, then issues a read command before giving the STOP condition
	XStatus iStatus;
	u8 address[2];
	u16 usCpuCycles = 0;

	address[0] = address_high;
	address[1] = address_low;

	iStatus = XIic_SetAddress(pstIic, XII_ADDR_TO_SEND_TYPE,usIicAddr);
	  if (iStatus != XST_SUCCESS)
	    return XST_FAILURE;

	iStatus = XIic_MasterSend(pstIic, address, 2);
    if (iStatus != XST_SUCCESS)
        return XST_FAILURE;

    while(1) {
    	if(HandlerInfo.SendBytesUpdated == TRUE) {
    		HandlerInfo.SendBytesUpdated = FALSE;
   			if (HandlerInfo.RemainingRecvBytes == 0) {
    			iStatus = XST_SUCCESS;
   			}
   			break;
   		}
    	if (HandlerInfo.EventStatusUpdated == TRUE) {
    		return XST_FAILURE;
   		}
   	}
    while (XIic_IsIicBusy(pstIic)== TRUE)
    {
    	usCpuCycles++;
    }

    iStatus = XIic_MasterRecv(pstIic, pucDatRx, ucNumDatRx);
   	if (iStatus != XST_SUCCESS)
   		return XST_FAILURE;
    	while (XIic_IsIicBusy(pstIic)== TRUE)
    	  {
    		usCpuCycles++;
    	  };

    iStatus = XIic_Stop(pstIic);

    return XST_SUCCESS;
}
void SetHandler(XIic *pstIic)
{
	XIic_SetSendHandler(pstIic, (void *)&HandlerInfo, SendHandler);
	XIic_SetRecvHandler(pstIic, (void *)&HandlerInfo, RecvHandler);
	XIic_SetStatusHandler(pstIic, (void *)&HandlerInfo, StatusHandler);
}

void SendHandler(void *CallbackRef, int ByteCount)
{
	HandlerInfo.RemainingSendBytes = ByteCount;
	HandlerInfo.SendBytesUpdated = TRUE;
}

void RecvHandler(void *CallbackRef, int ByteCount)
{
	HandlerInfo.RemainingRecvBytes = ByteCount;
	HandlerInfo.RecvBytesUpdated = TRUE;
}

void StatusHandler(void *CallbackRef, int Status)
{
	HandlerInfo.EventStatus |= Status;
	HandlerInfo.EventStatusUpdated = TRUE;
}


