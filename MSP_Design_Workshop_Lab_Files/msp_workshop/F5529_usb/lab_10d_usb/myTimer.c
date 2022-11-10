// ----------------------------------------------------------------------------
// myTimer.c  (for lab_10d_usb project)
// ----------------------------------------------------------------------------
//***** Header Files **********************************************************
#include "myTimer.h"

//***** Defines ***************************************************************
#define TRUE        1
#define FALSE       0

//***** Global Variables ******************************************************
bool bSend = FALSE;

//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    Timer_A_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;                       // Use ACLK (slower clock)
        initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;           // Input clock = ACLK / 1 = 32KHz
        initUpParam.timerPeriod = 0xFFFF / 2;                                     // Period (0xFFFF/2):  8000 / 32Khz = 1 second
        initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;   // Enable TAR -> 0 interrupt
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
                TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;                               // Enable CCR0 compare interrupt
        initUpParam.timerClear = TIMER_A_DO_CLEAR;                                // Clear TAR & clock divider
        initUpParam.startTimer = false;                                           // Don't start the timer, yet
    Timer_A_initUpMode( TIMER_A0_BASE, &initUpParam );                            // Set up Timer A0


    // Clear/enable flags and start timer
    Timer_A_clearTimerInterrupt( TIMER_A0_BASE );                                 // Clear TA0IFG
    Timer_A_clearCaptureCompareInterrupt( TIMER_A0_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0                                         // Clear CCR0IFG
    );

    Timer_A_startCounter(
        TIMER_A0_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
// Timer0 A0 interrupt service routine. Generated when TimerA_0 (real-time
// clock) rolls over from 32768 to 0, every second.
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    bSend = TRUE;                           // Time to update
    __bic_SR_register_on_exit(LPM0_bits);   // Exit LPM
}

