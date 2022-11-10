// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06c_timerHal project) 
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"
#include "hal.h"

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
    // 1. Setup Timer (TxR, TxCTL)
    //    TimerA1 in Up mode using ACLK
    //    Toggle LED1 on/off every second directly from the timers CCR2 output pin (jumper needed)
    //    Toggle LED2 on/off every second using TAxIFG
    //    Note: gInitUpParam was created as a global variable. This could have been done as a local variable, too.
    //          As an example, we show configuring the Period dynamically, but this value could have been set globally, as well.
    //*************************************************************************
    Timer_A_initUpMode( TIMER_BASE, &gInitUpParam );                            // Set up Timer

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
    Timer_A_initCompareMode( TIMER_BASE, &gInitCcr2Param );

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
    Timer_A_clearTimerInterrupt( TIMER_BASE );                                  // Clear TAxIFG

    Timer_A_startCounter(
        TIMER_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
#pragma vector = TIMER_VECTOR1
__interrupt void timer_ISR (void)
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    switch( __even_in_range( TIMER_IV, 14 )) {
     case  0: break;                                                            // None
     case  2: break;                                                            // CCR1 IFG
     case  4: break;                                                            // CCR2 IFG
     case  6: break;                                                            // CCR3 IFG
     case  8: break;                                                            // CCR4 IFG
     case 10: break;                                                            // CCR5 IFG
     case 12: break;                                                            // CCR6 IFG
     case 14:                                                                   // TxR overflow
          // Toggle LED2 on/off
          GPIO_toggleOutputOnPin( LED2_PORT, LED2_PIN );
          break;
     default: _never_executed();
    }
}

