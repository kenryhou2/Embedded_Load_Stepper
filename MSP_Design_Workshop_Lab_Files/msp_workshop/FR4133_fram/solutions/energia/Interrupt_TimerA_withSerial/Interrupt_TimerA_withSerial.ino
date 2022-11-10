/*
  *****************************************************************************
   WARNING  (FR4133 LaunchPad)
  *****************************************************************************
  The RDX/TDX pins on the J101 jumper (between the eZ-FET emulation and the
  target side of the FR4133 board) may not be populated by default. This
  is due to P1.0 being shared by the LED and the serial port.
  
  For the serial examples, you may want to move the pins from the RTS/CTS
  jumper pins over to the RTX/TDX pins. The flow control pins are not
  normally used by the serial module, so this is an easy way to find pins
  for the serial connections.
  
  Additionally, you need to make sure you are not driving P1.0 (LED1 or RED_LED)
  at the same time you're using the Energia "Serial" module.
  *****************************************************************************
*/

#include <inttypes.h>

uint8_t  timerCount  = 0;
uint16_t ToggleCount = 0;

void setup()
{
  // make sure RTC is turned off
  RTCCTL = 0;

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // put your setup code here, to run once:
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  
  TA0CCTL0 = CCIE;
  TA0CTL   = TASSEL_2 + MC_2;
}

void loop()
{
  // Nothing to do
  
}

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void myTimer_A(void)
{
  Serial.print("*");
    
  timerCount = (timerCount + 1) % 80;
  if(timerCount == 0) {
  P4OUT ^= 1;  // Toggle GREEN_LED

    ToggleCount++;
    Serial.println("|");
    Serial.print("Count:");
    Serial.println(ToggleCount);
  }
}
