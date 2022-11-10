// ----------------------------------------------------------------------------
// myGpio.c  (for lab_06d_simplePWM project)  ('F5529 Launchpad)
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

    // Set pin P4.7 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN7 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN7 );

    //**************************************************************************
    // Configure LaunchPad Buttons
    //**************************************************************************
//    // Set P2.1 as input with pull-up resistor (for push button S1)
//    //  configure interrupt on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P2, GPIO_PIN1 );
//    GPIO_selectInterruptEdge ( GPIO_PORT_P2, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION );
//    GPIO_clearInterrupt ( GPIO_PORT_P2, GPIO_PIN1 );
//    GPIO_enableInterrupt ( GPIO_PORT_P2, GPIO_PIN1 );

//    // Set P1.1 as input with pull-up resistor (for push button S2)
//    //  configure interrupt on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );
//    GPIO_selectInterruptEdge ( GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION );
//    GPIO_clearInterrupt ( GPIO_PORT_P1, GPIO_PIN1 );
//    GPIO_enableInterrupt ( GPIO_PORT_P1, GPIO_PIN1 );

    //**************************************************************************
    // Configure external crystal pins
    //**************************************************************************
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P5,
            GPIO_PIN5 +                                                         // XOUT on P5.5
            GPIO_PIN4 +                                                         // XIN  on P5.4
            GPIO_PIN3 +                                                         // XT2OUT on P5.3
            GPIO_PIN2                                                           // XT2IN  on P5.2
    );

    // When running this lab exercise, you will need to pull the JP8 jumper and
    // use a jumper wire to connect signal from P1.3 (on boosterpack pinouts) to
    // JP8.2 (bottom pin) of LED1 jumper ... this lets the TA0.2 signal drive
    // LED1 directly (without having to use interrupts)
    GPIO_setAsPeripheralModuleFunctionOutputPin( GPIO_PORT_P1, GPIO_PIN3 );

    //**************************************************************************
    // Output MSP clock signals to external pins
    // - This allows verifying the clocks with a logic analyzer
    //**************************************************************************
//    // Output the ACLK and MCLK signals to their respective pins - which allows you to
//    // watch them with a logic analyzer (ACLK on P1.0, SMCLK on P2.2, MCLK on P7.7)
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//            GPIO_PORT_P1,
//            GPIO_PIN0                                                         // ACLK on P1.0   (Shared with LED1 on jumper JP8)
//    );
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//            GPIO_PORT_P2,
//            GPIO_PIN2                                                         // SMCLK on P2.2  (Boosterpack - Right side (J5) pin 2)
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
//        case P1IV_NONE:   break;                                              // None
//        case P1IV_P1IFG0:                                                     // Pin 0
//             __no_operation();
//             break;
//       case P1IV_P1IFG1:                                                      // Pin 1 (button 2)
//            GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//            break;
//       case P1IV_P1IFG2:                                                      // Pin 2
//            __no_operation();
//            break;
//       case P1IV_P1IFG3:                                                      // Pin 3
//            __no_operation();
//            break;
//       case P1IV_P1IFG4:                                                      // Pin 4
//            __no_operation();
//            break;
//       case P1IV_P1IFG5:                                                      // Pin 5
//            __no_operation();
//            break;
//       case P1IV_P1IFG6:                                                      // Pin 6
//            __no_operation();
//            break;
//       case P1IV_P1IFG7:                                                      // Pin 7
//            __no_operation();
//            break;
//       default:   _never_executed();
//    }
//}

//#pragma vector=PORT2_VECTOR
//__interrupt void pushbutton2_ISR (void)
//{
//    switch( __even_in_range( P2IV, P2IV_P2IFG7 )) {
//        case P2IV_NONE:   break;                                              // None
//        case P2IV_P2IFG0:                                                     // Pin 0
//             __no_operation();
//             break;
//       case P2IV_P2IFG1:                                                      // Pin 1 (button 1)
//            GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN7 );
//            break;
//       case P2IV_P2IFG2:                                                      // Pin 2
//            __no_operation();
//            break;
//       case P2IV_P2IFG3:                                                      // Pin 3
//            __no_operation();
//            break;
//       case P2IV_P2IFG4:                                                      // Pin 4
//            __no_operation();
//            break;
//       case P2IV_P2IFG5:                                                      // Pin 5
//            __no_operation();
//            break;
//       case P2IV_P2IFG6:                                                      // Pin 6
//            __no_operation();
//            break;
//       case P2IV_P2IFG7:                                                      // Pin 7
//            __no_operation();
//            break;
//       default:   _never_executed();
//    }
//}
