lab_06a_timer

Similar to lab_05b_wdtBlink, we want to blink an LED. In this case, 
though, we'll use TIMER_A to generate an interrupt rather than using the 
watchdog interval timer. During the interrupt routine we'll toggle the 
GPIO pin that drives an LED on our Launchpad board.

In lab_06c_timerDirectDriveLed, we will drive the LED directly from the output 
of the Timer. Due to the pinout differences between these devices, we had
to select different timers to use across our various LaunchPads:
         'F5529 Launchpad uses TA0
         'FR4133 Launchpad uses TA1
         'FR5969 Launchpad uses TA1
         'FR6989 Launchpad uses TA1
	 'P401 (MSP432) LaunchPad uses uses TA0

If you compare the two solutions, you should see that the timer setup code
only differs in the function argument that indicates which timer you're using.
In other words, it's very minimal.

As we write the ISR code, you should see that TIMER_A has two interupts: 
- One is dedicated to CCR0 (Capture and Compare Register 0).
- The second handles all the other timer interrupts

This first TIMER_A lab will use the main timer/counter rollover interrupt 
(called TA0IFG or TA1IFG). As with GPIO interrupts, our timer ISR should read 
the Timer IV register (TA0IV or TA1IV) and decipher the correct response using 
a switch/case statement.

The goal is to create a timer interrupt every 2 seconds. If we toggle the LED
inside each ISR, it should take 4 seconds to blink on and off.


Basic Steps:
- Import project from previous lab solution
- Delete old, unneccessary code
- Create the Timer Setup code -- using Continuous mode
- Write the Timer ISR
- Run and observe the LED flashing


File source code in this readme file:
- main.c
- myGpio.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_06a_timer project) ('F5529 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myClocks.h"
#include "myTimers.h"

//***** Prototypes ************************************************************


//***** Defines ***************************************************************


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

    // Initialize timers
    initTimers();

    __bis_SR_register( GIE );                                                   // Enable interrupts globally

    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}



// ----------------------------------------------------------------------------
// myGpio.c ('F5529 Launchpad)
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


// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06a_timer project) ('F5529 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************


//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA0 in Continuous mode using ACLK
    //    Toggle LED2 (Green) on/off every 2 seconds using timer interrupt (TA0IFG)
    //*************************************************************************
    Timer_A_initContinuousModeParam initContParam = { 0 };
        initContParam.clockSource =                 TIMER_A_CLOCKSOURCE_ACLK;       // Use ACLK (slower clock)
        initContParam.clockSourceDivider =          TIMER_A_CLOCKSOURCE_DIVIDER_1;  // Input clock = ACLK / 1 = 32KHz
        initContParam.timerInterruptEnable_TAIE =   TIMER_A_TAIE_INTERRUPT_ENABLE;  // Enable TAR -> 0 interrupt
        initContParam.timerClear =                  TIMER_A_DO_CLEAR;               // Clear TAR & clock divider
        initContParam.startTimer =                  false;                          // Don't start the timer, yet
    Timer_A_initContinuousMode( TIMER_A0_BASE, &initContParam );

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
       // This example does not use these features


    //*************************************************************************
    // 3. Clear/enable interrupt flags and start timer
    //*************************************************************************
    Timer_A_clearTimerInterrupt( TIMER_A0_BASE );                                   // Clear TA0IFG

    //This enable was already done by the configureContinuousMode function
    //Timer_A_enableInterrupt( TIMER_A0_BASE );                                     // Enable interrupt on counter (TAR) rollover

    // Start the timer; make sure you specify the correct counting mode
    Timer_A_startCounter(
        TIMER_A0_BASE,
        TIMER_A_CONTINUOUS_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer0_ISR (void)
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    switch( __even_in_range( TA0IV, TA0IV_TAIFG )) {
     case TA0IV_NONE: break;                 // (0x00) None
     case TA0IV_TACCR1:                      // (0x02) CCR1 IFG
          _no_operation();
           break;
     case TA0IV_TACCR2:                      // (0x04) CCR2 IFG
          _no_operation();
           break;
     case TA0IV_TACCR3:                      // (0x06) CCR3 IFG
          _no_operation();
           break;
     case TA0IV_TACCR4:                      // (0x08) CCR4 IFG
          _no_operation();
           break;
     case TA0IV_5: break;                    // (0x0A) Reserved
     case TA0IV_6: break;                    // (0x0C) Reserved
     case TA0IV_TAIFG:                       // (0x0E) TA0IFG - TAR overflow
          // Toggle LED2 (Green) LED on/off
          GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN7 );
          break;
     default: _never_executed();
    }
}


