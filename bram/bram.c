#include "bram.h"

XBram stBram;


XStatus bram_init(XBram *pstBram, XBram_Config *ConfigPtr, u16 device_id)
{
    XStatus iStatus;

    ConfigPtr = XBram_LookupConfig(device_id);
    if(ConfigPtr == NULL)
        return XST_FAILURE;

    iStatus = XBram_CfgInitialize(pstBram, ConfigPtr, ConfigPtr->CtrlBaseAddress);
    if(iStatus != XST_SUCCESS)
       return XST_FAILURE; 


    return XST_SUCCESS;
}

XStatus bram_config(XBram *pstBram, XBram_Config *ConfigPtr)
{
    XStatus iStatus;
    InitializeEcc(ConfigPtr, ConfigPtr->CtrlBaseAddress);
    iStatus = XBram_SelfTest(pstBram, 0);
    if(iStatus != XST_SUCCESS)
        return XST_FAILURE;

    return XST_SUCCESS;
}

void InitializeEcc(XBram_Config *ConfigPtr, u32 EffectiveAddr)
{
    u32 Addr;
    volatile u32 Data;

    if (ConfigPtr->EccPresent &&
        ConfigPtr->EccOnOffRegister &&
        ConfigPtr->EccOnOffResetValue == 0 &&
        ConfigPtr->WriteAccess != 0) {
        for (Addr = ConfigPtr->MemBaseAddress;
             Addr < ConfigPtr->MemHighAddress; Addr+=4) {
            Data = XBram_In32(Addr);
            XBram_Out32(Addr, Data);
        }
        XBram_WriteReg(EffectiveAddr, XBRAM_ECC_ON_OFF_OFFSET, 1);
    }
}

u32 get_bram_value(XBram_Config *ConfigPtr, u32 offset)
{
    return XBram_ReadReg(ConfigPtr->MemBaseAddress, offset);
}

