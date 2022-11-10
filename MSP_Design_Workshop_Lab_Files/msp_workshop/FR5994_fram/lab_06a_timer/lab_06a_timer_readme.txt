lab_06a_timer

Similar to lab_05b_wdtBlink, we want to blink an LED. In this case, 
though, we'll use TIMER_A to generate an interrupt rather than using the 
watchdog interval timer. During the interrupt routine we'll toggle the 
GPIO pin that drives an LED on our Launchpad board.

Because the Boosterpack pinout differs between the various LaunchPads 
(which affects Lab6c), we had to choose different timers for someo of them:
         'F5529 Launchpad uses TA0
         'FR4133 Launchpad uses TA1
         'FR5994 Launchpad uses TA1
         'FR6989 Launchpad uses TA1
	 'P401 (MSP432) LaunchPad uses uses TA1

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
// main.c  (for lab_06a_timer project) ('FR5994 Launchpad)
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
// myGpio.c  ('FR5994 Launchpad)
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
    // Discussed in Timer chapter

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
#pragma vector=PORT5_VECTOR
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



// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06a_timer project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
// Timer_A configuration parameters - will toggle LED2 on/off every 2 seconds
// using timer interrupt (TA1IFG)
Timer_A_initContinuousModeParam gInitContParam = {
        TIMER_A_CLOCKSOURCE_ACLK,                                              // Use ACLK (slower clock)
        TIMER_A_CLOCKSOURCE_DIVIDER_1,                                         // Input clock = ACLK / 1 = 32KHz
        TIMER_A_TAIE_INTERRUPT_ENABLE,                                         // Enable TAR -> 0 interrupt
        TIMER_A_DO_CLEAR,                                                      // Clear TAR & clock divider
        false                                                                  // Don't start timer automatically
};

//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA1 in Continuous mode using ACLK
    //*************************************************************************
    Timer_A_initContinuousMode( TIMER_A1_BASE, &gInitContParam );

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
    //    This example does not use these features

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
    Timer_A_clearTimerInterrupt( TIMER_A1_BASE );                                   // Clear TA1IFG

    //This enable was already done by the configureContinuousMode function
    //Timer_A_enableInterrupt( TIMER_A1_BASE );                                     // Enable interrupt on counter (TAR) rollover

    Timer_A_startCounter(
        TIMER_A1_BASE,
        TIMER_A_CONTINUOUS_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
#pragma vector = TIMER1_A1_VECTOR
__interrupt void timer1_ISR ( void )
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    switch( __even_in_range( TA1IV, TA1IV_TAIFG )) {
     case TA1IV_NONE: break;                 // (0x00) None
     case TA1IV_TACCR1:                      // (0x02) CCR1 IFG
          _no_operation();
           break;
     case TA1IV_TACCR2:                      // (0x04) CCR2 IFG
          _no_operation();
           break;
     case TA1IV_3: break;                    // (0x06) Reserved
     case TA1IV_4: break;                    // (0x08) Reserved
     case TA1IV_5: break;                    // (0x0A) Reserved
     case TA1IV_6: break;                    // (0x0C) Reserved
     case TA1IV_TAIFG:                       // (0x0E) TA1IFG - TAR overflow
          // Toggle LED2 on/off
          GPIO_toggleOutputOnPin( LED2_PORT, LED2_PIN );
          break;
     default: _never_executed();
    }
}


