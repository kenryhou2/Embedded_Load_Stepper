// ----------------------------------------------------------------------------
// initPorts.c
// 
// This function drives all the I/O's as inputs tied low using the MSP430's
// internal pulldown resistor. This avoids floating inputs (which causes extra 
// power to be consumed).  This file is compatible with most MSP430 devices,
// including the F5529, FR5969, FR6989 and FR4133 Launchpads.
//
// It may not be compatible with custom hardware, if you have components
// attached to the I/Os that could be affected by these settings. So if
// you are using other boards, please verify/modify function before using it.
//
// ----------------------------------------------------------------------------
#include "driverlib.h"

#define GPIO_ALL        GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|  \
                        GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7


void initPorts(void)
{
#ifdef __MSP430_HAS_PORT1_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT2_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT3_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT4_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT5_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT6_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT7_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P7, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT8_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P8, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT9_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P9, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORTJ_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_PJ, GPIO_ALL);
#endif

#ifdef __MSP430FR4133
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif

#ifdef __MSP430FR5969
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif

#ifdef __MSP430FR6989
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif
}

