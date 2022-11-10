// ----------------------------------------------------------------------------
// main.c  (for lab_06d_simplePWM project)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"
#include "myClocks.h"
#include "myTimers.h"

//***** Prototypes ************************************************************


//***** Defines ***************************************************************
#define ONE_SECOND    myMCLK_FREQUENCY_IN_HZ
#define HALF_SECOND   myMCLK_FREQUENCY_IN_HZ / 2


//***** Global Variables ******************************************************
uint16_t period    = 4000;
uint16_t dutyCycle = 2000;


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
    initTimers( period, dutyCycle );

//    __bis_SR_register( GIE );                                                 // Enable interrupts globally

    while(1) {
        __delay_cycles( ONE_SECOND );

        initTimers( period, dutyCycle );
    }
}
