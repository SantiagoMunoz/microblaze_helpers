#ifndef TIMER_H_
#define TIMER_H_

#include "system.h"
//#include "status.h"
#include "xbasic_types.h"
#include "mblaze_nt_types.h"

#include "xparameters.h"
#include "xil_exception.h"
#include "xintc.h"
#include <stdio.h>
#include "xstatus.h"
#include "xtmrctr.h"

#include "intc.h"

#define TIMER_0         0

/**
 * Note: Modify counter_cycler control the number of timer overflows
 * before resetting ExpiredCounter
 **/

extern int ExpiredCounter;
extern XTmrCtr stTimerCounter;
extern int counter_cycles;

XStatus InitCounter(XTmrCtr *instance, u16 device_id);
void ConfigCounter(XTmrCtr *instance, XTmrCtr_Handler FuncPtr);

void SetTimerValue(XTmrCtr *instance, int value);//value = start number of the decrementing count
void timer_start(XTmrCtr *instance);
void timer_stop(XTmrCtr *instance);
void wait_for_timer_overflow();

//Function that manager the timer increment increment each time the timer expired
void timer_callback(void *CallbackRef, u8 TmrCtrNumber);

#endif
