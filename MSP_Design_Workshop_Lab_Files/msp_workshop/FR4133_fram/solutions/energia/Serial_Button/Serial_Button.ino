/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 
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
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 modified Apr 27 2012
 by Robert Wessels
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = PUSH2;     // the number of the pushbutton pin
const int ledPin =  GREEN_LED;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);     
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // print out the state of the button:
  Serial.println(buttonState);
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }
}
