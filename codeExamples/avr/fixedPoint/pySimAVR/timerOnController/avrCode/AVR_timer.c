#include "AVR_timer.h"

volatile counter_t timer0_overflow_count    = 0;
volatile counter_t timer0_overflow_count_oc = 0;

ISR(TIMER0_OVF_vect)
{
  counter_t ov = timer0_overflow_count;
  ov++;
  timer0_overflow_count = ov;
  if ( sizeof(ticks_t) > (sizeof(ov)+1) )  //static evaluated
      if( ov == 0 ) // ov overflow
        timer0_overflow_count_oc++;
}

//PRESCALER defined in header
#if PRESCALER == 1
#define TCCR0B_CS (1 << CS00)
#elif  PRESCALER == 8
#define TCCR0B_CS (1 << CS01)
#elif PRESCALER == 64
#define TCCR0B_CS (1 << CS01) | (1 << CS00)
#elif PRESCALER == 256
#define TCCR0B_CS (1 << CS02)
#elif PRESCALER == 1024
#define TCCR0B_CS (1 << CS02) | (1 << CS00)
#else
#error help me to find CS
#endif

void init_timer(){
  // fast pwm
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // defined by PRESCALER
  TCCR0B |= TCCR0B_CS ;
  // timer interrupt on
  TIMSK0 |= (1 << TOIE0);
}

ticks_t now(void){
    uint8_t oldSREG = SREG;
    cli();
    uint8_t tim = TCNT0;
    counter_t ov = timer0_overflow_count;
    ov += ((TIFR0 & _BV(TOV0)) && (tim < 255));
    counter_t oc = 0;
    //this is staticaly evaluated and removed if not need
    if( sizeof(ticks_t) > (sizeof(ov)+1) ){
        oc = timer0_overflow_count_oc;
    }
    SREG = oldSREG;

    return ((uint32_t)oc << (8 * (sizeof(ov)+1))) + ((ticks_t)ov << 8) + tim;
}

unsigned long millis(ticks_t ticks)
{
  return (ticks * PRESCALER)/(F_CPU/1000);
}

unsigned long micros(ticks_t ticks)
{
  return (ticks * PRESCALER)/(F_CPU/1000/1000);
}
