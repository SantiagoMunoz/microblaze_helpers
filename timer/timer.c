#include "timer.h"

int ExpiredCounter;
XTmrCtr stTimerCounter;
int counter_cycles;

XStatus InitCounter(XTmrCtr *instance, u16 device_id)
{
    XStatus status;

    status = XTmrCtr_Initialize(instance, device_id);
    if(status != XST_SUCCESS)
        return XST_FAILURE;
    status = XTmrCtr_SelfTest(instance, TIMER_0);
    if(status != XST_SUCCESS)
        return XST_FAILURE;
    
    //Default timer expire counter before reset
    counter_cycles = 3;

    return XST_SUCCESS;
}

void ConfigCounter(XTmrCtr *instance, XTmrCtr_Handler FuncPtr)
{
    XStatus status;
    //Set callback
    XTmrCtr_SetHandler(instance, FuncPtr, instance);
    //
    XTmrCtrl_SetOptions(instance, TIMER_0, XTC_INT_MODE_OPTION |       XTC_AUTO_RELOAD_OPTION);

}

void SetTimerValue(XTmr *instance, int value)
{
    XTmrCtr_setResetValue(instamce, TIMER_0, value);
}

void timer_start(Xtmr *instance)
{
    XTmrCtr_Start(instance, TIMER_0);
}

void timer_stop(XTmr *instance)
{
    XTmrCtr_Stop(instance, TIMER_0);
}


void wait_for_timer_overflow()
{
    int stored_expired_count = ExpiredCounter;
    int current_count;
    while(true)
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
    XTmrCtr *instance = (XTrmCtr *)CallbackRef;

    if(XTmrCtr_IsExpired(instance, TmrCtrNumber))
    {
        ExpiredCounter++;
        if(ExpiredCounter == counter_cycles)
            ExpiredCounter=0;
    }

}
