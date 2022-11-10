// ----------------------------------------------------------------------------
// hello.c  (for lab_09a_info_flash project)  ('F5529 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include <stdio.h>


//***** Prototypes ************************************************************
void initGPIO(void);
void write_infoB( uint16_t *, uint16_t * );


//***** Defines ***************************************************************
#define ONE_SECOND    1000000        // Well, it's about a second
#define HALF_SECOND   1000000 / 2
#define INFOB_START   (0x1900)       // INFO address in flash to which data is copied into
#define NUMBER_OF_BYTES   128        // Length of InfoB


//***** Global Variables ******************************************************
// We arbitrarily chose to use INFOB for this lab exercise.
#pragma ______________ ( ____________ , "___________" )
uint16_t count;


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    uint16_t i = 0;
    uint16_t c = 0;

    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Initialize GPIO
    initGPIO();

    // Read and increment 'count' variable which is located in flash Info B segment
    c = count;
    c++;

    write_infoB( &c, &count );

    // Inform the programmer how many times the board has been power-on reset
    //   which isn't possible without some form of non-volatile memory
    printf( "This program has been power cycled %d times\n", c );

    // Flash the LED 'count' number of times ... this way you can 'see' how many
    //   times a power-on reset has occurred, even if you can't read the printf()
    for ( i = 0; i < count; i++ )
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
void write_infoB( uint16_t *value, uint16_t *flashLocation )
{
    uint16_t status;

    // Erase INFOB
    do {
         _____________________( (uint8_t*)INFOB_START );
         status = FlashCtl_performEraseCheck( (uint8_t*)INFOB_START, NUMBER_OF_BYTES );
    } while (status == STATUS_FAIL);

    // Flash Write
    ________________( (uint16_t*) value,
                      (uint16_t*) flashLocation,
                      1
    );
}
