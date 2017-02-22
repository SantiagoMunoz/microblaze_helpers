
#ifndef GPIO_H_
#define GPIO_H_

/*****************************************************************************/
/*                                INCLUDES         		                     */
/*****************************************************************************/
#include "xgpio.h"
#include "xstatus.h"
#include "xbasic_types.h"
#include "mblaze_nt_types.h" // por el tipo boolean
#include "xgpio_l.h" // por XGPIO_IR_CH1_MASK

/*****************************************************************************/
/*                                DEFINES         		                     */
/*****************************************************************************/
#define BIT_0 0
#define BIT_1 1

#define BIT_VALID_ADC_0 (1<<28)
#define BIT_LAST_ADC_0  (1<<29)

#define GPIO_INTR_EN           XGPIO_IR_MASK
#define GPIO_INTR_CH1_EN       XGPIO_IR_CH1_MASK
#define GPIO_INTR_CH2_EN       XGPIO_IR_CH2_MASK
#define GPIO_DISABLE_INTR_MASK 0x00000000

#define CH_ONE 1
#define CH_TWO 2

// GPIO Directions

#define GPIO_IS_INPUT  0xFFFFFFFF
#define GPIO_IS_OUTPUT 0x00000000

/*****************************************************************************/
/*                                 VARIABLES                             	 */
/*****************************************************************************/
extern XGpio stGpioDiscrete;
extern u32 ulGpioDiscreteStatus;

extern XGpio stGpioControl;
extern u32 ulGpioControlStatus;

extern XGpio stGpioGPIOREG;
extern u32 ulGpioGPIOREGStatus;


/*****************************************************************************/
/*                                  FUNCIONES                                */
/*****************************************************************************/
XStatus GpioInit(XGpio *pstGpio, u16 usGpioId);
void GpioConfigStart(XGpio *pstGpio, u32 ulIntrMask, u32 ulDirMask,  boolean bIsDual, boolean bIsBidir);

void WriteGpio(XGpio *stGpio, u8 Channel, u32 ulValue);
XStatus SafeWriteGpio(XGpio *stGpio, u8 Channel, u32 ulValue, u32 Mask);
u32 ReadGpio(XGpio *stGpio, u8 Channel);



#endif /*GPIO_H_*/
