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
#define XT1_CRYSTAL_FREQUENCY_IN_HZ    ______
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
	_______________________(  
	        _______________________,
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
            ___________________,                         // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Set the FLL's reference clock source as REFO
    CS_initClockSignal(
            CS_FLLREF,                                   // Clock you're configuring
            ___________________,                         // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Configure the FLL's frequency and set MCLK & SMCLK to use the FLL as their source (8MHz)
    CS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,               // MCLK frequency
            ___________________                          // Ratio between MCLK and FLL's reference clock source
    );

    // Verify that the modified clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();
}

