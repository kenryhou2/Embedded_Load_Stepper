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

void setup()
{
  // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("on");

}

void loop()
{
  // put your main code here, to run repeatedly:
  
}
