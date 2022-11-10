// ----------------------------------------------------------------------------
// myGpio.c  ('FR4133 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"


//***** Defines ***************************************************************


//***** Global Variables ******************************************************


//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {

    //**************************************************************************
	// Configure LaunchPad LEDs
    //**************************************************************************
    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                             // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set pin P4.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN0 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN0 );

    //**************************************************************************
    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    //**************************************************************************
    PMM_unlockLPM5();

    //**************************************************************************
    // Configure LaunchPad Buttons
    //**************************************************************************
//    // Set P1.2 as input with pull-up resistor (for push button S1)
//    //  configure interrupt on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN2 );
//    GPIO_selectInterruptEdge ( GPIO_PORT_P1, GPIO_PIN2, GPIO_LOW_TO_HIGH_TRANSITION );
//    GPIO_clearInterrupt ( GPIO_PORT_P1, GPIO_PIN2 );
//    GPIO_enableInterrupt ( GPIO_PORT_P1, GPIO_PIN2 );

//    // Set P2.6 as input with pull-up resistor (for push button S2)
//    //  configure interrupt on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P2, GPIO_PIN6 );
//    GPIO_selectInterruptEdge ( GPIO_PORT_P2, GPIO_PIN6, GPIO_LOW_TO_HIGH_TRANSITION );
//    GPIO_clearInterrupt ( GPIO_PORT_P2, GPIO_PIN6 );
//    GPIO_enableInterrupt ( GPIO_PORT_P2, GPIO_PIN6 );

    //**************************************************************************
    // Configure Timer Output pin
    //**************************************************************************
    // When running this lab exercise, you will need to pull the JP1 jumper and
    // use a jumper wire to connect signal from P8.3 (on boosterpack pins) to
    // JP1.2 (bottom pin) of LED1 jumper ... this lets the TA1.2 signal drive
    // LED1 directly (without having to use interrupts)
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P8,
            GPIO_PIN3,                                                          // TA1.2 on P8.3 (Boosterpack pin 10)
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    //**************************************************************************
    // Configure external crystal pins
    //**************************************************************************
    // Set LFXT (low freq crystal pins) to crystal input (rather than GPIO)
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P4,
            GPIO_PIN1 +                                                         // XIN  on P4.1
            GPIO_PIN2 ,                                                         // XOUT on P4.2
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    //**************************************************************************
    // Output MSP clock signals to external pins
    // - This allows verifying the clocks with a logic analyzer
    //**************************************************************************
    // Output the ACLK and SMCLK signals to their respective pins to allow watching them with logic analyzer
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//            GPIO_PORT_P8,
//            GPIO_PIN0 +                                                       // SMCLK on P8.0 (Boosterpack pin 6)
//            GPIO_PIN1 ,                                                       // ACLK  on P8.1 (Boosterpack pin 2)
//            GPIO_PRIMARY_MODULE_FUNCTION
//    );
}


//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
//#pragma vector=PORT1_VECTOR
//__interrupt void pushbutton_ISR (void)
//{
//        // Toggle the LED on/off (initial code; moved into switch statement below)
//        //GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//
//    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
//        case P1IV_NONE:   break;                               // None
//        case P1IV_P1IFG0:                                      // Pin 0
//             __no_operation();
//             break;
//       case P1IV_P1IFG1:                                       // Pin 1
//            __no_operation();
//            break;
//       case P1IV_P1IFG2:                                       // Pin 2 (button 1)
//            GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//            break;
//       case P1IV_P1IFG3:                                       // Pin 3
//            __no_operation();
//            break;
//       case P1IV_P1IFG4:                                       // Pin 4
//            __no_operation();
//            break;
//       case P1IV_P1IFG5:                                       // Pin 5
//            __no_operation();
//            break;
//       case P1IV_P1IFG6:                                       // Pin 6
//            __no_operation();
//            break;
//       case P1IV_P1IFG7:                                       // Pin 7
//            __no_operation();
//            break;
//       default:   _never_executed();
//    }
//}

//#pragma vector=PORT2_VECTOR
//__interrupt void pushbutton2_ISR (void)
//{
//    switch( __even_in_range( P2IV, P2IV_P2IFG7 )) {
//        case P2IV_NONE:   break;                               // None
//        case P2IV_P2IFG0:                                      // Pin 0
//             __no_operation();
//             break;
//       case P2IV_P2IFG1:                                       // Pin 1
//           __no_operation();
//            break;
//       case P2IV_P2IFG2:                                       // Pin 2
//            __no_operation();
//            break;
//       case P2IV_P2IFG3:                                       // Pin 3
//            __no_operation();
//            break;
//       case P2IV_P2IFG4:                                       // Pin 4
//            __no_operation();
//            break;
//       case P2IV_P2IFG5:                                       // Pin 5
//            __no_operation();
//            break;
//       case P2IV_P2IFG6:                                       // Pin 6 (button 2)
//           GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN0 );
//            break;
//       case P2IV_P2IFG7:                                       // Pin 7
//            __no_operation();
//            break;
//       default:   _never_executed();
//    }
//}
