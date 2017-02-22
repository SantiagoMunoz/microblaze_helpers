#include "system.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xbasic_types.h"
#include "gpio.h"
#include "mblaze_nt_types.h" // por el tipo boolean
#include "system.h"
#include "s_uart.h"

XGpio stGpioDiscrete;
u32 ulGpioDiscreteStatus;

XGpio stGpioControl;
u32 ulGpioControlStatus;

XGpio stGpioGPIOREG;
u32 ulGpioGPIOREGStatus;

XStatus GpioInit(XGpio *pstGpio, u16 usGpioId)
{
  u16 iStatus;
  iStatus = XGpio_Initialize(pstGpio,usGpioId);
  if (iStatus != XST_SUCCESS)
    return iStatus;

  iStatus = XGpio_SelfTest(pstGpio);
  if (iStatus != XST_SUCCESS)
      return iStatus;

  ulGpioDiscreteStatus = 0;
  ulGpioControlStatus = 0;
  ulGpioGPIOREGStatus = 0;

  return iStatus;
}

void GpioConfigStart(XGpio *pstGpio, u32 ulIntrMask, u32 ulDirMask,  boolean bIsDual, boolean bIsBidir)
{
  XGpio_SetDataDirection(pstGpio,CH_ONE,ulDirMask);

  if(bIsDual == TRUE)
  {
	  if(bIsBidir == TRUE)
		  XGpio_SetDataDirection(pstGpio,CH_TWO,~ulDirMask);
	  else
		  XGpio_SetDataDirection(pstGpio,CH_TWO,ulDirMask);
  }
  if(pstGpio->InterruptPresent == TRUE)
  {
    XGpio_InterruptEnable(pstGpio,ulIntrMask);
    XGpio_InterruptGlobalEnable(pstGpio);
  }
}

void WriteGpio(XGpio *stGpio, u8 Channel, u32 ulValue)
{
  XGpio_DiscreteWrite(stGpio,Channel,ulValue);
}

XStatus SafeWriteGpio(XGpio *stGpio, u8 Channel, u32 ulValue, u32 Mask)
{
  if(stGpio == &stGpioDiscrete)
  {
	  ulGpioDiscreteStatus = (ulGpioDiscreteStatus & ~Mask) | ulValue;
	  XGpio_DiscreteWrite(stGpio, Channel, ulGpioDiscreteStatus);
  }
  else if (stGpio == &stGpioControl)
  {
	  ulGpioControlStatus = (ulGpioControlStatus & ~Mask) | ulValue;
	  XGpio_DiscreteWrite(stGpio, Channel, ulGpioControlStatus);
  }
  else if (stGpio == &stGpioGPIOREG)
  {
	  ulGpioDiscreteStatus = (ulGpioDiscreteStatus & ~Mask) | ulValue;
	  XGpio_DiscreteWrite(stGpio, Channel, ulGpioDiscreteStatus);
  }
  else
  {
	  UartSend("Trying to write into an invalid register\ni\r\0");
	  return XST_FAILURE;
  }

  return XST_SUCCESS;
}

u32 ReadGpio(XGpio *stGpio, u8 Channel)
{
  u32 ulGpioIn;

  ulGpioIn = XGpio_DiscreteRead(stGpio,Channel);

  return ulGpioIn;
}
