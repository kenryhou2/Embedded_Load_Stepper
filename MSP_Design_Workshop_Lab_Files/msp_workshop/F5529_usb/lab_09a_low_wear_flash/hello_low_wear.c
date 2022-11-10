// ----------------------------------------------------------------------------
// hello.c  (for lab_09a_low_wear_flash project)  ('F5529 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include <stdio.h>


//***** Prototypes ************************************************************
void initGPIO(void);
void write_infoB( uint16_t );
uint16_t write_infoB_location( uint16_t, int );
void erase_infoB( void );
void fill_infoB( void );


//***** Defines ***************************************************************
#define ONE_SECOND    1000000        // Well, it's about a second
#define HALF_SECOND   1000000 / 2
#define INFOB_START   (0x1900)       // INFO address in flash to which data is copied into
#define NUMBER_OF_BYTES   128        // Length of InfoB
#define NUMBER_OF_WORDS   NUMBER_OF_BYTES/2


//***** Global Variables ******************************************************
// We arbitrarily chose to use INFOB for this lab exercise.
#pragma DATA_SECTION ( count, ".infoB" )
uint16_t count[NUMBER_OF_WORDS];

volatile uint16_t erase = 0;
volatile uint16_t fill  = 0;

//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    uint16_t i = 0;
    uint16_t val = 0;
    uint16_t loc = 0;
    uint16_t status = 0;

    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Initialize GPIO
    initGPIO();

    if ( erase != 0 )                                  // This lets you easily erase the flash by changing the value
        erase_infoB();                                 // of the "erase" variable before reaching this point in the code

    if ( fill != 0 )                                   // This lets you easily fill the flash by changing the value
        fill_infoB();                                  // of the "fill" variable before reaching this point in the code

    if (count[ 0 ] == 0xffff )
    {
        val = 1;
        loc = 0;
    }
    else if ( count[ NUMBER_OF_WORDS-1 ] != 0xFFFF )
    {
        val = count[ NUMBER_OF_WORDS-1 ];
        erase_infoB();
        loc = 0;
    }
    else
    {
        for ( i = 1; i < NUMBER_OF_WORDS-2; i++ )
        {
            if ( count[ i ] == 0xffff )
            {
                val = count[ i - 1 ];
                val++;
                loc = i;
                break;
            }
        }
    }

    status = write_infoB_location( val, loc );
    if ( status == 0 )
        while( 1 );

    // Inform the programmer how many times the board has been power-on reset
    //   which isn't possible without some form of non-volatile memory
printf( "This program has been power cycled %d times\n", val );

    // Flash the LED 'count' number of times ... this way you can 'see' how many
    //   times a power-on reset has occurred, even if you can't read the printf()
    for ( i = 0; i < val; i++ )
    {
        // Turn on LED
        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait about a second
        __delay_cycles( ONE_SECOND );

        // Turn off LED
        GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait another second
        __delay_cycles( HALF_SECOND );
    }

    // Trap program here
    while(1);
}

//*****************************************************************************
void initGPIO(void)
{
    // Set pin P1.0 to output direction and turn off LED
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    __delay_cycles( HALF_SECOND );

}

//*****************************************************************************
void erase_infoB( void )
{
    uint16_t status;

    // Erase INFOB
    do {
         FlashCtl_eraseSegment( (uint8_t*)INFOB_START );
         status = FlashCtl_performEraseCheck( (uint8_t*)INFOB_START, NUMBER_OF_BYTES );
    } while (status == STATUS_FAIL);
}

//*****************************************************************************
uint16_t write_infoB_location( uint16_t val, int loc )
{
    if ( count[loc] == 0xFFFF )
    {
    // Flash Write
    FlashCtl_write16( &val,
                      &count[loc],
                      1
    );
        return( 1 );
    }
    else
    {
        return( 0 );
    }
}

//*****************************************************************************
void fill_infoB( void )
{
    uint16_t status;
    uint16_t i = 0;

    // Erase INFOB
    do {
         FlashCtl_eraseSegment( (uint8_t*)INFOB_START );
         status = FlashCtl_performEraseCheck( (uint8_t*)INFOB_START, NUMBER_OF_BYTES );
    } while (status == STATUS_FAIL);

//  // Flash Write
//  FlashCtl_memoryFill32( 0xbeefdead, (uint32_t*)INFOB_START, NUMBER_OF_WORDS/2 );

    for ( i = 1; i <= NUMBER_OF_WORDS; i++ )
    {
        // Flash Write
        FlashCtl_write16( &i, &count[i-1], 1 );
    }

    fill = 0;
}

