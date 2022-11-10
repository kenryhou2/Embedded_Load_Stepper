// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06a_timer project) ('FR5969 Launchpad)
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
    //    TimerA1 in Continuous mode using ACLK
    //    Toggle LED2 (Green) on/off every 2 seconds using timer interrupt (TA1IFG)
    //*************************************************************************
    Timer_A_initContinuousModeParam initContParam = { 0 };
        initContParam.clockSource =                 _____________________________;  // Use ACLK (slower clock)
        initContParam.clockSourceDivider =          _____________________________;  // Input clock = ACLK / 1 = 32KHz
        initContParam.timerInterruptEnable_TAIE =   _____________________________;  // Enable TAR -> 0 interrupt
        initContParam.timerClear =                  TIMER_A_DO_CLEAR;               // Clear TAR & clock divider
        initContParam.startTimer =                  false;                          // Don't start the timer, yet
    Timer_A_initContinuousMode( TIMER_A1_BASE, &initContParam );

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
    //    This example does not use these features

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
    Timer_A________________________( TIMER_A1_BASE );                               // Clear TA1IFG

    //This enable was already done by the configureContinuousMode function
    //Timer_A_enableInterrupt( TIMER_A1_BASE );                                     // Enable interrupt on counter (TAR) rollover

    Timer_A_startCounter(
        TIMER_A1_BASE,
        TIMER_A_ _______________
    );
}

//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_ISR (void)
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
          // Toggle LED2 (Green) LED on/off
          GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
          break;
     default: _never_executed();
    }
}

