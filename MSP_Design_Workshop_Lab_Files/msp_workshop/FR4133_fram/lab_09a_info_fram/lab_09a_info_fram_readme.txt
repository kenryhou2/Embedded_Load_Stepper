lab_09a_info_fram

This lab uses non-volatile memory to store a data value so that it will be 
available after a power-cycle.

The value will be stored in Info B, which is one of the four segments of
non-volatile memory (NVM) set aside for data information. The 'F5529 uses flash
technology to store non-volatile information, while the 'FR6989, 'FR5969 & 'FR4133 
use FRAM technology.

The code will keep track of how many power-cycles (BOR's) have occurred.
After power up and initializing the GPIO, the code looks for a count value in
NVM, it then increments the count value and:

  - Writes the updated value back to Flash or FRAM
  - Prints out the # of power-cycle counts with printf()
  - Blinks the LED count # of times

To minimize your typing, we have created the project for you. The "hello.c"
file in this project is an amalgam of labs:

  - lab_03a_gpio for the gpio setup
  - lab_04b_wdt  for the printf functionality

To this we've added:
  - Logic to manage the "count" value
  - F5529: Uses function to write to Flash (FRAM doesn't require complex write algorithm)
  - FR4133: Here, we use a function to write to FRAM since this device does not contain
            a full fledged MPU. The FRAM write function turns off the memory protection flag,
	    performs a write, and turns the protection flag back on. While this function 
	    works with the FR5xx/6xx devices, the MPU can be configured to allow full access
	    to portions of FRAM while still leaving others protected.
  - You will need to fill in a few answers from your Lab 9a worksheet

There is no MPU "protection" setup for the 'FR5969/6989 FRAM devices in this exercise. Instead,
this is shown in lab_09b_mpu_gui or lab_09b_mpu_with_driverlib. 
(Once again, note that the F5529 and FR4133 devices don't have an MPU.)


Basic Steps:
- Import project from lab folder
- Fill in the blanks
- ('FR5969 only) Modify setting in linker command file
- Build and launch debugger
- Open memory window
- Step thru code to verify it works
- Restart program
- Step through code again ... hopefully it retained its count value
- Terminate the debugger
- Reset the Launchpad with the reset button ... does the LED blink 1-more-time
  each time its reset or power-cycled?


File source code in this readme file:
- hello.c

Final code ... you can copy from this if you want to save typing time & effort.


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
#pragma DATA_SECTION (count, ".infoA")
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


