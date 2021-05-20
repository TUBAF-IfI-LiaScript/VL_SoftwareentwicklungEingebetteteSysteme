#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <avr/delay.h>

void setServo(char port, float input)
{
  //                               ??         ??
  uint16_t newValue = (uint16_t) (1000+input*4000);
  switch (port)
  {
    case 'A':
      OCR1A = newValue;
      break;
    case 'B':
      OCR1B = newValue;
      break;
  }
}

void startPeriod20( void )
{
  // turn on PB1 and PB2 as output
  DDRB |= (1<<DDB2) | (1<<DDB1);
  
  // set up timer to FastPWM mode
  TCCR1A |= (1<<WGM11);
  TCCR1B |= (1<<WGM13) | (1<<WGM12);
  
  // turn off the pin when compare value is reached, turn on again at BOTTOM
  // also called non-inverted mode
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1);

  // start Timer with prescale 8
  TCCR1B |= (1<<CS11);

  // set period to  20ms, 1ms is 2000
  ICR1 = 40000;

  //  1ms      2ms
  //  2000     4000
  //  |        |
  //--------------------------
  //       40000 Ticks = 20ms 
}

int main(void)
{
  startPeriod20();
  float t = 0;
  setServo('A',0.0);
  while(1);
  
  while(1)
  {
    t += 0.005;
    if (t>1) t--;
    setServo('A', t);
    _delay_ms(20);
  }
 
  return 0;
}