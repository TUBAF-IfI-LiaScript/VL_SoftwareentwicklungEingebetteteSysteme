#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdfix.h>

int main (void) {

  unsigned short _Accum fixVarA = 1.5K;
  short _Accum fixVarB =  -1.5K;
  long _Accum fixResult = fixVarA * fixVarB;

  while(1);
  return 0;
}
