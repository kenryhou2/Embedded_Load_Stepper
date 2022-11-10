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
// main.c  (for lab_03b_button project) ('FR4133 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>


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

    // Set switch 1 (S1) as input button (connected to P1.2)
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN2 );

    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    PMM_unlockLPM5();

    while(1) {
        // Read pin P1.2 which is connected to push button 1
        usiButton1 = GPIO_getInputPinValue ( GPIO_PORT_P1, GPIO_PIN2 );

        if ( usiButton1 == GPIO_INPUT_PIN_LOW ) {
            // If button is down, turn on LED
            GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );
        }
        else {
            // If button is up, turn off LED
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
        }
    }
}


