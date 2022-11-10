lab_06b_upTimerB

This is a variation of lab_06b_upTimer. The difference here is that we're now 
using Timer_B rather than Timer_A.

The two timers are VERY similar. There are 4 changes that are needed:

1. Use the TIMER_B API (as opposed to TIMER_A)
2. Select a different timer instance; in this case, you specify TB0 using: TIMER_B0_BASE
3. Use a different interrupt vector:  TIMER0_B0_VECTOR or TIMER0_B1_VECTOR
4. You need to use the TB0 Interrupt Vector Register (TB0IV)

Other than these call out differences, the code remains the same.

Note: The 'FR4133 does not have the TIMER_B peripheral, so there is no 
      solution provided for this device.

File source code in this readme file:
- main.c
- myGpio.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_06b_upTimerB project) ('FR5994 Launchpad)
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
// myTimers.c  (for lab_06b_upTimerB project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
// Timer_B configuration parameters for Up mode
Timer_B_initUpModeParam gInitUpParamB = {
    TIMER_B_CLOCKSOURCE_ACLK,                                                  // Use ACLK (slower clock)
    TIMER_B_CLOCKSOURCE_DIVIDER_1,                                             // Input clock = ACLK / 1 = 32KHz
    0,                                                                         // Period (set to "0", we'll set actual value later)
    TIMER_B_TBIE_INTERRUPT_ENABLE,                                             // Enable TAR -> 0 interrupt
    TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE,                                        // Enable CCR0 compare interrupt
    TIMER_B_DO_CLEAR,                                                          // Clear TAR & clock divider
    false                                                                      // Don't start timer automatically
};


//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TBR, TBCTL)
    //    TimerB0 in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second using CCR0IFG
    //    Toggle LED2 (Green) on/off every second using TB0IFG
    //    Note: gInitUpParam was created as a global variable. This could have been done as a local variable, too.
	//          As an example, we show configuring the Period dynamically, but this value could have been set globally, as well.
    //*************************************************************************
	gInitUpParamB.timerPeriod = 0xFFFF / 2;                                     // Period (0xFFFF/2):  8000 / 32Khz = 1 second
    Timer_B_initUpMode( TIMER_B0_BASE, &gInitUpParamB );                        // Set up Timer B0

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
       // This example does not use these features
       // CCR0 is setup by the Timer_B_initUpMode function

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
    Timer_B_clearTimerInterrupt( TIMER_B0_BASE );                                 // Clear TB0IFG
    Timer_B_clearCaptureCompareInterrupt( TIMER_B0_BASE,
        TIMER_B_CAPTURECOMPARE_REGISTER_0                                         // Clear CCR0IFG
    );

    //These two enables are already done by the configureUpMode function
    //Timer_B_enableInterrupt( TIMER_B0_BASE );                                   // Enable TB0IFG (TBR rollover to 0)
    //Timer_B_enableCaptureCompareInterrupt(TIMER_B0_BASE,
    //    TIMER_B_CAPTURECOMPARE_REGISTER_0                                       // Enable CCR0IFG
    //);

    Timer_B_startCounter(
        TIMER_B0_BASE,
        TIMER_B_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ccr0_ISR ( void )
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    // Toggle LED1 on/off
    GPIO_toggleOutputOnPin( LED1_PORT, LED1_PIN );
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void timer0_ISR (void)
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    switch( __even_in_range( TB0IV, TB0IV_TBIFG )) {
        case TB0IV_NONE:   break;                               // (0x00) No Interrupt pending 
        case TB0IV_TBCCR1:                                      // (0x02) CCR1 IFG
             __no_operation();
             break;
       case TB0IV_TBCCR2:                                       // (0x04) CCR2 IFG
            __no_operation();
            break;
       case TB0IV_TBCCR3:                                       // (0x06) CCR3 IFG
            __no_operation();
            break;
       case TB0IV_TBCCR4:                                       // (0x08) CCR4 IFG
            __no_operation();
            break;
       case TB0IV_TBCCR5:                                       // (0x0A) CCR5 IFG
            __no_operation();
            break;
       case TB0IV_TBCCR6:                                       // (0x0C) CCR6 IFG
            __no_operation();
            break;
       case TB0IV_TBIFG:                                        // (0x0E) TB0IFG - TBR overflow
            // Toggle LED2 (Green) LED on/off
            GPIO_toggleOutputOnPin( LED2_PORT, LED2_PIN );
            break;
       default:   _never_executed();
    }
}


