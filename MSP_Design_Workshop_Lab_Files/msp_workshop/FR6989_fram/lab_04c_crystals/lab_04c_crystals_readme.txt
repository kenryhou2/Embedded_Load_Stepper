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
// main.c  (for lab_04c_crystals project)  ('FR6989 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>                                                          // DriverLib include file
#include "myClocks.h"


//***** Prototypes ************************************************************
void initGPIO(void);


//***** Defines ***************************************************************
#define ONE_SECOND  4000000
#define HALF_SECOND 2000000


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

    // Set pin P9.7 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN7 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN7 );

    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    PMM_unlockLPM5();

    // Set LFXT (low freq crystal pins) to crystal input (rather than GPIO)
    // Since HFXT is not used, we don't need to set these pins. But for the 
    // record, they are:
    //              GPIO_PIN6                            // HFXTIN on PJ.6
    //              GPIO_PIN7                            // HFXOUT on PJ.7
    GPIO_setAsPeripheralModuleFunctionInputPin(
                    GPIO_PORT_PJ,
                    GPIO_PIN4 +                          // LFXIN  on PJ.4
                    GPIO_PIN5 ,                          // LFXOUT on PJ.5
                    GPIO_PRIMARY_MODULE_FUNCTION
    );

//    // Output the MCLK, ACLK signals to their respective pins - which allows you to
//    // watch them with a logic analyzer (MCLK on P4.0, ACLK on P4.1).
//    // Note, all three clocks (MCLK, SMCLK, ACLK) are each available on 4 different I/O pins; though,
//    // we're only showing these two because they are the only ones routed to BoosterPack pins
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P4,                         
//                    GPIO_PIN0 +                          // MCLK on P4.0
//                    GPIO_PIN1 ,                          // ACLK on P4.1
//                    GPIO_TERNARY_MODULE_FUNCTION
//    );
}


// ----------------------------------------------------------------------------
// myClocksWithCrystals.c  (for lab_04c_crystals project)  ('FR6989 Launchpad)
// ----------------------------------------------------------------------------
// This routine sets up the Low Frequency crystal (LFXT) and high-freq
// internal clock source (DCO). Then configures ACLK, SMCLK, and MCLK:
//
// Oscillators:
//    LFXT   =  32KHz                     External crystal input
//    DCO    =   4MHz  (default is 1MHz)  Internal high-frequency oscillator
//
// Internal Clocks:
//    ACLK  = 32KHz    (uses LFXT external oscillator)
//    SMCLK =  2MHz    (uses DCO internal oscillator)
//    MCLK  =  4MHz    (uses DCO internal oscillator)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdbool.h>
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
#define LF_CRYSTAL_FREQUENCY_IN_HZ     32768
#define HF_CRYSTAL_FREQUENCY_IN_HZ     0                                        // FR6989 Launchpad does not ship with HF Crystal populated

#define XT_TIMEOUT                     100000


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

    // Initialize the LFXT crystal oscillator (using a timeout in case there is a problem with the crystal)
    // - This requires PJ.4 and PJ.5 pins to be connected (and configured) as "crystal" pins.
    // - Another alternative is to use the non-timeout function which "hangs" if LFXT isn't configured:
    //    CS_turnOnLFXT( CS_LFXT_DRIVE_0 );
    // - The "WithTimeout" function used here will always exit, even if LFXT fails to initialize.
    //   You must check to make sure LFXT was initialized properly... in a real application, you would
    //   usually replace the while(1) with a more useful error handling function.
    bReturn = CS_turnOnLFXTWithTimeout(
                  CS_LFXT_DRIVE_0,
                  XT_TIMEOUT
              );

    if ( bReturn == STATUS_FAIL )
    {
        while( 1 );
    }


//    // Initialize HFXT. Returns STATUS_SUCCESS if initializes successfully.
//    // The FR6989 Launchpad does not ship with HF crystal, so we've left it commented-out.
//     bReturn = CS_turnOnHFXTWithTimeout( 
//                   CS_HFXTDRIVE_4MHZ_8MHZ,
//                   XT_TIMEOUT
//               );
//
//     if ( bReturn == STATUS_FAIL )
//     {
//         while( 1 );
//     }

    // Set FRAM Controller waitstates to 1 when MCLK > 8MHz (per datasheet)
	// Please refer to the "Non-Volatile Memory" chapter for more details
	//FRAMCtl_configureWaitStateControl( FRAMCTL_ACCESS_TIME_CYCLES_1 );

    // Set DCO to run at 4MHz
    CS_setDCOFreq(
            CS_DCORSEL_0,                                                       // Set Frequency range (DCOR)
            CS_DCOFSEL_3                                                        // Set Frequency (DCOF)
    );

    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use LFXT as its oscillator source (32KHz)
    // With a 32KHz crystal and a divide by 1, ACLK should run at that rate
    CS_initClockSignal(
            CS_ACLK,                                                            // Clock you're configuring
            CS_LFXTCLK_SELECT,                                                  // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Set SMCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 4MHz)
    // With DCO set at 4MHz, SMCLK is set for half that rate (2MHz)
    CS_initClockSignal(
            CS_SMCLK,                                                           // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_2                                                  // Divide down clock source by this much
    );

    // Set MCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 4MHz)
    CS_initClockSignal(
            CS_MCLK,                                                            // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Verify that the modified clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();
}


