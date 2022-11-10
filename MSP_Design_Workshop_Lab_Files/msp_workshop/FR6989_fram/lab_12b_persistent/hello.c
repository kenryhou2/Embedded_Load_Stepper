// ----------------------------------------------------------------------------
// hello.c  (for lab_12b_persistent project)  ('FR6989 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include <stdio.h>
#include "myGpio.h"
#include "myClocks.h"
#include "myLcd.h"

//***** Prototypes ************************************************************


//***** Defines ***************************************************************
#define ONE_SECOND    myMCLK_FREQUENCY_IN_HZ
#define HALF_SECOND   myMCLK_FREQUENCY_IN_HZ / 2


//***** Global Variables ******************************************************
#pragma PERSISTENT ( count )
uint16_t count = 0;


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    uint16_t i = 0;

    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    // Initialize LCD
    myLCD_init();

    // Read and increment 'count' variable which is located in FRAM INFO segment
    count++;

    // Inform the programmer how many times the board has been power-on reset
    //   which isn't possible without some form of non-volatile memory
    printf( "This program has been power cycled %d times\n", count );

    // Display 'count' on the LCD
    myLCD_displayNumber( (unsigned long) count );

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

