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
    _______________ ( LCD_E_BASE,
            ___________________________,                                             // LCDMx location
            ___________________________                                              // LCD_HEART_COM                                                            // "LCD_" symbols are defined in the 'myLcd.h' header file
    );

    // Set the LCD's "Timer" symbol
    LCD_E_setMemory( LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER_COM );    // Heart replaced by Timer symbol

    // Updates only the specified segments (in this case, adding the Heart segment)
    LCD_E_updateMemory( LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_HEART_COM ); // Both Heart and Timer symbols should be displayed

    // Clears only the specified segments (in this case, the Timer segment)
    __________________ ( LCD_E_BASE, LCD_TIMER_IDX, LCD_TIMER_COM );                   // Erases just the Timer symbol (this time, we used our #define for the LCD memory index)

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
    ________________________ ( LCD_E_BASE,
            LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64,
            _____________________________
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

