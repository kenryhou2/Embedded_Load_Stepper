#include <inttypes.h>

uint8_t timerCount = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
  
  TA0CCTL0 = CCIE;
  TA0CTL = TASSEL_2 + MC_2;
}

void loop()
{
  // Nothing to do
  
}

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void myTimer_A(void)
{
timerCount = (timerCount + 1) % 800;
if(timerCount ==0)
P1OUT ^= 1;
}
