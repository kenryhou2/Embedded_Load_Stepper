// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06d_simplePWM project) ('F5529 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************


//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers( uint16_t myPeriod, uint16_t myDutyCycle )
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA0, CCR0, and CCR2 are automatically setup by the Timer_A_outputPWM
    //    function. Depending upon the period/duty-cycle choices, the light will
    //    blink, or be "always on" (to the naked eye) but either bright or dim.
    //    (For dim, try: Period=40, Duty Cycle=2.)
    //*************************************************************************
    Timer_A_outputPWMParam param = { 0 };
        param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;               // Use ACLK (slower clock)
        param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;   // Input clock = ACLK / 1 = 32KHz
        param.timerPeriod = myPeriod;                               // Period
        param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;  // Use CCR2 for compare
        param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;     // Toggling provides a good waveform to watch with LED
        param.dutyCycle = myDutyCycle;                              // Duty cycle (20/40 = 1/2 duty cycle)
    Timer_A_outputPWM( TIMER_A0_BASE, &param );

}

