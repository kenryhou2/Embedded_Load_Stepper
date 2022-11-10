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
#define LF_CRYSTAL_FREQUENCY_IN_HZ     ______
#define HF_CRYSTAL_FREQUENCY_IN_HZ     ______

#define MCLK_DESIRED_FREQUENCY_IN_KHZ  ______
#define MCLK_FLLREF_RATIO              ______ / ( UCS_REFOCLK_FREQUENCY / 1024 )    // Ratio = 250


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
     PMM_setVCore( ________________________ );


    //**************************************************************************
    // Configure Oscillators
    //**************************************************************************
    // Set the XT1/XT2 crystal frequencies used on the LaunchPad, and connected
	// to the clock pins, so that driverlib knows how fast they are (these are 
    // needed for the DriverLib clock 'get' and crystal start functions)
    _______________________(
            _______________________,                                            // XT1CLK input
            _______________________                                             // XT2CLK input
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
            ___________________,                         // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Set REFO as the oscillator reference clock for the FLL
    UCS_initClockSignal(
            UCS_FLLREF,                                  // Clock you're configuring
            ___________________,                         // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Set MCLK and SMCLK to use the DCO/FLL as their oscillator source (8MHz)
	// The function does a number of things: Calculates required FLL settings; Configures FLL and DCO,
	// and then sets MCLK and SMCLK to use the DCO (with FLL runtime calibration)
    UCS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,               // MCLK frequency
            ___________________                          // Ratio between MCLK and FLL's reference clock source
    );

    // Verify that the modified clock settings are as expected
    myACLK  = UCS_getACLK();
    mySMCLK = UCS_getSMCLK();
    myMCLK  = UCS_getMCLK();
}

