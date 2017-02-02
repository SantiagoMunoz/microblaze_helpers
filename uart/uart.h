#ifndef UART_H_
#define UART_H_

/*****************************************************************************/
/*                                INCLUDES         		                     */
/*****************************************************************************/
#include "system.h"
#include "xuartlite.h"
#include "xstatus.h"
#include "xbasic_types.h"
#include "mblaze_nt_types.h"

/*****************************************************************************/
/*                                DEFINES         		                     */
/*****************************************************************************/
#define UART_0_ENABLE_INTR 1 // habilita interrupciones (interrupt mode)

#define UART_0_RX_BUFFER_SIZE 500 // Buffer size in bytes for OAM&P command interface buffer

#define NUM_CHAR_COMMAND_MAX 20
#define NUM_CHAR_PARAM_MAX 20
#define NUM_CHAR_VALOR_MAX 10

#define BITE_744MHZ_LOCK_POS  0
#define BITE_744MHZ_NIVEL_POS 1
#define REF_OSC_LEVEL_OUT_POS 2

#define BITE_744MHZ_LOCK_MASK  (1 << 0)
#define BITE_744MHZ_NIVEL_MASK (1 << 1)
#define REF_OSC_LEVEL_OUT_MASK (1 << 2)
#define T_CRIT_A_MASK          (1 << 3)
#define INT_MASK               (1 << 4)
/*****************************************************************************/
/*                                  TYPE                            	 	 */
/*****************************************************************************/

/*****************************************************************************/
/*                                 VARIABLES                             	 */
/*****************************************************************************/
extern XUartLite stUart;

extern u8 aucUartRxBuffer[UART_0_RX_BUFFER_SIZE];
extern u16 usUartRxBufferIndex;
extern u8 ucFlechaIn;

/*****************************************************************************/
/*                                  FUNCIONES                                */
/*****************************************************************************/
XStatus UartInit(XUartLite *pstUart, u16 usUartId);
void UartSetHandler(XUartLite *pstUart, XUartLite_Handler funcRecvIsr, XUartLite_Handler funcSendIsr);
void UartConfigStart(XUartLite *pstUart, boolean bEnableIntr);

void UartEcho(u8 *paucBufferIn);
void UartSend(u8 *paucBufferIn);
void UartPrint(char *format,...);
void UartMenu(void);
void UartRxIsr(void *CallBackRef, u16 usEventData);
void UartTxIsr(void *CallBackRef, u16 usEventData);
void UartMain(void);
void WaitUartRx(void);

XStatus DiscreteTestMenu(void);
XStatus DiscreteTestFunction(void);
XStatus ReadBitesMenu(void);
XStatus ReadBites(void);

#endif /*UART_H_*/
