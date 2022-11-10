#include <inttypes.h>

uint8_t  timerCount  = 0;
uint16_t ToggleCount = 0;

void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  
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
    P1OUT ^= 1;

    ToggleCount++;
    Serial.println("|");
    Serial.print("Count:");
    Serial.println(ToggleCount);
  }
}
