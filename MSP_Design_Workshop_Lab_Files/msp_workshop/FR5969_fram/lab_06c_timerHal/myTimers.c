// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06c_timerHal project) 
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"
#include "hal.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************


//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second directly from the timers CCR2 output pin
    //    Toggle LED2 (Green) on/off every second using TAxIFG
    Timer_A_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;                       // Use ACLK (slower clock)
        initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;           // Input clock = ACLK / 1 = 32KHz
        initUpParam.timerPeriod = 0xFFFF / 2;                                     // Period (0xFFFF/2):  8000 / 32Khz = 1 second
        initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;    // Enable TAR -> 0 interrupt
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
            TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;                                  // No CCR0 interrupt
        initUpParam.timerClear = TIMER_A_DO_CLEAR;                                // Clear TAR & clock divider
        initUpParam.startTimer = false;                                           // Don't start the timer, yet
    Timer_A_initUpMode( TIMER_BASE, &initUpParam );

    // 2. Setup Capture & Compare features
    Timer_A_initCompareModeParam initCcr2Param = { 0 };
        initCcr2Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;        // Use CCR2 for compare
        initCcr2Param.compareInterruptEnable =
            TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;                             // Since directly driving LED, interrup not req'd
        initCcr2Param.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_RESET;        // Toggle provides a 1 sec period based on CCR0 and CCR2 values
        initCcr2Param.compareValue = 0x4000;                                      // Compare value: 4000 = 1/2 second
    Timer_A_initCompareMode( TIMER_BASE, &initCcr2Param );

    // 3. Clear/enable flags and start timer
    Timer_A_clearTimerInterrupt( TIMER_BASE );                                    // Clear TAxIFG

    Timer_A_startCounter(
        TIMER_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
#pragma vector=TIMER_VECTOR1
__interrupt void timer_ISR (void)
{
    // 4. Timer ISR and vector

    switch( __even_in_range( TIMER_IV, 14 )) {
     case  0: break;                // None
     case  2: break;                // CCR1 IFG
     case  4: break;                // CCR2 IFG
     case  6: break;                // CCR3 IFG
     case  8: break;                // CCR4 IFG
     case 10: break;                // CCR5 IFG
     case 12: break;                // CCR6 IFG
     case 14:                       // TxR overflow
              // Toggle LED2 (Green) LED on/off
              GPIO_toggleOutputOnPin( LED2_PORT, LED2_PIN );
              break;
     default: _never_executed();
    }
}

