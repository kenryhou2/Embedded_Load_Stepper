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
// main.c  (for lab_03b_button project) ('FR6989 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>                                                          // DriverLib include file


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

    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                             // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set P1.1 as a digital input pin
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );            // Switch 1 (S1)

    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    PMM_unlockLPM5();

    while(1) {
        // Read pin P1.1 which is connected to push button 2
        usiButton1 = GPIO_getInputPinValue ( GPIO_PORT_P1, GPIO_PIN1 );

        if ( usiButton1 == GPIO_INPUT_PIN_LOW ) {          
            GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );                 // If button is down, turn on LED
        }
        else {       
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );                  // If button is up, turn off LED
        }
    }
}


