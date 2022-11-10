/*
  myDigitalReadSerial with on-board Pushbutton
  Reads a digital input on pushbutton 2, then
  prints the result to the serial monitor 
  
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
  
  This example code is in the public domain.
 */

// PUSH2 pin has a pushbutton attached to it. Give it a name:
int pushButton = PUSH2;  // For MSP430FR4133 LaunchPad, this is pin 25


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); 
  // make the on-board pushbutton's pin an input pullup:
  pinMode(pushButton, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  // print out the state of the button:
  Serial.println(buttonState);
  delay(1);        // delay in between reads for stability
}



