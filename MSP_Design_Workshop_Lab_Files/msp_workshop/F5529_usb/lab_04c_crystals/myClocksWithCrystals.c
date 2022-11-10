// ----------------------------------------------------------------------------
// myClocksWithCrystals.c  (for lab_04c_crystals project) ('F5529 Launchpad)
//
// This routine sets up the external Low Frequency crystal (XT1), high-freq 
// crystal (XT2), and high-freq internal oscillator (DCO), which is based upon 
// the Freqency Locked Loop (FLL). It then configures the internal clocks.
//
// Oscillators:
//    DCO    =   8MHz  (default is ~1MHz) Internal high-frequency oscillator
//    REFO   =  32KHz                     Internal 32KHz reference oscillator
//    MODOSC =   5MHz                     Internal 5MHz oscillator
//    VLO    = ~10KHz                     Internal very low power, low frequency oscillator
//    XT1    =  32KHz                     External crystal input
//    XT2    =   4MHz                     External crystal input
//
// Reference Clock:
//    FLL    = XT1    =  32KHz            Internal reference clock; used for calibrating DCO at runtime

// Internal Clocks:
//    ACLK   = XT1    =  32KHz
//    SMCLK  = XT2    =   4MHz
//    MCLK   = DCO    =   8MHz
//    MODCLK = MODOSC =   5MHz  (default)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdbool.h>
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
#define LF_CRYSTAL_FREQUENCY_IN_HZ     32768                                    // 32KHz
#define HF_CRYSTAL_FREQUENCY_IN_HZ     4000000                                  // 4MHz

#define MCLK_DESIRED_FREQUENCY_IN_KHZ  8000                                     // 8MHz
#define MCLK_FLLREF_RATIO              MCLK_DESIRED_FREQUENCY_IN_KHZ / ( UCS_REFOCLK_FREQUENCY / 1024 )    // Ratio = 250

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

    // Initialize the XT1 crystal oscillator (using a timeout in case there is a problem with the crystal)
    // - This requires P5.4 and P5.5 pins to be connected (and configured) as clock input pins.
    // - Another alternative is to use the non-timeout function which "hangs" if XT1 isn't configured;
    //    UCS_turnOnXT1( CS_XT1_DRIVE_0, UCS_XCAP_3 );   (in fact, we used the non-timeout function to setup XT2)
    // - The "WithTimeout" function used here will always exit, even if XT1 fails to initialize.
    //   You must check to make sure XT1 was initialized properly... in a real application, you would
    //   usually replace the while(1) with a more useful error handling function.
    bReturn = UCS_turnOnLFXT1WithTimeout(
                  UCS_XT1_DRIVE_0,
                  UCS_XCAP_3,
                  XT_TIMEOUT
              );

    if ( bReturn == STATUS_FAIL )
    {
        while( 1 );
    }

    // Initializes the XT2 crystal oscillator with no timeout.
    // In case of failure, code hangs here.
    // For time-out instead of code hang use UCS_turnOnXT2WithTimeout().
    UCS_turnOnXT2( UCS_XT2_DRIVE_4MHZ_8MHZ );

//    This is an example of turning on XT2 with the the timeout option.
//    bReturn = UCS_turnOnXT2WithTimeout(
//                  UCS XT2 DRIVE 4MHZ 8MHZ,
//                  XT2_TIMEOUT
//              );
//
//    if ( bReturn == STATUS_FAIL )
//    {
//        while( 1 );
//    }


    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use XT1 as its oscillator source (32KHz)
    UCS_initClockSignal(
            UCS_ACLK,                                    // Clock you're configuring
            UCS_XT1CLK_SELECT,                           // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Select XT2 as oscillator source for SMCLK
    UCS_initClockSignal(
            UCS_SMCLK,                                   // Clock you're configuring
            UCS_XT2CLK_SELECT,                           // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Set REFO as the oscillator reference clock for the FLL
    UCS_initClockSignal(
            UCS_FLLREF,                                  // Clock you're configuring
            UCS_XT1CLK_SELECT,                           // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Set MCLK and SMCLK to use the DCO/FLL as their oscillator source (8MHz)
	// The function does a number of things: Calculates required FLL settings; Configures FLL and DCO,
	// and then sets MCLK and SMCLK to use the DCO (with FLL runtime calibration)
    UCS_initFLLSettle(
            MCLK_DESIRED_FREQUENCY_IN_KHZ,               // MCLK frequency
            MCLK_FLLREF_RATIO                            // Ratio between MCLK and FLL's reference clock source
    );

    // Verify that the modified clock settings are as expected
    myACLK  = UCS_getACLK();
    mySMCLK = UCS_getSMCLK();
    myMCLK  = UCS_getMCLK();

    // Select XT2 as SMCLK source
    // We have to Re-do this call ... Why? If you use UCS_initFLLSettle() to 
    //   setup MCLK, it also configures SMCLK; therefore, you should call this 
	//   function after setting up MCLK
    UCS_initClockSignal(
            UCS_SMCLK,
            UCS_XT2CLK_SELECT,
            UCS_CLOCK_DIVIDER_1
    );

    // Verify that the modified clock settings are as expected
    myACLK  = UCS_getACLK();
    mySMCLK = UCS_getSMCLK();
    myMCLK  = UCS_getMCLK();
}

