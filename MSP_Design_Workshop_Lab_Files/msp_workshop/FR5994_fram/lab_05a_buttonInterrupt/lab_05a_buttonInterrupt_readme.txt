lab_05a_buttonInterrupt

When Lab 5a is complete, you should be able to push a button to toggle 
the LED on or off.

We begin by importing the solution from Lab 4a. After which we’ll need to 
delete a bit of ‘old’ code and add the following:
  - Setup the interrupt vector
  - Enable interrupts
  - Create an interrupt service routine (ISR)

Basic Steps:
- Import previous project's solution
- Add 'unused_interrupts.c' to project (MSP430 devices only)
- Delete the old LED blink code from while{} loop
- Add code to enable interrupts
- Run code checking interrupt IE and IFG registers,
  as well as seeing the interrupt occur and branch to the
  default ISR (MSP430: unused_interrupts.c; MSP432: msp432_startup_ccs.c)
- Create an ISR and debug it (noticing that the IFG flag is never cleared)
- Go back and 'upgrade' the ISR, using the PORT1 IV register 
- Run and watch it work!


File source code in this readme file:
- main.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_05a_buttonInterrupt project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myClocks.h"


//***** Prototypes ************************************************************
void initGPIO(void);


//***** Defines ***************************************************************
#define ONE_SECOND  8000000
#define HALF_SECOND 4000000


//***** Global Variables ******************************************************


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

    __bis_SR_register( GIE );                                                   // Enable interrupts globally

    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}

