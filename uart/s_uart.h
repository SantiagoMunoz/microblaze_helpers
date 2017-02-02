#ifndef S_UART_H_
#define S_UART_H_

#include <stdarg.h>
#include <stdio.h>
#include "xparameters.h"
#include "xuartlite.h"
#include "xstatus.h"
#include "xbasic_types.h"
#include "system.h"

extern XUartLite stUart;

extern u8 aucUartRxBuffer[32];
extern u8 usUartRxBufferIndex;

XStatus UartInit(XUartLite *pstUart, u16 Uart_ID);
XStatus UartConfig(XUartLite *psUart);

XStatus UartSetHandler(XUartLite *psUart, XUartLite_Handler RxISR, XUartLite_Handler TxISR);

void UartRxIsr(void *CallBackRef, u16 usEventData);
void UartTxIsr(void *CallBackRef, u16 usEventData);

void UartWait4Rx(void);

XStatus UartSend(u8 *text);
void UartPrint(char *format,...);

#endif
