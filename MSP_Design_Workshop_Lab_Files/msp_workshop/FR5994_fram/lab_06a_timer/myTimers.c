// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06a_timer project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
// Timer_A configuration parameters - will toggle LED2 on/off every 2 seconds
// using timer interrupt (TA1IFG)
Timer_A_initContinuousModeParam gInitContParam = {
        ________________________,                                              // Use ACLK (slower clock)
        _____________________________,                                         // Input clock = ACLK / 1 = 32KHz
        _____________________________,                                         // Enable TAR -> 0 interrupt
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

