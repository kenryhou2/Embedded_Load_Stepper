lab_04c_crystals

This lab extends lab_04a_clock by using the crystal oscillators as clock
sources.

General Procedure
- Import & rename lab_04a_clock to lab_04c_crystals
- Delete myClocks.c
- Add myClocksWithCrystals.c
- Modify GPIO setup code
- Debug


File source code in this readme file:
- main.c
- myClocksWithCrystals.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_04c_crystals project)  ('FR4133 Launchpad)
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

    // Set XT1 (low freq crystal pins) to crystal input pins (rather than GPIO):
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P4,
            GPIO_PIN1 +                          // XIN  on P4.1
            GPIO_PIN2 ,                          // XOUT on P4.2
            GPIO_PRIMARY_MODULE_FUNCTION
    );

//    // Output the ACLK and SMCLK signals to their respective pins to allow watching them with logic analyzer
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//            GPIO_PORT_P8,
//            GPIO_PIN0 +                          // SMCLK on P8.0 (Boosterpack pin 6)
//            GPIO_PIN1 ,                          // ACLK  on P8.1 (Boosterpack pin 2)
//            GPIO_PRIMARY_MODULE_FUNCTION
//    );
}



// ----------------------------------------------------------------------------
// myClocksWithCrystals.c  (for lab_04c_crystals project)  ('FR4133 Launchpad)
// ----------------------------------------------------------------------------
// This routine sets up two oscillator sources and configures the MSP clocks.
//
// Oscillators configured:
//   - XT1: external crystal oscillator
//   - DCO: high-freq internal oscillator (runtime calibrated by FLL)
//
// Clocks configured:
//    FLL   = 32KHz    (FLL reference clock input uses XT1 oscillator)
//    ACLK  = 32KHz    (uses XT1 external crystal oscillator)
//    SMCLK =  4MHz    (uses DCO oscillator)
//    MCLK  =  8MHz    (uses DCO oscillator)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
//#include <stdbool.h>
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
// See additional #defines in 'myClocks.h'
#define MCLK_FLLREF_RATIO              MCLK_DESIRED_FREQUENCY_IN_KHZ / ( XT1_CRYSTAL_FREQUENCY_IN_HZ/1024 )
#define XT_TIMEOUT                     50000


//***** Global Variables ******************************************************
uint32_t myACLK  = 0;
uint32_t mySMCLK = 0;
uint32_t myMCLK  = 0;

uint8_t  returnValue = 0;
bool     bReturn     = STATUS_FAIL;


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

    // Initialize the XT1 crystal oscillator (using a timeout in case there is a problem with the crystal)
    // - This requires P4.1 and P4.2 pins to be connected (and configured) as "crystal" pins.
    // - Another alternative is to use the non-timeout function which "hangs" if XT1 isn't configured:
    //    CS_turnOnXT1( CS_XT1_DRIVE_0 );
    // - The "WithTimeout" function used here will always exit, even if XT1 fails to initialize.
    //   You must check to make sure XT1 was initialized properly... in a real application, you would
    //   usually replace the while(1) with a more useful error handling function.
    bReturn = CS_turnOnXT1WithTimeout(
                  CS_XT1_DRIVE_3,
                  XT_TIMEOUT
              );

    if ( bReturn == STATUS_FAIL )
    {
        while( 1 );
    }

    // Set FRAM Controller waitstates to 1 when MCLK > 8MHz (per datasheet)
	// Please refer to the "Non-Volatile Memory" chapter for more details
	//FRAMCtl_configureWaitStateControl( FRAMCTL_ACCESS_TIME_CYCLES_1 );


    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use XT1 as its oscillator source (32KHz)
    CS_initClockSignal(
            CS_ACLK,                                     // Clock you're configuring
            CS_XT1CLK_SELECT,                            // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Set XT1 as the oscillator reference clock for the FLL
    CS_initClockSignal(
            CS_FLLREF,                                   // Clock you're configuring
            CS_XT1CLK_SELECT,                            // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Set MCLK and SMCLK to use the DCO/FLL as their oscillator source (8MHz)
	// The function does a number of things: Calculates required FLL settings; Configures FLL and DCO,
	// and then sets MCLK and SMCLK to use the DCO (with FLL runtime calibration)
    CS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,               // MCLK frequency
            MCLK_FLLREF_RATIO                            // Ratio between MCLK and FLL's reference clock source
    );

    // Verify that the new clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();

    // Modify SMCLK to run at 1/2 the rate of MCLK (4MHz)
    // - On the 'FR4xx, both MCLK and SMCLK have the same clock source
    // - The one variation between these two clocks is that SMCLK has an extra clock divide option
    // - This example shows setting SMCLK at 1/2 the frequency of MCLK
    CS_initClockSignal( 
            CS_SMCLK,                                    // Clock you're configuring
            CS_DCOCLKDIV_SELECT,                         // Clock source
            CS_CLOCK_DIVIDER_2                           // Divide down clock source by this much
    );

    // Verify that the modified clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();
}


