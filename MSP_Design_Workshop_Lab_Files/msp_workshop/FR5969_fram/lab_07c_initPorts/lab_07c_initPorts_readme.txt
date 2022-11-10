lab_07c_initPorts

This lab explores the effect that GPIO initialization has on system power 
consumption. We should find that initializing all of the pins will result
in lower energy being consumed.

General procedure:
- Create lab_07c_initPorts (by renaming previously imported lab_04c_crystals_solution)
- Replace while{} loop with __low_power_mode_3() intrinsic function call
- Capture energy baseline (how much energy does program use before initializing ports)
- Add initPortsAsOutputs.c file and call its initPorts() function from main.
  Note: this MUST BE DONE before the call to our initGPIO() function
- Capture energy information and compare to previous result
- Add a second file, initPorts.c, to the project. (This sets all GPIO as inputs.)
- Set initPortsAsOutputs.c as "Exclude From Build"
- Capture energy information again, and compare to previous results

File source code in this readme file:
- main.c
- initPorts.c
- initPortsAsOutputs.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_07c_initPorts project) ('FR5969 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myClocks.h"
#include "initPorts.h"


//***** Prototypes ************************************************************
void initGPIO(void);


//***** Defines ***************************************************************
#define ONE_SECOND    myMCLK_FREQUENCY_IN_HZ
#define HALF_SECOND   myMCLK_FREQUENCY_IN_HZ / 2


//***** Global Variables ******************************************************


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Initialize GPIO Ports
    initPorts();

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    // Enter Low Power Mode 3 (LPM3)
    __low_power_mode_3();

//    while(1) {
//        // Turn on LED
//        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//
//        // Wait about a second
//        __delay_cycles( HALF_SECOND );
//
//        // Turn off LED
//        GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//
//        // Wait another second
//        __delay_cycles( HALF_SECOND );
//    }
}

//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {

    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set pin P9.7 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN6 );                             // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN6 );

    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    PMM_unlockLPM5();

    // Set LFXT (low freq crystal pins) to crystal input (rather than GPIO)
    // Since HFXT is not used, we don't need to set these pins. But for the 
    // record, they are:
    //              GPIO_PIN6                            // HFXTIN on PJ.6
    //              GPIO_PIN7                            // HFXOUT on PJ.7
    GPIO_setAsPeripheralModuleFunctionInputPin(
                    GPIO_PORT_PJ,
                    GPIO_PIN4 +                          // LFXIN  on PJ.4
                    GPIO_PIN5 ,                          // LFXOUT on PJ.5
                    GPIO_PRIMARY_MODULE_FUNCTION
    );

//  Output the ACLK and SMCLK signals to their respective pins to allow watching them with logic analyzer
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P2,
//                    GPIO_PIN0 +                          // ACLK on P2.0
//                    GPIO_PRIMARY_MODULE_FUNCTION
//    );
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P3,
//                    GPIO_PIN4 +                          // SMCLK on P3.4
//                    GPIO_PRIMARY_MODULE_FUNCTION
//    );
}



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



// ----------------------------------------------------------------------------
// initPortsAsOutputs.c
// 
// This function drives all the I/O's as output-low, to avoid floating inputs
// (which causes extra power to be consumed).  This file is compatible with
// most MSP430 devices, including the F5529, FR5969, FR6989 and FR4133 Launchpads.
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
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT2_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT3_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT4_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT5_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT6_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT7_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT8_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT9_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORTJ_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_ALL);
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


