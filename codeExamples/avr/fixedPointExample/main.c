#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdfix.h>

int main (void) {

  DDRB |= (1<<DDB7);

  unsigned short _Accum variableA = 0.75K;
  short _Accum variableB = -1.875K;

  long _Accum result = variableA * variableB;

  while(1){
    if (result<0) PORTB |= (1<<7);
  }
  return 0;
}
