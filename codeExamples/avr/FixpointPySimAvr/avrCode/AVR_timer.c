#include "AVR_timer.h"

ISR(TIMER0_OVF_vect)
{
  // copy these to local variables so they can be stored in registers
  // (volatile variables must be read from memory on every access)
  unsigned long m = timer0_millis;
  unsigned char f = timer0_fract;

  m += MILLIS_INC;
  f += FRACT_INC;
  if (f >= FRACT_MAX) {
    f -= FRACT_MAX;
    m += 1;
  }

  timer0_fract = f;
  timer0_millis = m;
  timer0_overflow_count++;
}

void init_timer(){
  // fast pwm
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // this combination is for the standard 168/328/1280/2560
  // prescaler 64
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // timer interrupt on
  TIMSK0 |= (1 << TOIE0);
}

unsigned long millis()
{
  unsigned long m;
  uint8_t oldSREG = SREG;
  // disable interrupts while we read timer0_millis or we might get an
  // inconsistent value (e.g. in the middle of a write to timer0_millis)
  cli();
  m = timer0_millis;
  SREG = oldSREG;
  return m;
}

unsigned long micros()
{
  unsigned long m;
  uint8_t oldSREG = SREG, t;

  cli();
  m = timer0_overflow_count;
  t = TCNT0;

  if ((TIFR0 & _BV(TOV0)) && (t < 255)) {
    m++;
  }
  SREG = oldSREG;
  return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}
