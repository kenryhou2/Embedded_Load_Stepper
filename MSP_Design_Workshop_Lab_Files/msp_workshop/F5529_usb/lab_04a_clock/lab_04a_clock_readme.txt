lab_04a_clock

This lab explores system initialization, of which, setting up the clocks are a
big part. After adding code to configure the clocks, we'll run it and verify
that we're seeing the expected clock rates.

An optional step has us reconfigure MCLK to use a slower oscillator, which 
makes our LED blink very, very slowly.

General procedure:
- Import lab_03a_gpio
- Create myClocks.c to hold our clock initialization code
- Reorganize the system init code in main() to fit our chapter's 'template'
- Debug the code using Watch Expressions to view the various clock rates
  of our MSP processor
- Change MCLK to run slowly ... and wait very patiently for the LED to
  turn on & off

File source code in this readme file:
- main.c
- myClocks.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_04a_clock project)  ('F5529 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myClocks.h"


//***** Prototypes ************************************************************
void initGPIO(void);


//***** Defines ***************************************************************
#define   ONE_SECOND    8000000
#define   HALF_SECOND   4000000


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

    while(1) {
        // Turn on LED
        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait about a second
        __delay_cycles( ONE_SECOND );

        // Turn off LED
        GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait another second
        __delay_cycles( HALF_SECOND );
    }
}

//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {

    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                             // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set pin P4.7 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN7 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN7 );
}

// ----------------------------------------------------------------------------
// myClocks.c  (for lab_04a_clock project)  ('F5529 Launchpad)
//
// This routine sets ACLK to run from REFO, then configures MCLK and SMCLK to
// run from the and high-freq internal clock source (DCO).
//
// Oscillators:
//    DCO    =   8MHz  (default is ~1MHz) Internal high-frequency oscillator
//    REFO   =  32KHz                     Internal 32KHz reference oscillator
//    MODOSC =   5MHz                     Internal 5MHz oscillator
//    VLO    = ~10KHz                     Internal very low power, low frequency oscillator
//    XT1    =  --KHz  (not configured)   External crystal input
//    XT2    =  --MHz  (not configured)   External crystal input
//
// Reference Clock:
//    FLL    = REFO   =  32KHz            Internal reference clock; used for calibrating DCO at runtime

// Internal Clocks:
//    ACLK  =  REFO   =  32KHz
//    SMCLK =  DCO    =   8MHz
//    MCLK  =  DCO    =   8MHz
//    MODCLK = MODOSC =   5MHz  (default)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
//#include <stdbool.h>
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
#define LF_CRYSTAL_FREQUENCY_IN_HZ     32768                                    // 32KHz
#define HF_CRYSTAL_FREQUENCY_IN_HZ     4000000                                  // 4MHz

#define MCLK_DESIRED_FREQUENCY_IN_KHZ  8000                                     // 8MHz
#define MCLK_FLLREF_RATIO              MCLK_DESIRED_FREQUENCY_IN_KHZ / ( UCS_REFOCLK_FREQUENCY / 1024 )    // Ratio = 250


//***** Global Variables ******************************************************
uint32_t myACLK  = 0;
uint32_t mySMCLK = 0;
uint32_t myMCLK  = 0;


//***** initClocks ************************************************************
void initClocks(void) {

    //**************************************************************************
    // Configure core voltage level
    //**************************************************************************
     // Set core voltage level to handle 8MHz clock rate
     PMM_setVCore( PMM_CORE_LEVEL_1 );


    //**************************************************************************
    // Configure Oscillators
    //**************************************************************************
    // Set the XT1/XT2 crystal frequencies used on the LaunchPad, and connected
	// to the clock pins, so that driverlib knows how fast they are (these are 
    // needed for the DriverLib clock 'get' and crystal start functions)
    UCS_setExternalClockSource(
            LF_CRYSTAL_FREQUENCY_IN_HZ,                                         // XT1CLK input
            HF_CRYSTAL_FREQUENCY_IN_HZ                                          // XT2CLK input
    );

    // Verify if the default clock settings are as expected
    myACLK  = UCS_getACLK();
    mySMCLK = UCS_getSMCLK();
    myMCLK  = UCS_getMCLK();


    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use REFO as its oscillator source (32KHz)
    UCS_initClockSignal(
            UCS_ACLK,                                    // Clock you're configuring
            UCS_REFOCLK_SELECT,                          // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Set REFO as the oscillator reference clock for the FLL
    UCS_initClockSignal(
            UCS_FLLREF,                                  // Clock you're configuring
            UCS_REFOCLK_SELECT,                          // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Set MCLK and SMCLK to use the DCO/FLL as their oscillator source (8MHz)
	// The function does a number of things: Calculates required FLL settings; Configures FLL and DCO,
	// and then sets MCLK and SMCLK to use the DCO (with FLL runtime calibration)
    UCS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,               // MCLK frequency
            MCLK_FLLREF_RATIO                            // Ratio between MCLK and FLL's reference clock source
    );

//    // Optional lab step set MCLK to run from REFO
//    // This will make the LED blink very sloooowly in our while{} loop
//    UCS_initClockSignal( UCS_BASE,
//            UCS_MCLK,                                    // Clock you're configuring
//            UCS_REFOCLK_SELECT,                          // Clock source
//            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
//    );

    // Verify that the modified clock settings are as expected
    myACLK  = UCS_getACLK();
    mySMCLK = UCS_getSMCLK();
    myMCLK  = UCS_getMCLK();
}


