lab_07b_lpm_timer

This lab shows how you can improve power usage by replacing __delay_cycles()
function calls with a timer - as suggested by ULP Advisor.

It also makes use of the low power modes (LPM3) to let the procesor sleep
while waiting for the timer's interrupt.

General procedure:
- This labs picks up from Lab 7a by looking at the Lab 4c solution
- Turn on ULP Advisor and examine some of its advice
- Set file-specific options to avoid advice on DriverLib
- Import Lab 6b which does basically the same thing as Lab 4c but 
  uses timers instead of __delay_cycles()
- Replace the while{} loop with LPM3

File source code in this readme file:
- main.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_07b_lpm_timer project) ('FR5969 Launchpad)
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

    __low_power_mode_3();
//    __bis_SR_register( GIE );                                                 // Enable interrupts globally
//
//    while(1) {
//        __no_operation();                                                     // Placeholder for while loop (not required)
//    }
}



// ----------------------------------------------------------------------------
// myTimers.c  (for lab_07b_lpm_timer project) ('FR5969 Launchpad)
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
    //    TimerA1 in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second using CCR0IFG
    //    Toggle LED2 (Green) on/off every second using TA1IFG
    Timer_A_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;                       // Use ACLK (slower clock)
        initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;           // Input clock = ACLK / 1 = 32KHz
        initUpParam.timerPeriod = 0xFFFF / 2;                                     // Period (0xFFFF/2):  8000 / 32Khz = 1 second
        initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;    // Enable TAR -> 0 interrupt
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
                TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;                               // Enable CCR0 compare interrupt
        initUpParam.timerClear = TIMER_A_DO_CLEAR;                                // Clear TAR & clock divider
        initUpParam.startTimer = false;                                           // Don't start the timer, yet
    Timer_A_initUpMode( TIMER_A1_BASE, &initUpParam );                            // Set up Timer A1

    // 2. Setup Capture & Compare features
       // This example does not use these features
       // CCR0 is setup by the Timer_A_initUpMode function

    // 3. Clear/enable flags and start timer
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
#pragma vector=TIMER1_A0_VECTOR
__interrupt void ccr0_ISR (void)
{
    // 4. Timer ISR and vector

    // Toggle LED1 on/off
    //GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN6 );
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_ISR (void)
{
    // 4. Timer ISR and vector

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
          // Toggle LED2 (Green) LED on/off
          GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
          break;
     default: _never_executed();
    }
}


