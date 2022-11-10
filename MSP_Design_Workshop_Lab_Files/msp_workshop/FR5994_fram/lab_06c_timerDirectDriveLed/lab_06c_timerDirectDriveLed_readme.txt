lab_06c_timerDirectDriveLed

This lab is a minor adaptation of the code from the previous exercise. The main
difference is that you will connect the GPIO pin to the output of the Timer_A CCR2 
pin (TA1.2).

Unfortunately, the Launchpad does not have an LED connected directly to a
timer output pin, therefore we'll need to use a jumper in order to make the 
proper connection - here's an excerpt from the lab solution:

        // When running this lab exercise, you will need to pull the JP8 jumper and
        // use a jumper wire to connect signal from P1.3 (on boosterpack pinouts) to
        // JP6.2 (bottom pin) of LED1 jumper ... this lets the TA1.2 signal drive 
        // LED1 directly (without having to use interrupts)

The rate that we're toggling the LED was a convenient, if arbitrary, choice.
What rate will we toggle the LED with the timer in Lab6c? We answer that by 
comparing it with the LED toggle rates for our preceeding two labs:

  - Lab6a toggled the LED at a 4 second rate (an interrupt every 2 seconds)
  - Lab6b toggled the LED at a 2 second rate (an interrupt every second)
  - Lab6c will toggle the LED every second (no interrupt needed)


Basic Steps:
- Copy project from lab_06b_upTimer (your solution or the workshop solution)
- Delete (or comment out) GPIO pin setup for P1.0
- Add GPIO setup to connect P1.3 to peripheral module function (i.e. timer)
- In timer setup, add code to configure CCR2 for compare mode
- Turn off CCR0 interrupt (as this isn't needed this for this exercise)
- Set a breakpoint - if you hit it, then something isn't correct
- Run and observe both LED's are (still) flashing


File source code in this readme file:
- main.c
- myGpio.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_06c_timerDirectDriveLed project)  ('FR5994 Launchpad)
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
// myGpio.c  (for lab_06c_timerDirectDriveLed project)  ('FR5994 Launchpad)
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



// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06c_timerDirectDriveLed project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
// Timer_A configuration parameters for Up mode
Timer_A_initUpModeParam gInitUpParam = {
    TIMER_A_CLOCKSOURCE_ACLK,                                                   // Use ACLK (slower clock)
    TIMER_A_CLOCKSOURCE_DIVIDER_1,                                              // Input clock = ACLK / 1 = 32KHz
    0xFFFF / 2,                                                                 // Period (0xFFFF/2):  8000 / 32Khz = 1 second
    TIMER_A_TAIE_INTERRUPT_ENABLE,                                              // Enable TAR -> 0 interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,                                        // Enable CCR0 compare interrupt
    TIMER_A_DO_CLEAR,                                                           // Clear TAR & clock divider
    false                                                                       // Don't start timer automatically
};

// Timer_A configuration for CCR2 compare parameters
Timer_A_initCompareModeParam gInitCcr2Param = {
    TIMER_A_CAPTURECOMPARE_REGISTER_2,                                          // Use CCR2 for compare
    TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,                                   // Since directly driving LED, interrup not req'd
    TIMER_A_OUTPUTMODE_TOGGLE_RESET,                                            // Toggle provides a 1 sec period based on CCR0 and CCR2 values
    0x4000                                                                      // Compare value: 4000 = 1/2 second
};


//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA1 in Up mode using ACLK
    //    Toggle LED1 on/off every second directly from the timers CCR2 output pin (jumper needed)
    //    Toggle LED2 on/off every second using TA1IFG
    //    Note: gInitUpParam was created as a global variable. This could have been done as a local variable, too.
    //          As an example, we show configuring the Period dynamically, but this value could have been set globally, as well.
    //*************************************************************************
    Timer_A_initUpMode( TIMER_A1_BASE, &gInitUpParam );                         // Set up Timer A1

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
    Timer_A_initCompareMode( TIMER_A1_BASE, &gInitCcr2Param );

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
    Timer_A_clearTimerInterrupt( TIMER_A1_BASE );                               // Clear TA1IFG

    Timer_A_startCounter(
        TIMER_A1_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
//#pragma vector = TIMER1_A0_VECTOR
//__interrupt void ccr0_ISR ( void )
//{
//    //**************************************************************************
//    // 4. Timer ISR and vector
//    //**************************************************************************
//    // Toggle LED1 on/off
//    GPIO_toggleOutputOnPin( LED1_PORT, LED1_PIN );
//}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void timer1_ISR ( void )
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    switch( __even_in_range( TA1IV, TA1IV_TAIFG )) {
     case TA1IV_NONE: break;                                                    // (0x00) None
     case TA1IV_TACCR1:                                                         // (0x02) CCR1 IFG
          _no_operation();
           break;
     case TA1IV_TACCR2:                                                         // (0x04) CCR2 IFG
          _no_operation();
           break;
     case TA1IV_3: break;                                                       // (0x06) Reserved
     case TA1IV_4: break;                                                       // (0x08) Reserved
     case TA1IV_5: break;                                                       // (0x0A) Reserved
     case TA1IV_6: break;                                                       // (0x0C) Reserved
     case TA1IV_TAIFG:                                                          // (0x0E) TA1IFG - TAR overflow
          // Toggle LED2 on/off
          GPIO_toggleOutputOnPin( LED2_PORT, LED2_PIN );
          break;
     default: _never_executed();
    }
}


