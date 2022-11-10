lab_05a_buttonInterrupt

When Lab 5a is complete, you should be able to push a button to toggle 
the LED on or off.

We begin by importing the solution from Lab 4a. After which we’ll need to 
delete a bit of ‘old’ code and add the following:
  - Setup the interrupt vector
  - Enable interrupts
  - Create an interrupt service routine (ISR)

Basic Steps:
- Import previous project's solution
- Add 'unused_interrupts.c' to project (MSP430 devices only)
- Delete the old LED blink code from while{} loop
- Add code to enable interrupts
- Run code checking interrupt IE and IFG registers,
  as well as seeing the interrupt occur and branch to the
  default ISR (MSP430: unused_interrupts.c; MSP432: msp432_startup_ccs.c)
- Create an ISR and debug it (noticing that the IFG flag is never cleared)
- Go back and 'upgrade' the ISR, using the PORT1 IV register 
- Run and watch it work!


File source code in this readme file:
- main.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_05a_buttonInterrupt project) ('F5529 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>                                                          // DriverLib include file
#include "myClocks.h"


//***** Prototypes ************************************************************
void initGPIO(void);


//***** Defines ***************************************************************
#define   ONE_SECOND    8000000
#define   HALF_SECOND   4000000


//***** Global Variables ******************************************************


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    __bis_SR_register( GIE );                                                   // Enable interrupts globally

    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}

//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {

    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                             // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set pin P4.7 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN7 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN7 );

//    // Set P2.1 as input with pull-up resistor (for push button S1)
//    //  configure interrupt on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P2, GPIO_PIN1 );
//    GPIO_selectInterruptEdge ( GPIO_PORT_P2, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION );
//    GPIO_clearInterrupt ( GPIO_PORT_P2, GPIO_PIN1 );
//    GPIO_enableInterrupt ( GPIO_PORT_P2, GPIO_PIN1 );

    // Set P1.1 as input with pull-up resistor (for push button S2)
    //  configure interrupt on low-to-high transition
    //  and then clear flag and enable the interrupt
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );
    GPIO_selectInterruptEdge ( GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION );
    GPIO_clearInterrupt ( GPIO_PORT_P1, GPIO_PIN1 );
    GPIO_enableInterrupt ( GPIO_PORT_P1, GPIO_PIN1 );

//    // Connect pins to clock crystals
//    GPIO_setAsPeripheralModuleFunctionInputPin(
//            GPIO_PORT_P5,
//            GPIO_PIN5 +                                  // XOUT on P5.5
//            GPIO_PIN4 +                                  // XIN  on P5.4
//            GPIO_PIN3 +                                  // XT2OUT on P5.3
//            GPIO_PIN2                                    // XT2IN  on P5.2
//    );

//    // Output the ACLK and MCLK signals to their respective pins - which allows you to
//    // watch them with a logic analyzer (ACLK on P1.0, SMCLK on P2.2, MCLK on P7.7)
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//            GPIO_PORT_P1,
//            GPIO_PIN0                                    // ACLK on P1.0   (Shared with LED1 on jumper JP8)
//    );
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//            GPIO_PORT_P2,
//            GPIO_PIN2                                    // SMCLK on P2.2  (Boosterpack - Right side (J5) pin 2)
//    );
}


//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
#pragma vector=PORT1_VECTOR
__interrupt void pushbutton_ISR (void)
{
        // Toggle the LED on/off (initial code; moved into switch statement below)
        //GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
        case P1IV_NONE:   break;                               // None
        case P1IV_P1IFG0:                                      // Pin 0
             __no_operation();
             break;
       case P1IV_P1IFG1:                                       // Pin 1 (button 2)
            GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
            break;
       case P1IV_P1IFG2:                                       // Pin 2
            __no_operation();
            break;
       case P1IV_P1IFG3:                                       // Pin 3
            __no_operation();
            break;
       case P1IV_P1IFG4:                                       // Pin 4
            __no_operation();
            break;
       case P1IV_P1IFG5:                                       // Pin 5
            __no_operation();
            break;
       case P1IV_P1IFG6:                                       // Pin 6
            __no_operation();
            break;
       case P1IV_P1IFG7:                                       // Pin 7
            __no_operation();
            break;
       default:   _never_executed();
    }
}

//#pragma vector=PORT2_VECTOR
//__interrupt void pushbutton2_ISR (void)
//{
//    switch( __even_in_range( P2IV, P2IV_P2IFG7 )) {
//        case P2IV_NONE:   break;                               // None
//        case P2IV_P2IFG0:                                      // Pin 0
//             __no_operation();
//             break;
//       case P2IV_P2IFG1:                                       // Pin 1 (button 1)
//            GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN7 );
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
//       case P2IV_P2IFG6:                                       // Pin 6
//            __no_operation();
//            break;
//       case P2IV_P2IFG7:                                       // Pin 7
//            __no_operation();
//            break;
//       default:   _never_executed();
//    }
//}

