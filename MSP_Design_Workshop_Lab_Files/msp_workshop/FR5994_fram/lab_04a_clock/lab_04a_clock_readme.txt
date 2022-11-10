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
// main.c  (for lab_04a_clock project) ('FR5994 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myClocks.h"


//***** Prototypes ************************************************************


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

    while(1) {
        // Turn on LED
        GPIO_setOutputHighOnPin( LED1_PORT, LED1_PIN );

        // Wait about a second
        __delay_cycles( ONE_SECOND );

        // Turn off LED
        GPIO_setOutputLowOnPin( LED1_PORT, LED1_PIN );

        // Wait another second
        __delay_cycles( HALF_SECOND );
    }
}

// ----------------------------------------------------------------------------
// myClocks.c  (for lab_04a_clock project)  ('FR5994 Launchpad)
//
// This routine sets ACLK to run from VLO, then configures MCLK and SMCLK to
// run from the and high-freq internal clock source (DCO).
//    ACLK  = ~10KHz
//    SMCLK =  8MHz
//    MCLK  =  8MHz
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
//#include <stdbool.h>
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
#define LF_CRYSTAL_FREQUENCY_IN_HZ     32768
#define HF_CRYSTAL_FREQUENCY_IN_HZ     0                                        // FR5994 Launchpad does not ship with HF Crystal populated


//***** Global Variables ******************************************************
uint32_t myACLK  = 0;
uint32_t mySMCLK = 0;
uint32_t myMCLK  = 0;


//***** initClocks ************************************************************
void initClocks(void) {

    //**************************************************************************
    // Configure Oscillators
    //**************************************************************************
    // Set the crystal frequencies attached to the LFXT and HFXT oscillator pins
	// so that driverlib knows how fast they are (needed for the clock 'get' functions)
    CS_setExternalClockSource(
            LF_CRYSTAL_FREQUENCY_IN_HZ,
            HF_CRYSTAL_FREQUENCY_IN_HZ
    );

    // Verify if the default clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();

    // Set FRAM Controller waitstates to 1 when MCLK > 8MHz (per datasheet)
	// Please refer to the "Non-Volatile Memory" chapter for more details
	//FRAMCtl_configureWaitStateControl( FRAMCTL_ACCESS_TIME_CYCLES_1 );

    // Set DCO to run at 8MHz
    CS_setDCOFreq(
            CS_DCORSEL_1,                                                       // Set Frequency range (DCOR)
            CS_DCOFSEL_3                                                        // Set Frequency (DCOF)
    );

    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use VLO as its oscillator source (~10KHz)
    CS_initClockSignal(
            CS_ACLK,                                                            // Clock you're configuring
            CS_VLOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Set SMCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 8MHz)
    CS_initClockSignal(
            CS_SMCLK,                                                           // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Set MCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 8MHz)
    CS_initClockSignal(
            CS_MCLK,                                                            // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

//    // Set the MCLK to use the VLO clock (used for one of the optional exercises)
//    CS_initClockSignal(
//            CS_MCLK,                                     // Clock you're configuring
//            CS_VLOCLK_SELECT,                            // Clock source
//            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
//    );

    // Verify that the modified clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();
}


