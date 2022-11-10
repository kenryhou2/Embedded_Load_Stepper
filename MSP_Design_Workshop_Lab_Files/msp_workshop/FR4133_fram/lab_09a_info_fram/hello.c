// ----------------------------------------------------------------------------
// hello.c  (for lab_09a_info project)  ('FR4133 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include <stdio.h>


//***** Prototypes ************************************************************
void initGPIO(void);


//***** Defines ***************************************************************
#define ONE_SECOND    1000000                                                   // Well, it's about a second
#define HALF_SECOND   1000000 / 2


//***** Global Variables ******************************************************
// For the other processors we arbitrarily chose to use INFOB for this lab exercise.
// The 'FR4x family only has one info block, which is called .infoA by the default
// linker command file. (INFOA is linked to 0x1800)
#pragma ______________ ( ____________ , "___________" )
uint16_t count;


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    uint16_t i = 0;

    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Enable writes to "data" FRAM (i.e. FRAM INFO memory blocks)
    SysCtl_enableFRAMWrite( SYSCTL_FRAMWRITEPROTECTION_DATA );                  // SYSCFG0 = ~DFWP;

    // Set waitstates to zero so printf() will run without halting
    FRAMCtl_configureWaitStateControl( FRAMCTL_ACCESS_TIME_CYCLES_0 );          // FRCTL0 = FRCTLPW | NWAITS_0;

    // Initialize GPIO
    initGPIO();

    // Read and increment 'count' variable which is located in FRAM INFO segment
    count++;

    // Inform the programmer how many times the board has been power-on reset
    //   which isn't possible without some form of non-volatile memory
    printf( "This program has been power cycled %d times\n", count );

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

