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
// main.c  (for lab_06b_upTimerB project) ('FR5969 Launchpad)
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
// myGpio.c  ('FR5969 Launchpad)
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
    // Set pin P4.6 to output direction, then turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN6 );                             // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN6 );

    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    //**************************************************************************
    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    //**************************************************************************
    PMM_unlockLPM5();

    //**************************************************************************
    // Configure LaunchPad Buttons
    //**************************************************************************
//    // Set P4.5 as input with pull-up resistor (for push button S1)
//    //  configure interrupt on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P4, GPIO_PIN5 );
//    GPIO_selectInterruptEdge ( GPIO_PORT_P4, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION );
//    GPIO_clearInterrupt ( GPIO_PORT_P4, GPIO_PIN5 );
//    GPIO_enableInterrupt ( GPIO_PORT_P4, GPIO_PIN5 );

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
    // Set LFXT (low freq crystal pins) to crystal input (rather than GPIO)
    // Since HFXT is not used, we don't need to set these pins. But for the 
    // record, they are:
    //              GPIO_PIN6                            // HFXTIN on PJ.6
    //              GPIO_PIN7                            // HFXOUT on PJ.7
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_PJ,
            GPIO_PIN4 +                                  // LFXIN  on PJ.4
            GPIO_PIN5 ,                                  // LFXOUT on PJ.5
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    //**************************************************************************
    // Output MSP clock signals to external pins
    // - This allows verifying the clocks with a logic analyzer
    //**************************************************************************
    // Output the ACLK and SMCLK signals to their respective pins - which allows you to
    // watch them with a logic analyzer (ACLK on P2.0, SMCLK on P3.4)
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P2,                        // Unfortunately, P2.0 is not pinned out to FR5969 Boosterpack pin
//                    GPIO_PIN0,                           // ACLK on P2.0
//                    GPIO_TERNARY_MODULE_FUNCTION
//    );
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P3,
//                    GPIO_PIN4,                           // SMCLK on P3.4
//                    GPIO_TERNARY_MODULE_FUNCTION         // Could use Secondary or Tertiary mode
//    );
}


//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
//#pragma vector=PORT1_VECTOR
//__interrupt void pushbutton_ISR (void)
//{
//    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
//        case P1IV_NONE:   break;                               // None
//        case P1IV_P1IFG0:                                      // Pin 0
//             __no_operation();
//             break;
//       case P1IV_P1IFG1:                                       // Pin 1 (button 2)
//            GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//            break;
//       case P1IV_P1IFG2:                                       // Pin 2
//            __no_operation();
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

//#pragma vector=PORT4_VECTOR
//__interrupt void pushbutton1_ISR (void)
//{
//    switch( __even_in_range( P4IV, P4IV_P4IFG7 )) {
//        case P4IV_NONE:   break;                               // None
//        case P4IV_P4IFG0:                                      // Pin 0
//             __no_operation();
//             break;
//       case P4IV_P4IFG1:                                       // Pin 1
//           __no_operation();
//            break;
//       case P4IV_P4IFG2:                                       // Pin 2
//            __no_operation();
//            break;
//       case P4IV_P4IFG3:                                       // Pin 3
//            __no_operation();
//            break;
//       case P4IV_P4IFG4:                                       // Pin 4
//            __no_operation();
//            break;
//       case P4IV_P4IFG5:                                       // Pin 5 (button 1)
//           GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN6 );
//            break;
//       case P4IV_P4IFG6:                                       // Pin 6
//            __no_operation();
//            break;
//       case P4IV_P4IFG7:                                       // Pin 7
//            __no_operation();
//            break;
//       default:   _never_executed();
//    }
//}


// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06b_upTimerB project) ('FR5969 Launchpad)
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
    // 1. Setup Timer (TAR, TACTL)
    //    TimerB0 in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second using CCR0IFG
    //    Toggle LED2 (Green) on/off every second using TB0IFG
    Timer_B_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_B_CLOCKSOURCE_ACLK;                       // Use ACLK (slower clock)
        initUpParam.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;           // Input clock = ACLK / 1 = 32KHz
        initUpParam.timerPeriod = 0xFFFF / 2;                                     // Period (0xFFFF/2):  8000 / 32Khz = 1 second
        initUpParam.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_ENABLE;    // Enable TBR -> 0 interrupt
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
                TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;                               // Enable CCR0 compare interrupt
        initUpParam.timerClear = TIMER_B_DO_CLEAR;                                // Clear TBR & clock divider
        initUpParam.startTimer = false;                                           // Don't start the timer, yet
    Timer_B_initUpMode( TIMER_B0_BASE, &initUpParam );                            // Set up Timer B0

    // 2. Setup Capture & Compare features
       // This example does not use these features
       // CCR0 is setup by the Timer_B_initUpMode function

    // 3. Clear/enable flags and start timer
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
#pragma vector=TIMER0_B0_VECTOR
__interrupt void ccr0_ISR (void)
{
    // 4. Timer ISR and vector

    // Toggle LED1 on/off
    GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN6 );
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void timer0_ISR (void)
{
    // 4. Timer ISR and vector

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
            GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
            break;
       default:   _never_executed();
    }
}


