// ----------------------------------------------------------------------------
// myClocks.c  ('F5529 Launchpad)
// ----------------------------------------------------------------------------
// This routine sets up two oscillator sources and configures the MSP internal 
// clocks. Additionally, it sets the core voltage to handle the 8MHz clock rate.
//
// Oscillators:
//    DCO    =   8MHz  (default is ~1MHz) Internal high-frequency oscillator
//    REFO   =  32KHz                     Internal 32KHz reference oscillator
//    MODOSC =   5MHz                     Internal 5MHz oscillator
//    VLO    = ~10KHz                     Internal very low power, low frequency oscillator
//    XT1    =  32KHz                     External crystal input
//    XT2    =  --MHz  (not configured)   External crystal input
//
// Reference Clock:
//    FLL    = REFO   =  32KHz            Internal reference clock; used for calibrating DCO at runtime

// Internal Clocks:
//    ACLK   = REFO   =  32KHz
//    SMCLK  = DCO    =   8MHz
//    MCLK   = DCO    =   8MHz
//    MODCLK = MODOSC =   5MHz  (default)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
// See additional #defines in 'myClocks.h'
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

    // Initialize XT1 crystal oscillator without a timeout
	// -- In case of failure the code remains 'stuck' in this function
    // -- To have the function timeout, rather than getting stuck, use the
	//    function:   UCS_turnOnLFXT1WithTimeout(); 
	//    see an example of this in lab_04c_crystals
    // -- This function requires P5.4 and P5.5 pins to be connected (and 
	//    configured) as clock input pins (otherwise it will fail)
    UCS_turnOnLFXT1( UCS_XT1_DRIVE_0, UCS_XCAP_3 );


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

    // Configure the DCO's frequency using the FLL as its reference; then set MCLK & SMCLK to use this clock source (8MHz)
    UCS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,               // MCLK frequency
            MCLK_FLLREF_RATIO                            // Ratio between MCLK and FLL's reference clock source
    );

    // Verify that the modified clock settings are as expected
    myACLK  = UCS_getACLK();
    mySMCLK = UCS_getSMCLK();
    myMCLK  = UCS_getMCLK();
}

