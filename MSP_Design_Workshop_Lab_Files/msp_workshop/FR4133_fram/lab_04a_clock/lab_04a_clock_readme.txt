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
// main.c  (for lab_04a_clock project) ('FR4133 Launchpad)
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

    // Set pin P4.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN0 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN0 );

    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    PMM_unlockLPM5();

//    // Output the ACLK and SMCLK signals to their respective pins to allow watching them with logic analyzer
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//            GPIO_PORT_P8,
//            GPIO_PIN0 +                          // SMCLK on P8.0 (Boosterpack pin 6)
//            GPIO_PIN1 ,                          // ACLK  on P8.1 (Boosterpack pin 2)
//            GPIO_PRIMARY_MODULE_FUNCTION
//    );

}

// ----------------------------------------------------------------------------
// myClocks.c  (for lab_04a_clock project)  ('FR4133 Launchpad)
//
// This routine sets ACLK to run from REFO, then configures MCLK and SMCLK to
// run from the high-freq internal clock source (DCO).
//
// Oscillators configured:
//   - DCO: high-freq internal oscillator (runtime calibrated by FLL)
//
// Clocks configured:
//    FLL   = 32KHz    (FLL reference clock input uses REFO oscillator)
//    ACLK  = 32KHz    (uses REFO internal oscillator)
//    SMCLK =  8MHz    (uses DCO oscillator)
//    MCLK  =  8MHz    (uses DCO oscillator)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
//#include <stdbool.h>
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
#define XT1_CRYSTAL_FREQUENCY_IN_HZ    32768
#define REFOCLK_FREQUENCY              32768
#define ONE_SECOND                     800000

#define MCLK_DESIRED_FREQUENCY_IN_KHZ  8000                                     // 8 * 1khz
#define MCLK_FLLREF_RATIO              MCLK_DESIRED_FREQUENCY_IN_KHZ / ( REFOCLK_FREQUENCY/1024 )


//***** Global Variables ******************************************************
uint32_t myACLK  = 0;
uint32_t mySMCLK = 0;
uint32_t myMCLK  = 0;


//***** initClocks ************************************************************
void initClocks(void) {

    //**************************************************************************
    // Configure Oscillators
    //**************************************************************************
    // Set the XT1 crystal frequency being used so that driverlib
    //   knows how fast it is (needed for the clock 'get' and XT1 start functions)
    CS_setExternalClockSource(
            XT1_CRYSTAL_FREQUENCY_IN_HZ
    );

    // Verify if the default clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();

    // Set FRAM Controller waitstates to 1 when MCLK > 8MHz (per datasheet)
	// Please refer to the "Non-Volatile Memory" chapter for more details
	//FRAMCtl_configureWaitStateControl( FRAMCTL_ACCESS_TIME_CYCLES_1 );


    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use REFO as its oscillator source (32KHz)
    CS_initClockSignal(
            CS_ACLK,                                     // Clock you're configuring
            CS_REFOCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Set the FLL's reference clock source as REFO
    CS_initClockSignal(
            CS_FLLREF,                                   // Clock you're configuring
            CS_REFOCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Configure the FLL's frequency and set MCLK & SMCLK to use the FLL as their source (8MHz)
    CS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,               // MCLK frequency
            MCLK_FLLREF_RATIO                            // Ratio between MCLK and FLL's reference clock source
    );

    // Verify that the modified clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();
}


