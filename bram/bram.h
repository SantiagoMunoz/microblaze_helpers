#ifndef BRAM_H_
#define BRAM_H_

#include "xparameters.h"
#include "xbram.h"

extern XBram stBram_ADC0CH0;
extern XBram_Config stBram_config_ADC0CH0;

extern XBram stBram_ADC0CH1;
extern XBram_Config stBram_config_ADC0CH1;

extern XBram stBram_ADC1CH0;
extern XBram_Config stBram_config_ADC1CH0;

extern XBram stBram_ADC1CH1;
extern XBram_Config stBram_config_ADC1CH1;

XStatus bram_init(XBram *pstBram, XBram_Config *ConfigPtr, u16 device_id);

XStatus bram_config(XBram *pstBram, XBram_Config *ConfigPtr);

void InitializeEcc(XBram_Config *ConfigPtr, u32 EffectiveAddr);

u32 get_bram_value(XBram_Config *ConfigPtr, u32 offset);

#endif
