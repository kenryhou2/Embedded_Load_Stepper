// ----------------------------------------------------------------------------
// myGpio.c  (for lab_06d_simplePWM project)  ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"


//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {

    //**************************************************************************
    // Configure LaunchPad LEDs
    //**************************************************************************
    // Set red and green LEDs to output direction, then turn them off
    GPIO_setAsOutputPin(
            GPIO_PORT_P1,
            GPIO_PIN0 +                                                         // Red LED (LED1)
            GPIO_PIN1                                                           // Green LED (LED2)
    );

    GPIO_setOutputLowOnPin(
            GPIO_PORT_P1,
            GPIO_PIN0 +
            GPIO_PIN1
    );

    //**************************************************************************
    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    //**************************************************************************
    PMM_unlockLPM5();

//    //**************************************************************************
//    // Configure LaunchPad Buttons
//    //**************************************************************************
//    // Set the GPIO pins connected to the pushbuttons as inputs with pull-up resistors
//    GPIO_setAsInputPinWithPullUpResistor(
//            GPIO_PORT_P5,
//            GPIO_PIN6 +                                                         // Pushbutton 1 (S1)
//            GPIO_PIN5                                                           // Pushbutton 2 (S2)
//    );
//
//    // Configure GPIO pins to trigger interrupt on a Low-to-High transition
//    GPIO_selectInterruptEdge (
//            GPIO_PORT_P5,
//            GPIO_PIN6 +
//            GPIO_PIN5,
//            GPIO_LOW_TO_HIGH_TRANSITION
//    );
//
//    // Clear the interrupt flags (P5.x IFG) bits for the push-button pins
//    GPIO_clearInterrupt (
//            GPIO_PORT_P5,
//            GPIO_PIN6 +
//            GPIO_PIN5
//    );
//
//    // Enable the interrupts (P5.x IE) bits for the push-button pins
//    GPIO_enableInterrupt (
//            GPIO_PORT_P5,
//            GPIO_PIN6 +
//            GPIO_PIN5
//    );

    //**************************************************************************
    // Configure Timer Output pin(s)
    //**************************************************************************
    // When running this lab exercise, you will need to pull the J6 jumper and
    // use a jumper wire to connect signal from P1.3 (on boosterpack pinouts) to
    // J6.2 (bottom pin) of LED1 jumper ... this lets the TA1.2 signal drive
    // LED1 directly (without having to use interrupts)
    GPIO_setAsPeripheralModuleFunctionOutputPin( 
            GPIO_PORT_P1, 
            GPIO_PIN3,                                                          // TA1.2
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    //**************************************************************************
    // Configure external crystal pins
    //**************************************************************************
    // Set LFXT (low freq crystal pins) to crystal input (rather than GPIO)
    // Since HFXT is not used, we don't need to set these pins. But for the
    // record, they are:
    //              GPIO_PIN6                                                   // HFXTIN on PJ.6
    //              GPIO_PIN7                                                   // HFXOUT on PJ.7
    GPIO_setAsPeripheralModuleFunctionInputPin(
                    GPIO_PORT_PJ,
                    GPIO_PIN4 +                                                 // LFXIN  on PJ.4
                    GPIO_PIN5 ,                                                 // LFXOUT on PJ.5
                    GPIO_PRIMARY_MODULE_FUNCTION
    );

//    //**************************************************************************
//    // Output MSP clock signals to external pins
//    // - This allows verifying the clocks with a logic analyzer
//    //**************************************************************************
//    //Output the MCLK and SMCLK signals to their respective pins to allow watching them with logic analyzer
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P5,
//                    GPIO_PIN7,                                                // MCLK on P5.7
//                    GPIO_TERNARY_MODULE_FUNCTION
//    );
//
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P3,
//                    GPIO_PIN4,                                                // SMCLK on P3.4
//                    GPIO_SECONDARY_MODULE_FUNCTION
//    );
}


//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
#pragma vector = PORT5_VECTOR
__interrupt void pushbutton_ISR (void)
{
        // Toggle the LED on/off (initial code; moved into switch statement below)
        //GPIO_toggleOutputOnPin( LED1_PORT, LED1_PIN );

    switch( __even_in_range( P5IV, P5IV_P5IFG7 )) {
        case P5IV_NONE:   break;                               // None
        case P5IV_P5IFG0:                                      // Pin 0
             __no_operation();
             break;
       case P5IV_P5IFG1:                                       // Pin 1
            __no_operation();
            break;
       case P5IV_P5IFG2:                                       // Pin 2
            __no_operation();
            break;
       case P5IV_P5IFG3:                                       // Pin 3
            __no_operation();
            break;
       case P5IV_P5IFG4:                                       // Pin 4
            __no_operation();
            break;
       case P5IV_P5IFG5:                                       // Pin 5 (button 2)
            GPIO_toggleOutputOnPin( LED2_PORT, LED2_PIN );
            break;
       case P5IV_P5IFG6:                                       // Pin 6 (button 1)
            GPIO_toggleOutputOnPin( LED1_PORT, LED1_PIN );
            break;
       case P5IV_P5IFG7:                                       // Pin 7
            __no_operation();
            break;
       default:   _never_executed();
    }
}

