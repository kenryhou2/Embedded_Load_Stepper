lab_12b_persistent_readme

This lab extends and combines two previous lab exercises:  

- lab_09a_persistent
- lab_12a_heart

If you remember, lab09a kept track of the number of times 
a board was reset, using FRAM to store the persistent count data.
Each time the board was reset, the count value was printed to the
consol and the LED was blinked (that many times).

In this lab, by using the Launchpad's LCD, we add one more way to
display the 'count' value.


// ----------------------------------------------------------------------------
// hello.c  (for lab_12b_persistent project)  ('FR4133 Launchpad)
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
// For the other processors we arbitrarily chose to use INFOB for this lab exercise.
// The 'FR4x family only has one info block, which is called .infoA in the default
// linker command file.
//#pragma DATA_SECTION (count, ".infoA")

// We could have still used INFOA for this exercise, but another alternative is to
// declare the memory location to be PERSISTENT. Looking at the linker command file, we
// can see that ".TI.persistent" is linked into the main FRAM. This block of FRAM is
// controlled by the PFWP flag (rather than the DFWP flag). Rather than unlocking the
// entire block of main FRAM (where our program is stored), it's better to use the
// driverlib FRAMCtl_write functions - as shown later in this example.
#pragma PERSISTENT ( count )
uint16_t count = 0;
uint16_t temp  = 0;


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    uint16_t i = 0;

    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Enable writes to "data" FRAM (i.e. FRAM INFO memory blocks)
    // In this example, we'll leave the write protection "on" and let the FFRAMCTL_write function handle disabling memory protection
    //SysCtl_enableFRAMWrite( SYSCTL_FRAMWRITEPROTECTION_DATA );               // SYSCFG0 = ~DFWP;

    // Set waitstates to zero so printf() will run without halting
    FRAMCtl_configureWaitStateControl( FRAMCTL_ACCESS_TIME_CYCLES_0 );         // FRCTL0 = FRCTLPW | NWAITS_0;

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    // Initialize LCD
    myLCD_init();

    // Read 'count' and add 1 to it, storing the temporary result to the 'temp' variable
    temp = count + 1;
	
    // Write the value of temp back to the persistent 'count' variable which is located in FRAM
    FRAMCtl_write16(
            &temp,                    // 'from' address of data to be written
            &count,                   // 'to' address where data should go
            1                         // How many elements to move (e.g. 1 x 16-bits)
    );

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


