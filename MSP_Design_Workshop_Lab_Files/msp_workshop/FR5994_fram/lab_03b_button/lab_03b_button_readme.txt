lab_03b_button

The goal of Lab3b is to light the LED when a button is pushed.

After setting up the two pins we need (one input, one output), the code enters 
an endless while loop where it checks the state of the push button and lights 
the LED if the button is pushed down.

Basic Steps:
- Cut/Paste previous project
- Delete/replace previous while loop
- Single-step code to observe behavior
- Run, to watch it work!

WARNING
Only lighting LED while "polling" the button is very inefficient!
We'll improve on this in the Interrupts lab exercise.

File source code in this readme file:
- main.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_03b_button project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"


//***** Prototypes ************************************************************


//***** Defines ***************************************************************


//***** Global Variables ******************************************************
volatile unsigned short usiButton1 = 0;

//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Initialize GPIO
    initGPIO();

    while(1) {
        // Read Pushbutton, then turn LED on if the Pushbutton is pushed down
        usiButton1 = GPIO_getInputPinValue ( BUTTON1_PORT, BUTTON1_PIN );

        if ( usiButton1 == GPIO_INPUT_PIN_LOW ) {
            GPIO_setOutputHighOnPin( LED1_PORT, LED1_PIN );                     // If button is down, turn on LED
        }
        else {
            GPIO_setOutputLowOnPin( LED1_PORT, LED1_PIN );                      // If button is up, turn off LED
        }
    }
}


