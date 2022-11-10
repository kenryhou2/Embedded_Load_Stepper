// ----------------------------------------------------------------------------
// hello.c  (for lab_09b_mpu_with_driverlib project)  ('FR5969 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include <stdio.h>
#include "myMPU.h"
#include "system_isr_routines.h"


//***** Prototypes ************************************************************
void initGPIO(void);


//***** Defines ***************************************************************
#define ONE_SECOND    1000000                                                   // Well, it's about a second
#define HALF_SECOND   1000000 / 2
#define _DEBUG        1              // Set to "0" to turn off Debug statements


//***** Persistent Variables *******************************************************
#pragma PERSISTENT ( count )
static uint16_t count = 0;

#pragma PERSISTENT ( iTest )
int16_t iTest = 0;


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    uint16_t i = 0;

    // This program implements the "_system_pre_init()" function. This function
    // is automatically called by the compiler's init routine (_c_int00).
    // In our implementation of this function, we have done the following:
    //   1. Stopped the Watchdog Timer
    //   2. Checked reason for last reset - Result stored in SYSNMIflag variable
    //   3. Initialized the MPU
    RESET_handleEvent( SYSNMIflag[ SYSNMIflag_RST_EVENT ] );

    if ( _DEBUG )
        printf("I'm in _DEBUG mode!\n");

    // Initialize GPIO
    initGPIO();

    // Cause memory access violations (for testing purposes)
    for ( i = iTest; i <= NUMBER_OF_MEMORY_VIOLATION_TESTS; i++ )
    {
        iTest++;
        violateMemory( i );
    }

    // Read 'count' from non-volatile memory; this also increments count
    count++;

    // Inform the programmer how many times the board has been power-on reset
    //   which isn't possible without some form of non-volatile memory
    if ( _DEBUG ) printf( "\nThis program has been power cycled %d times\n", count );

    // Flash the LED 'count' number of times ... this way you can 'see' how many
    //   times a power-on reset has occurred, even if you can't read the printf()
    for ( i = 0; i < count; i++ )
    {
        // Turn on LED
        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait about a second
        __delay_cycles( ONE_SECOND );

        // Turn off LED
        GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait another second
        __delay_cycles( HALF_SECOND );
    }

    // Trap program here
    while(1);
}

//*****************************************************************************
void initGPIO(void)
{
    // Set pin P1.0 to output direction and turn off LED
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Unlock pins (required for most FRAM devices)
    PMM_unlockLPM5();

    __delay_cycles( HALF_SECOND );

}

