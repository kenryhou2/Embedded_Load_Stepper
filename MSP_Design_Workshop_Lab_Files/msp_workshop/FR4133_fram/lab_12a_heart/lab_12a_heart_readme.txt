lab_12a_heart

The FR4133 & FR6989 Launchpads have a LCD which contains a number of segments:
-  Six 14-segment characters (commonly used to show numbers or letters)
-  What we call Punctuation for the six characters (negative, periods, and colon symbols)
-  Various other Symbols (Heart, Timer, RX/TX, Battery meter, etc.)

As the title of the lab exercise suggests, our main goal is to enable the "Heart" symbol 
on the LCD. This involves initializing the display, as well as setting the LCD memory
registers to appropriately drive the correct COM/SEG lines.

Additionally, you'll explore the dual-memory and blinking features of the FR4xx and FR6xx
LCD controllers.

Basic Steps:
- Copy project from lab_06b_upTimer (we suggest using the workshop provided solution)
- Delete the "myTimer" C and header files
- Import the "myLCD" C and header files -- and edit appropriately
- Call the initLCD() function from main()
- In main(), write the code needed to display the "heart" symbol
- Single-step through the code to evaluate the various blinking methodologies

File source code in this readme file:
- main.c
- myLcd.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_12a_heart project) ('FR4133 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myClocks.h"
#include "myLcd.h"

//***** Prototypes ************************************************************


//***** Defines ***************************************************************


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

    // Initialize LCD
    myLCD_init();

// -------------------------------------------------------------------------------------
// Single-step through this code to see how each line affects the LCD
// -------------------------------------------------------------------------------------

    // Set the LCD's "Heart" symbol
    LCD_E_setMemory( LCD_E_BASE,
            LCD_E_MEMORY_BLINKINGMEMORY_12,                                          // LCDMx location
            LCD_HEART_COM                                                            // "LCD_" symbols are defined in the 'myLcd.h' header file
    );

    // Set the LCD's "Timer" symbol
    LCD_E_setMemory( LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER_COM );    // Heart replaced by Timer symbol

    // Updates only the specified segments (in this case, adding the Heart segment)
    LCD_E_updateMemory( LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_HEART_COM ); // Both Heart and Timer symbols should be displayed

    // Clears only the specified segments (in this case, the Timer segment)
    LCD_E_clearMemory( LCD_E_BASE, LCD_TIMER_IDX, LCD_TIMER_COM );                   // Erases just the Timer symbol (this time, we used our #define for the LCD memory index)

    // Toggles only the specified segments (in this case, the Heart segment)
    LCD_E_toggleMemory( LCD_E_BASE, LCD_HEART_IDX, LCD_HEART_COM );                  // Turns off the Heart symbol, so the display should now be blank (this time, we used our #define for the LCD memory index)

    // Set both the "Heart" and "Timer" symbols
    LCD_E_setMemory( LCD_E_BASE,
                     LCD_E_MEMORY_BLINKINGMEMORY_12,
                     LCD_HEART_COM | LCD_TIMER_COM );                                // Turn on both at once, since both are controlled by LCD Memory 12

// Let's explore the Blinking features

    // Blinks enabled segments on the display (in this case, the Heart and Timer).
    // Notice how the blinking continues, even when the processor is halted at a breakpoint (or during single-stepping).
    // This is because the LCD controller is automatically handling the blinking -- no timers or interrupts are required!
    LCD_E_setBlinkingControl( LCD_E_BASE,
            LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64,
            LCD_E_BLINK_MODE_INDIVIDUAL_SEGMENTS
    );

    // Turn off the Timer symbol
    LCD_E_clearMemory( LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER_COM );  // Erases just the Timer symbol

    // Turns off the blinking feature
    LCD_E_setBlinkingControl ( LCD_E_BASE,
            LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64,
            LCD_E_BLINK_MODE_DISABLED
    );

// Using the 2nd memory (i.e. Blinking Memory)

    // Set the LCD's "Timer" segment in 2nd memory (i.e. Blinking Memory)
    // Notice that the Timer doesn't "show up" after executing this function, since we're currently displaying the main LCD memory
    LCD_E_setBlinkingMemory( LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER_COM );  // Nothing appears to happen...

    // Tell the LCD to display the 2nd (Blinking) memory
    LCD_E_selectDisplayMemory( LCD_E_BASE, LCD_E_DISPLAYSOURCE_BLINKINGMEMORY );     // Timer symbol "on"; Heart symbol "off"

    // Select to display main LCD (main) memory, again
    LCD_E_selectDisplayMemory( LCD_E_BASE, LCD_E_DISPLAYSOURCE_MEMORY );             // Heart is "on"; Timer symbol "off"

    // Tell the LCD controller to switch back-and-forth between memories
    LCD_E_setBlinkingControl( LCD_E_BASE,
            LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64,
            LCD_E_BLINK_MODE_SWITCHING_BETWEEN_DISPLAY_CONTENTS                      // You should see the Heart and Timer alternating
    );

    // Turn off Blinking
    LCD_E_setBlinkingControl( LCD_E_BASE,
            LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64,
            LCD_E_BLINK_MODE_DISABLED
    );

    // As a final example, turn on the Exclamation symbol
    myLCD_showSymbol( LCD_UPDATE, LCD_EXCLAMATION, LCD_MEMORY_MAIN );


// -------------------------------------------------------------------------------------
// End of Single-stepping through code
// -------------------------------------------------------------------------------------

    // Write "DONE" to LCD
    // myShowChar:  Param 1 is letter to display; Param 2 is the character's location
    myLCD_showChar( 'D', 2 );
    myLCD_showChar( 'O', 3 );
    myLCD_showChar( 'N', 4 );
    myLCD_showChar( 'E', 5 );


    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}



// ----------------------------------------------------------------------------
// myLCD.c  ('FR4133 LaunchPad)
// ----------------------------------------------------------------------------

// ***** Header Files *********************************************************
#include "myLcd.h"
#include "string.h"
#include "driverlib.h"


// ***** Prototypes ***********************************************************
// See additional prototypes in the 'myLCD.h' file
void TEST_myLCD_showSymbol( void );


// ***** Global Variables******************************************************
// Memory locations for LCD characters
const unsigned int location[ LCD_NUM_CHAR ] =
{
       4,                                                                       // Position 1 = Digit A1
       6,                                                                       // Position 2 = Digit A2
       8,                                                                       // Position 3 = Digit A3
       10,                                                                      // Position 4 = Digit A4
       2,                                                                       // Position 5 = Digit A5
       18                                                                       // Position 6 = Digit A6
};

// LCD memory map for numeric digits
const char digit[10][2] =
{
    {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};

// LCD memory map for uppercase letters
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

// This structure is defined by the LCD_E DriverLib module, and is passed to the LCD_E_init() function
LCD_E_initParam initParams = {
    LCD_E_CLOCKSOURCE_XTCLK,
    LCD_E_CLOCKDIVIDER_3,
    LCD_E_4_MUX,
    LCD_E_STANDARD_WAVEFORMS,
    LCD_E_SEGMENTS_ENABLED
};


//*****************************************************************************
// myLCD_init()
//
// Configures the LCD display controller, and then clears the display and
// enables it
//*****************************************************************************
void myLCD_init()
{
    // Turn off LCD
    LCD_E_off( LCD_E_BASE );

    // L0~L26 & L36~L39 pins selected for use with LCD
    // Note: LCD pin configurations override any GPIO settings for these pins
    LCD_E_setPinAsLCDFunctionEx( LCD_E_BASE,         // Gives range of pins to LCD controller
            LCD_E_SEGMENT_LINE_0,                    // Starting Lx pin of range
            LCD_E_SEGMENT_LINE_26                    // Ending pin of range
    );

    LCD_E_setPinAsLCDFunctionEx( LCD_E_BASE,         // Gives range of pins to LCD controller
            LCD_E_SEGMENT_LINE_36,                   // Starting Lx pin of range
            LCD_E_SEGMENT_LINE_39                    // Ending pin of range
    );

    // Init LCD as 4-mux mode
    LCD_E_init( LCD_E_BASE, &initParams );

    // LCD Operation - Mode 3, internal 3.02v, charge pump 256Hz
    LCD_E_setVLCDSource( LCD_E_BASE, LCD_E_INTERNAL_REFERENCE_VOLTAGE, LCD_E_EXTERNAL_SUPPLY_VOLTAGE );
    LCD_E_setVLCDVoltage( LCD_E_BASE, LCD_E_REFERENCE_VOLTAGE_2_96V );

    LCD_E_enableChargePump( LCD_E_BASE );
    LCD_E_setChargePumpFreq( LCD_E_BASE, LCD_E_CHARGEPUMP_FREQ_16 );

    // Clear LCD memory
    LCD_E_clearAllMemory( LCD_E_BASE );
    LCD_E_clearAllBlinkingMemory( LCD_E_BASE );

    // Configure COMs and SEGs
    // L0 = COM0, L1 = COM1, L2 = COM2, L3 = COM3
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_MEMORY_COM0 );
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_1, LCD_E_MEMORY_COM1 );
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_2, LCD_E_MEMORY_COM2 );
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_3, LCD_E_MEMORY_COM3 );

    // Select to display main LCD memory
    LCD_E_selectDisplayMemory( LCD_E_BASE, LCD_E_DISPLAYSOURCE_MEMORY );

    // Turn blinking features off
    LCD_E_setBlinkingControl( LCD_E_BASE, LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64, LCD_E_BLINK_MODE_DISABLED );

    // Turn on LCD
    LCD_E_on( LCD_E_BASE );

// ----------------------------------------------------------------------------
// Test Routines
// ----------------------------------------------------------------------------
    //Runs the routine which tests all the options for the myLCD_showSymbol() function
    //TEST_myLCD_showSymbol();

    //// Test showChar function
    //myLCD_showChar( 'A', 1 );
    //myLCD_showChar( 'B', 2 );
    //myLCD_showChar( 'C', 3 );
    //myLCD_showChar( 'D', 4 );
    //myLCD_showChar( 'E', 5 );
    //myLCD_showChar( 'F', 6 );                                                 // Display should show "ABCDEF"

	//// Different tests for myLCD_displayString function
	myLCD_displayString( "ABCDEF" );											// Use string as function param
	//{
	//	char *t = "123456";
	//	myLCD_displayString( t );                                               // String declared as char *
	//}
	//{
	//	char t2[6] = "MNOPQR";
	//	myLCD_displayString( t2 );                                              // String declared as an array
	//}
	//{
	//	char t2[8] = "MNOPQRXX";
	//	myLCD_displayString( t2 );                                              // Test for string longer than 6 characters
	//
	//	t2[2] = '\0';
	//	t2[4] = '\0';
	//	myLCD_displayString( t2 );                                              // Test for more than one end-of-string char
	//}
	//{
	//	myLCD_displayString( "ABCDEF" );
	//	myLCD_displayString( "123" );                                           // Test string less than 6 chars
	//}


    //// Test displayNumber function
    //myLCD_displayNumber( 123456 );                                            // Display should show "123456"
    //myLCD_displayNumber( 1111111 );                                           // Display should show " ERROR"
}


//*****************************************************************************
// myLCD_showChar()
//
// Displays input character at given LCD digit/position
// Only spaces, numeric digits, and uppercase letters are accepted characters
//
// This function has two arguments and does not return a value.
//
// Arg 1: "c" is a string char that you want displayed on the LCD glass.
//        Currently, only A-Z and 0-9 and a space " " are supported. You can
//        clear any character position by using the space.
// Arg 2: "Position" tells the function where you want the character to be
//        placed. With six character positions, they're specified in this
//        order:  1 2 3 4 5 6
//
// This routine converts the provided string character to an offset into a
// lookup table, which provides the values to turn each character's
// segments on/off.
//*****************************************************************************
void myLCD_showChar( char c, int Position )
{
    Position -= 1;

    if ( ( Position >= 0 ) && ( Position <= 6 ) )
    {

        if ( c == ' ' )
        {
            // Display space
            LCD_E_setMemory( LCD_E_BASE, location[Position], 0 );
            LCD_E_setMemory( LCD_E_BASE, location[Position] + 1, 0 );
        }
        else if ( c >= '0' && c <= '9' )
        {
            // Display digit
            LCD_E_setMemory( LCD_E_BASE, location[Position], digit[c-48][0] );
            LCD_E_setMemory( LCD_E_BASE, location[Position] + 1, digit[c-48][1] );
        }
        else if ( c >= 'A' && c <= 'Z' )
        {
            // Display alphabet
            LCD_E_setMemory( LCD_E_BASE, location[Position], alphabetBig[c-65][0] );
            LCD_E_setMemory( LCD_E_BASE, location[Position] + 1, alphabetBig[c-65][1] );
        }
        else
        {
            // Turn all segments on if character is not a space, digit, or uppercase letter
            LCD_E_setMemory( LCD_E_BASE, location[Position], 0xFF );
            LCD_E_setMemory( LCD_E_BASE, location[Position] + 1, 0xFF );
        }
    }
}


//*****************************************************************************
// myLCD_displayString()
//
// Displays the provided string of LCD_NUM_CHAR (i.e. six) characters. If fewer
// than LCD_NUM_CHAR chars are provided, the function will put ' ' in the
// remaining characters. The function ignores any characters past LCD_NUM_CHAR.
//
// Arg 1: "char s[6]" is a string of 6 characters
//
// There are different ways you can declare and pass a string to this function.
// For examples, look at the tests we made when calling this function. You'll
// find these commented out at the end of the myLcd_init() function.
//
// This function does not provide any 'punctuation'. That is, decimals, signs
// or colons. You could add these using the myLCD_showSymbol() function.
//*****************************************************************************
void myLCD_displayString( char s[ LCD_NUM_CHAR ] )
{
	int i;                                                                      // Loop variable
	int p = 0;                                                                  // Location of end-of-string (EOS) character

	for ( i = 0; i < LCD_NUM_CHAR; i++ )                                        // Loop for the number of char's in the display
	{
		if ( ( s[i] == '\0' ) || ( p >> 0 ) )                                   // If EOS is found (or has been found) display ' ' (i.e. space)
		{
			myLCD_showChar( ' ', i+1 );                                         // Display space char
			p = i;                                                              // Set p = location where first EOS is found
		}
		else
		{
			myLCD_showChar( s[i], i+1 );                                        // Display character from string
		}
	}
}

//*****************************************************************************
// myLCD_displayNumber()
//
// Displays a right-aligned number on the display. This function takes one
// input value and does not return a value.
//
// Arg 1: "val" is the integer to be displayed. It must be less than 6 digits
//        in length or this function writes "ERROR". The number is first
//        converted into character equivalants, then displayed.
//
// This function does not provide any 'punctuation'. That is, decimals, signs
// or colons. You could add these using the myLCD_showSymbol() function.
//*****************************************************************************
void myLCD_displayNumber( unsigned long val )
{
    unsigned long div[6];
    unsigned long mod[6];
    unsigned long err;

    err = val / 1000000;
    if ( err > 0 )
    {
        myLCD_showChar( ' ', 1 );
        myLCD_showChar( 'E', 2 );
        myLCD_showChar( 'R', 3 );
        myLCD_showChar( 'R', 4 );
        myLCD_showChar( 'O', 5 );
        myLCD_showChar( 'R', 6 );
    }
    else
    {
        div[0] = val / 100000  ;
        mod[0] = val % 100000  ;
        div[1] = mod[0] / 10000;
        mod[1] = mod[0] % 10000;
        div[2] = mod[1] / 1000 ;
        mod[2] = mod[1] % 1000 ;
        div[3] = mod[2] / 100  ;
        mod[3] = mod[2] % 100  ;
        div[4] = mod[3] / 10   ;
        div[5] = mod[3] % 10   ;

        int i = 0;
        int LeadingZeros = 0;

        for ( i = 0; i < ( LCD_NUM_CHAR - 1); i++ )
        {
            if ( ( div[i] == 0 ) && ( !LeadingZeros ) )
            {
                myLCD_showChar( ' ', i + 1 );
            }
            else
            {
                myLCD_showChar( div[i] + '0', i + 1 );
                LeadingZeros++;
            }
        }

        i = LCD_NUM_CHAR - 1;
        myLCD_showChar( div[i] + '0', i + 1 );
    }
}

//*****************************************************************************
// TEST_myLCD_showSymbol()
//
// Provides a single function which tests every mode of the myLCD_showSymbol()
// function. You should be able to verify it's correct operation using with
// this function - along with use of breakpoints, single-stepping and a little
// manipulation of the LCD memory in the CCS Registers window.
//*****************************************************************************
void TEST_myLCD_showSymbol( void )
{
    int i;                                                                      // Iterates thru Operations
    int j;                                                                      // Iterates thru Symbols
    int m;                                                                      // Iterates thru Memories
    volatile int r = -1;                                                        // Let's you view the return value (volatile needed to compiler won't optimize it away)

    for ( m = 0; m < 2; m++ )
    {
        if ( m == 1 )
        {
            LCD_E_selectDisplayMemory( LCD_E_BASE, LCD_E_DISPLAYSOURCE_BLINKINGMEMORY );
        }
        else
        {
            LCD_E_selectDisplayMemory( LCD_E_BASE, LCD_E_DISPLAYSOURCE_MEMORY );
        }

        for ( i = 0; i <= 3; i++ )
        {
            for ( j = 0; j <= LCD_A4COL; j++ )
            {
                r = myLCD_showSymbol( i, j, m );
            }
        }
    }

    // Clear LCD memory and restore to display to main LCD memory
    LCD_E_off( LCD_E_BASE );
    LCD_E_selectDisplayMemory( LCD_E_BASE, LCD_E_DISPLAYSOURCE_MEMORY );
    LCD_E_clearAllMemory( LCD_E_BASE );
    LCD_E_clearAllBlinkingMemory( LCD_E_BASE );
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_MEMORY_COM0 );
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_1, LCD_E_MEMORY_COM1 );
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_2, LCD_E_MEMORY_COM2 );
    LCD_E_setPinAsCOM( LCD_E_BASE, LCD_E_SEGMENT_LINE_3, LCD_E_MEMORY_COM3 );
    LCD_E_on( LCD_E_BASE );
}

//*****************************************************************************
// myLCD_showSymbol()
//
// The function shows (or modifies) the LCD memory bits to control the various
// special icons (i.e. non-character segments) of the display.
//
// This function has three arguments and returns either '0' or '1'.
//
//      Return - reflects the value of the icon's segment enable bit
//               at the end of the function
//      Arg 1  - 'Operation' specifies on of 4 different functions
//               that can be performed on an icon
//      Arg 2  - 'Symbol' indicates the special icon segment operated upon
//      Arg 3  - 'Memory' specifies which LCD memory (main or blinking)
//               the function should modify or test
//
// Operation:    The function can perform 4 different functions on each icon:
//
//      LCD_UPDATE - sets the segment bit to enable the icon
//      LCD_CLEAR  - clears the icon's segment bit
//      LCD_TOGGLE - toggles the icon's segment bit
//      LCD_GET    - returns the value of the icon's segment bit
//
//      Additionally, all operations return the icon's segment bit value.
//      For UPDATE, this should always be '1'; and for CLEAR, this should
//      be '0'; the others will vary based on the current value (GET) or
//      new value (TOGGLE).
//
//  Symbol:      The 'icon' value is used to select which LCD memory register
//               and bit to operate upon.
//
//  Memory:      Determines which LCD memory to act upon. '0'
//               represents the Main LCD memory registers; while '1' represents
//               the Blinking LCD memory registers. This allows a single
//               function to perform all the possible operations for a segment.
//
// The associated header file enumerates these operations as well as the various
// icons found on this LCD.
//*****************************************************************************
int myLCD_showSymbol( int Operation, int Symbol, int Memory )
{
    int idx = 0;                                                                // Specifies the index of the LCD memory register (i.e. x in LCDMx)
    int bit = 0;                                                                // Specifies the charachters bit within the LCD memory register
    int mem = 0;                                                                // Offset for LCD's "main" or "blinking" memory registers (either 0 or 32)
    int ret = -1;                                                               // Holds the function's return value

    // Which Memory has the user specified?  'Main' or 'Blinking'
    // Since 'Blinking' only applies to MUX4 (and lower) configurations, we ignore the Memory field if it does not apply.
    // This function cheats - instead of using LCDMx (for main) or LCDBMx (for blinking), we cheated and made use of the fact that the
    // two sets of LCD memory registers are aliased. Therefore, setting LCDBM3 also sets LCDM35.
    // Note, we made 'initParams' a global variable to make this test easier; though, we could have also tested the LCD register itself.
    if (( initParams.muxRate <= LCD_E_4_MUX ) && ( Memory == 1 ))
        mem = 35-3;                                                             // Writing to LCDBM35 also sets LCDM3 (and so on with the other values)

    // Select the LCDM memory register index (idx) and bit depending upon the provided Symbol value
    switch ( Symbol )
    {
    case LCD_TMR:
        idx = LCD_TIMER_IDX;
        bit = LCD_TIMER_COM;
        break;
    case LCD_HRT:
        idx = LCD_HEART_IDX;
        bit = LCD_HEART_COM;
        break;
    case LCD_REC:
        idx = LCD_REC_IDX;
        bit = LCD_REC_COM;
        break;
    case LCD_EXCLAMATION:
        idx = LCD_EXCLAMATION_IDX;
        bit = LCD_EXCLAMATION_COM;
        break;
    case LCD_BATT:
        idx = LCD_BATT_IDX;
        bit = LCD_BATT_COM;
        break;
    case LCD_BRACKETS:
        idx = LCD_BRACKETS_IDX;
        bit = LCD_BRACKETS_COM;
        break;
    case LCD_B6:
        idx = LCD_B6_IDX;
        bit = LCD_B6_COM;
        break;
    case LCD_B5:
        idx = LCD_B5_IDX;
        bit = LCD_B5_COM;
        break;
    case LCD_B4:
        idx = LCD_B4_IDX;
        bit = LCD_B4_COM;
        break;
    case LCD_B3:
        idx = LCD_B3_IDX;
        bit = LCD_B3_COM;
        break;
    case LCD_B2:
        idx = LCD_B2_IDX;
        bit = LCD_B2_COM;
        break;
    case LCD_B1:
        idx = LCD_B1_IDX;
        bit = LCD_B1_COM;
        break;
    case LCD_ANT:
        idx = LCD_ANT_IDX;
        bit = LCD_ANT_COM;
        break;
    case LCD_TX:
        idx = LCD_TX_IDX;
        bit = LCD_TX_COM;
        break;
    case LCD_RX:
        idx = LCD_RX_IDX;
        bit = LCD_RX_COM;
        break;
    case LCD_NEG:
        idx = LCD_NEG_IDX;
        bit = LCD_NEG_COM;
        break;
    case LCD_DEG:
        idx = LCD_DEG_IDX;
        bit = LCD_DEG_COM;
        break;
    case LCD_A1DP:
        idx = LCD_A1DP_IDX;
        bit = LCD_A1DP_COM;
        break;
    case LCD_A2DP:
        idx = LCD_A2DP_IDX;
        bit = LCD_A2DP_COM;
        break;
    case LCD_A3DP:
        idx = LCD_A3DP_IDX;
        bit = LCD_A3DP_COM;
        break;
    case LCD_A4DP:
        idx = LCD_A4DP_IDX;
        bit = LCD_A4DP_COM;
        break;
    case LCD_A5DP:
        idx = LCD_A5DP_IDX;
        bit = LCD_A5DP_COM;
        break;
    case LCD_A2COL:
        idx = LCD_A2COL_IDX;
        bit = LCD_A2COL_COM;
        break;
    case LCD_A4COL:
        idx = LCD_A4COL_IDX;
        bit = LCD_A4COL_COM;
        break;
    default:
        break;
    }

    // This switch acts upon the correct icon segment based upon the Operation specified by the user
    switch ( Operation )
    {
    case LCD_UPDATE:
        LCDMEM[ idx + mem ] |= bit;
        break;
    case LCD_CLEAR:
        LCDMEM[ idx + mem ] &= ~bit;
        break;
    case LCD_TOGGLE:
        LCDMEM[ idx + mem ] ^= bit;
        break;
    }

    // The LCD_GET operation is always performed; this is what is returned by the function
    if ( ( LCDMEM[ idx + mem ] & bit ) >> 0 )
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ( ret );
}


