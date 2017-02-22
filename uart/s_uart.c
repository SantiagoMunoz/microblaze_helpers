#include "s_uart.h"

XUartLite stUart;
                        
u8 aucUartRxBuffer[128];
u8 usUartRxBufferIndex;

XStatus UartInit(XUartLite *pstUart, u16 Uart_ID)
{
    XStatus iStatus;

    iStatus = XUartLite_Initialize(&stUart, Uart_ID);
    if(iStatus != XST_SUCCESS)
        return XST_FAILURE;
    iStatus = XUartLite_SelfTest(&stUart);
    if(iStatus != XST_SUCCESS)
        return XST_FAILURE;

    return XST_SUCCESS;
}

XStatus UartConfig(XUartLite *psUart)
{
    usUartRxBufferIndex = 0;
    return UartSetHandler(psUart, UartRxIsr, UartTxIsr);
}

XStatus UartSetHandler(XUartLite *psUart, XUartLite_Handler RxISR, XUartLite_Handler TxISR)
{
    XUartLite_SetSendHandler(psUart, TxISR, (void *)psUart);
    XUartLite_SetRecvHandler(psUart, RxISR, (void *)psUart);
    XUartLite_EnableInterrupt(psUart);
    return XST_SUCCESS;
}

void UartRxIsr(void *CallBackRef, u16 usEventData)
{
    u8 input_data;
    XUartLite_Recv( (XUartLite *)CallBackRef, &input_data, 1);
    if(input_data == '\b')
    {
        if(usUartRxBufferIndex > 0)
            usUartRxBufferIndex -= 1;
    }else{
        aucUartRxBuffer[usUartRxBufferIndex] = input_data;
        usUartRxBufferIndex += 1;
    }
    
    stSystem.ucUartRx = 1;
}

void UartTxIsr(void *CallBackRef, u16 usEventData)
{
    stSystem.ucUartTx = 0;
}

void UartWait4Rx(void)
{
    while( (stSystem.ucUartRx == 0) | (aucUartRxBuffer[usUartRxBufferIndex-1] != '\n') ){}

    stSystem.ucUartRx = 0;
}

XStatus UartSend(char *text)
{
    XStatus iStatus;

	u16 char_counter = 0;
    
    while(stSystem.ucUartTx){}

    while(*(text + char_counter) != '\0')
    {
        char_counter++;
    }
    if(char_counter == 0)
    	return XST_SUCCESS;
    
    stSystem.ucUartTx = 1;
    iStatus = XUartLite_Send(&stUart, (u8*)text, char_counter);
    while(stSystem.ucUartTx){}
    return iStatus;
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
