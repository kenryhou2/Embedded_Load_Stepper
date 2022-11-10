
#include <driverlib.h>


int main(void) {

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Set P1.0 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1);


    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PMM_unlockLPM5();

    volatile unsigned short usiButton1=0;

    while(1)
    {
        usiButton1 = GPIO_getInputPinValue (GPIO_PORT_P1, GPIO_PIN1);
        if( usiButton1 == GPIO_INPUT_PIN_LOW)
        {
            GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0);
        }

        else
        {
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0);
        }
    }
}
