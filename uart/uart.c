
#include <stdarg.h>
#include <stdio.h>
#include "xuartlite.h"
#include "xstatus.h"
#include "xbasic_types.h"
#include "uart.h"
#include "mblaze_nt_types.h" // por el tipo boolean
#include "gpio.h"
#include "aux_functions.h"
XUartLite stUart; /*!< Uart UART_0 peripheral software instance */

u8 aucUartRxBuffer[UART_0_RX_BUFFER_SIZE];
u16 usUartRxBufferIndex;
u8 ucFlechaIn;

/**************************************************************************/
/* Name    uartInit                                                       */

XStatus UartInit(XUartLite *pstUart, u16 usUartId)
{
  XStatus iStatus;

  iStatus = XUartLite_Initialize(pstUart,usUartId);
  if (iStatus != XST_SUCCESS)
    return iStatus;
    
  XUartLite_ResetFifos(pstUart);
    
  iStatus = XUartLite_SelfTest(pstUart);
  if (iStatus != XST_SUCCESS)
    return iStatus;

  return XST_SUCCESS;
}

/**************************************************************************/
/* Name    uartSetHandler                                                 */


void UartSetHandler(XUartLite *pstUart, XUartLite_Handler funcRecvIsr, XUartLite_Handler funcSendIsr)
{
  // Asocia uart_isr para Tx y Rx
  XUartLite_SetRecvHandler(pstUart,(XUartLite_Handler)funcRecvIsr,(void *)pstUart);
  XUartLite_SetSendHandler(pstUart,(XUartLite_Handler)funcSendIsr,(void *)pstUart);
}

/**************************************************************************/
/* Name    uartConfigStart                                                */

void UartConfigStart(XUartLite *pstUart, boolean bEnableIntr)
{
  if(bEnableIntr)
    XUartLite_EnableInterrupt(pstUart);
  else
    XUartLite_DisableInterrupt(pstUart);
}

/**************************************************************************/
/* Name    uartEcho                                                       */

void UartEcho(u8 *paucBufferIn)
{
  while(stSystem.ucUartTx)
  {}
  stSystem.ucUartTx = TRUE;
  XUartLite_Send(&stUart,paucBufferIn,1);
}

/**************************************************************************/
/* Name    uartSend                                                       */

void UartSend(u8 *paucBufferIn)
{
  u16 usNumChar=0;
  u8 ucCharacter;

  while(stSystem.ucUartTx)
  {}
  ucCharacter = *(paucBufferIn+usNumChar);
  //Cuento el numero de letras a enviar
  while(ucCharacter != "\0")
  {
    usNumChar++;
    ucCharacter = *(paucBufferIn+usNumChar);
  }

  stSystem.ucUartTx = TRUE;
  XUartLite_Send(&stUart,paucBufferIn,usNumChar);

}

void UartPrint(char *format,...)
{
	char buffer[100];

	  va_list args;
	  va_start(args, format);
	  vsprintf(buffer, format, args);

	  UartSend((u8*)buffer);
	  va_end(args);
}

void UartMenu(void)
{
  usUartRxBufferIndex=0;
  UartSend( (u8 *) "\r\n 1. DISCRETE TEST \r\n");
  UartSend( (u8 *) "2. READ BITES \r\n");
  UartSend( (u8 *) "OPTION = ");
  WaitUartRx();

}


void UartRxIsr(void *CallBackRef, u16 usEventData)
{
  u8 aucDatoRx[1];

  XUartLite_Recv((XUartLite *)CallBackRef,aucDatoRx,1);
  if (aucDatoRx[0]=='\033')//Primer caracter cuando se pulsa una flecha
  {
    ucFlechaIn =1;
  }

  if (ucFlechaIn !=0)
  {
    if (ucFlechaIn ==3)
      ucFlechaIn =0;
    else
      ucFlechaIn ++;
  }
  else
  {
    if (aucDatoRx[0]== '\b')
    {
      UartSend( (u8 *) "\b \b");
      if (usUartRxBufferIndex !=0)
        usUartRxBufferIndex --;
    }
    else
    {
      UartEcho(aucDatoRx);
      aucUartRxBuffer[usUartRxBufferIndex]=aucDatoRx[0];
      usUartRxBufferIndex ++;
    }
    stSystem.ucUartRx = (u8)TRUE;
  }
}


void UartTxIsr(void *CallBackRef, u16 usEventData)
{
  stSystem.ucUartTx = FALSE;
}


void UartMain(void)
{
  XStatus eResult;
  u8 ucCommand;

  UartMenu();
  stSystem.ucUartRx = FALSE;
  if (aucUartRxBuffer[usUartRxBufferIndex-1] == '\n')
  {
    //Only one character accepted
    if ((usUartRxBufferIndex-3) ==0)
    {
  	  ucCommand = (u8)stringDec2Int(aucUartRxBuffer,1);;
      switch(ucCommand)
      {
        case 1:
        	eResult=DiscreteTestMenu();
        break;
        case 2:
        	eResult=ReadBitesMenu();
        break;
        default:
        	 eResult= XST_FAILURE;
        break;
      }
    }
    else
    {
      eResult= XST_FAILURE;
    }
  }
}


void WaitUartRx(void)
{
  while(stSystem.ucUartRx == FALSE)
  {}
  while (aucUartRxBuffer[usUartRxBufferIndex-1] != '\n')
  {}

  stSystem.ucUartRx = FALSE;
}

XStatus DiscreteTestMenu(void)
{
	boolean bExit;
	u8 ucConnectorNumber,ucPinNumber,ucPinValue;
	XStatus eResult;

	bExit = FALSE;

	do{
		usUartRxBufferIndex=0;
		UartSend( (u8 *) "\r\nSELECT CONNECTOR \r\n");
		UartSend( (u8 *) "1.- CONNECTOR J1 \r\n");
		UartSend( (u8 *) "2.- CONNECTOR J2 \r\n");
		UartSend( (u8 *) "0- EXIT\r\n");
		WaitUartRx();
		ucConnectorNumber  = (u8)stringDec2Int(aucUartRxBuffer,usUartRxBufferIndex-2);
		if((ucConnectorNumber == 1) || (ucConnectorNumber == 2))
		{
			usUartRxBufferIndex=0;
			UartSend( (u8 *) "\r\n1- SELECT PIN NUMBER (1 to 40) \r\n");
			WaitUartRx();
			ucPinNumber  = (u8)stringDec2Int(aucUartRxBuffer,usUartRxBufferIndex-2);
			if (ucPinNumber < 1 || ucPinNumber > 40)
			{
				UartSend( (u8 *) "\r\nPIN NUMBER NOT VALID \r\n");
				eResult=XST_FAILURE;
				bExit=TRUE;
			}
			else
			{
				usUartRxBufferIndex=0;
				UartSend( (u8 *) "\r\nSELECT VALUE (0 or 1) \r\n");
				WaitUartRx();
				ucPinValue  = (u8)stringDec2Int(aucUartRxBuffer,usUartRxBufferIndex-2);
			}
			eResult= SetPinValue(ucConnectorNumber,ucPinNumber,ucPinValue);
		}
		else
			bExit=TRUE;
		    eResult=XST_FAILURE;
	}while(!bExit);

	return eResult;
}

XStatus ReadBitesMenu(void)
{
	boolean bExit;
	u8 ucOption;
	u32 ulBites;

	bExit = FALSE;

	do{
		usUartRxBufferIndex=0;
		UartSend( (u8 *) "\r\nSELECT OPTION \r\n");
		UartSend( (u8 *) "1.- READ ALL BITES \r\n");
		UartSend( (u8 *) "2.- READ BITE 744MHz LOCK \r\n");
		UartSend( (u8 *) "3.- READ BITE 744MHz NIVEL \r\n");
		UartSend( (u8 *) "4.- READ BITE REFERENCE OSCILATOR LEVEL \r\n");
		UartSend( (u8 *) "0.- EXIT \r\n");
		WaitUartRx();
		ucOption  = (u8)stringDec2Int(aucUartRxBuffer,usUartRxBufferIndex-2);
		ulBites=ReadGpio(&stGpioControl,CH_TWO);
		if(ucOption==1)
		{
			UartPrint("BITES: 0x%x\r\n",(u8)(ulBites & 0x7));
			if((ulBites & 0x7) == 7)
				UartSend((u8 *)"ALL BITES OK\r\n");
			else
				UartSend((u8 *)"ONE OR MORE BITES ARE NOT OK\r\n");
		}
		else if(ucOption==2)
		{
			if(((ulBites & BITE_744MHZ_LOCK_MASK) >> BITE_744MHZ_LOCK_POS) == 1)
				UartSend((u8 *)"744MHz LOCK\r\n");
			else
				UartSend((u8 *)"744MHz NO LOCK\r\n");
		}
		else if(ucOption==3)
		{
			if(((ulBites & BITE_744MHZ_NIVEL_MASK) >> BITE_744MHZ_NIVEL_POS) == 1)
				UartSend((u8 *)"744MHz NIVEL OK \r\n");
			else
				UartSend((u8 *)"744MHz NIVEL NOT OK \r\n");
		}
		else if(ucOption==4)
		{
			if(((ulBites & REF_OSC_LEVEL_OUT_MASK) >> REF_OSC_LEVEL_OUT_POS) == 1)
				UartSend((u8 *)"REFERENCE OSCILATOR LEVEL OK \r\n");
			else
				UartSend((u8 *)"REFERENCE OSCILATOR LEVEL NOT OK \r\n");
		}
		else
			bExit=TRUE;
	}while(!bExit);

	return XST_SUCCESS;
}
