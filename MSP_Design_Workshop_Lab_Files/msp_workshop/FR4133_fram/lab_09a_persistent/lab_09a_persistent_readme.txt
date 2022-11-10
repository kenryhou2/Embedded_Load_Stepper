lab_09a_persistent

Like the previous exercise example (lab_09a_info_fram), this lab uses non-volatile 
memory to store a data value so that it will be available after a power-cycle.

We can indicate that we want the "count" value to be PERSISTENT using the 
PERSISTENT pragma. This tells the tools that it it should be preserved in 
non-volatile memory (NVM) so that its value persists across power-cycles. The
default linker command file stores PERSISTENT data (the ".TI.persistent" section).

Note: For the FR4133, the persistent section is stored in the Main block of  
of FRAM. The advantage here is that Main FRAM is 15KB, while this device's INFOA 
block is only .5KB. Since we're using the main FRAM block, we need to "unprotect" 
it using one of the SYS control bits: PFWP (Program FRAM Write Protect). Unlike our 
previous example, though, disabling this protection leaves our entire program code 
vulnerable to accidental overwrite (because it resides in the same large main FRAM 
block). A better solution is to utilize the FRAM write functions provided in Driver 
Library. These only disable the write protection briefly - only while the actual 
FRAM writes are occuring; otherwise, the write protection flags remain untouched. 
(Note, these same FRAMCtl_writeXX() functions work for both INFO or MAIN FRAM memories.) 

Going back to the remaining code in our lab, just like in the previous example, the 
code  will keep track of how many power-cycles (BOR's) have occurred. After power up 
and initializing the GPIO, the code looks for a count value in NVM, it then increments 
the count value and:

  - Writes the updated value back to Flash or FRAM
  - Prints out the # of power-cycle counts with printf()
  - Blinks the LED count # of times

To minimize your typing, we have created the project for you. The "hello.c"
file in this project is an amalgam of labs:

  - lab_03s_gpio for the gpio setup
  - lab_04b_wdt  for the printf functionality

To this we've added:
  - Logic to manage the "count" value
  - Call the DriverLib function to writes to FRAM
  - You will need to fill in a few answers from your Lab 9a worksheet


Basic Steps:
- Import project from lab folder
- Fill in the blanks
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
// hello.c  (for lab_09a_persistent project)  ('FR4133 Launchpad)
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
// linker command file.
//#pragma DATA_SECTION (count, ".infoA")

// We could have still used INFOA for this exercise, but another alternative is to
// declare the memory location to be PERSISTENT. Looking at the linker command file, we
// can see that ".TI.persistent" is linked into the main FRAM. This block of FRAM is
// controlled by the PFWP flag (rather than the DFWP flag). Rather than unlocking the
// entire block of main FRAM (where our program is stored), it's better to use the
// driverlib FRAMCtl_write functions - as shown in this example.
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
    //SysCtl_enableFRAMWrite( SYSCTL_FRAMWRITEPROTECTION_DATA );                // SYSCFG0 = ~DFWP;

    // Set waitstates to zero so printf() will run without halting
    FRAMCtl_configureWaitStateControl( FRAMCTL_ACCESS_TIME_CYCLES_0 );          // FRCTL0 = FRCTLPW | NWAITS_0;

    // Initialize GPIO
    initGPIO();

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


