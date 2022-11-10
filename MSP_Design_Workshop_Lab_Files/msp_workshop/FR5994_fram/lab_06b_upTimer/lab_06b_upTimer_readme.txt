lab_06b_upTimer

In this timer lab, we switch our code from counting in the "Continuous" mode
to the "Up" mode. This gives us more flexibility to select the frequency for
generating interrupts and output signals.

As we write the ISR code, you should see that TIMER_A has two interupts: 
- One is dedicated to CCR0 (Capture and Compare Register 0).
- The second handles all the other timer interrupts

In this lab, our goal is to create "both" timer interrupts every 1 second.

In our previous lab exercise, we created the group (non-dedicated) 
timer ISR. This lab adds an additional ISR for the dedicated (CCR0 based) 
interrupt. Each of our two ISR's will toggle a different colored LED.

Basic Steps:
- Copy project from lab_06a_timer (your solution or the workshop solution)
- Modify timer configuration function to accomodate "up" mode
- Write an additional Timer ISR for CCR0
- Run and observe the LED flashing

File source code in this readme file:
- main.c
- myGpio.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_06b_upTimer project) ('FR5994 Launchpad)
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
// myTimers.c  (for lab_06b_upTimer project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
// Timer_A configuration parameters for Up mode
Timer_A_initUpModeParam gInitUpParam = {
    TIMER_A_CLOCKSOURCE_ACLK,                                                  // Use ACLK (slower clock)
    TIMER_A_CLOCKSOURCE_DIVIDER_1,                                             // Input clock = ACLK / 1 = 32KHz
    0,                                                                         // Period (set to "0", we'll set actual value later)
    TIMER_A_TAIE_INTERRUPT_ENABLE,                                             // Enable TAR -> 0 interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,                                        // Enable CCR0 compare interrupt
    TIMER_A_DO_CLEAR,                                                          // Clear TAR & clock divider
    false                                                                      // Don't start timer automatically
};


//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA1 in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second using CCR0IFG
    //    Toggle LED2 (Green) on/off every second using TA1IFG
    //    Note: gInitUpParam was created as a global variable. This could have been done as a local variable, too.
	//          As an example, we show configuring the Period dynamically, but this value could have been set globally, as well.
    //*************************************************************************
	gInitUpParam.timerPeriod = 0xFFFF / 2;                                      // Period (0xFFFF/2):  8000 / 32Khz = 1 second
    Timer_A_initUpMode( TIMER_A1_BASE, &gInitUpParam );                         // Set up Timer A1

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
       // This example does not use these features
       // CCR0 is setup by the Timer_A_initUpMode function

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
    Timer_A_clearTimerInterrupt( TIMER_A1_BASE );                                 // Clear TA1IFG
    Timer_A_clearCaptureCompareInterrupt( TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0                                         // Clear CCR0IFG
    );

    //These two enables are already done by the configureUpMode function
    //Timer_A_enableInterrupt( TIMER_A1_BASE );                                   // Enable TA1IFG (TAR rollover to 0)
    //Timer_A_enableCaptureCompareInterrupt( TIMER_A1_BASE,
    //    TIMER_A_CAPTURECOMPARE_REGISTER_0                                       // Enable CCR0IFG
    //);

    Timer_A_startCounter(
        TIMER_A1_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
#pragma vector = TIMER1_A0_VECTOR
__interrupt void ccr0_ISR ( void )
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    // Toggle LED1 on/off
    GPIO_toggleOutputOnPin( LED1_PORT, LED1_PIN );
}

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


