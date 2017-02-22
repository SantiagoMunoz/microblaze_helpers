#include "timer.h"

XTmrCtr stTimerCounter;
int ExpiredCounter;  //Number of overflows since last reset
int counter_cycles;  //Number of times ExpiredCounter increases before getting driven back to zero

XStatus InitCounter(XTmrCtr *instance, u16 device_id)
{
    XStatus iStatus;

    iStatus = XTmrCtr_Initialize(instance, device_id);
    if(iStatus != XST_SUCCESS)
        return XST_FAILURE;
    iStatus = XTmrCtr_SelfTest(instance, TIMER_0);
    if(iStatus != XST_SUCCESS)
        return XST_FAILURE;
    
    //Default timer expire counter before reset
    counter_cycles = 3;

    return XST_SUCCESS;
}

void ConfigCounter(XTmrCtr *instance, XTmrCtr_Handler FuncPtr)
{
    //Set callback
    XTmrCtr_SetHandler(instance, FuncPtr, instance);
    //
    XTmrCtr_SetOptions(instance, TIMER_0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION);

}

void SetTimerValue(XTmrCtr *instance, int value)
{
    XTmrCtr_SetResetValue(instance, TIMER_0, value);
}

void timer_start(XTmrCtr *instance)
{
    XTmrCtr_Start(instance, TIMER_0);
}

void timer_stop(XTmrCtr *instance)
{
    XTmrCtr_Stop(instance, TIMER_0);
}

void wait_for_timer_overflow()
{
    int stored_expired_count = ExpiredCounter;
    int current_count;
    while(1)
    {
        current_count = ExpiredCounter;
        if(current_count < stored_expired_count)
            break;
        stored_expired_count = current_count;
    }
    return;
}
   
void timer_callback(void *CallbackRef, u8 TmrCtrNumber)
{
    XTmrCtr *instance = (XTmrCtr *)CallbackRef;

    if(XTmrCtr_IsExpired(instance, TmrCtrNumber))
    {
        ExpiredCounter++;
        if(ExpiredCounter == counter_cycles)
            ExpiredCounter=0;
    }

}

void timerdelay(int no250MsSegments){

    SetTimerValue(&stTimerCounter, 0xBEBC20);
    counter_cycles = no250MsSegments;
    timer_start(&stTimerCounter);
    wait_for_timer_overflow();
    timer_stop(&stTimerCounter);
    
    return;
}

void fiftymstimerdelay(){

    //SetTimerValue(&stTimerCounter, 0x4C4B40);
    //counter_cycles = 5;
	SetTimerValue(&stTimerCounter, 0x1388);
	counter_cycles = 4;
    timer_start(&stTimerCounter);
    wait_for_timer_overflow();
    timer_stop(&stTimerCounter);
    
    return;
}

