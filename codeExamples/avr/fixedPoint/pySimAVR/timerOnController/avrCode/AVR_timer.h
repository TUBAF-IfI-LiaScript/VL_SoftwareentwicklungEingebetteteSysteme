#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#ifndef AVR_TIMER_H
#define AVR_TIMER_H

//not used
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
//not used
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )

//higher prescaler less overflow and less resolution 1, 8, 64, 256, 1024
//Prescaler         1,     8,      64,      256,      1024
//overflow       16µs, 128µs, 1.024ms,  4.096ms, 16.384 ms
//resolution   1/16µs, 1/2µs,     4µs,     16µs,      64µs
//expect a jitter of +-1
#define PRESCALER    (256UL)
// #define PRESCALER    (1024UL)


// these typedef define the performance of the counter
// 8/32 has 24 bits usable (8+8+8) (HW_Timer + overflow (ov) + overflow overflow (oc))
// 16/32 has (8+16+16) (40 bits) (32 shown)

//isrsize tick_t|  16 |  32 |   size of ISR in hex
//counter_t     |     |     |   width in bits
//           8  |  22 |  30 |
//          16  |  2e |  44 |
//          32  |  -  |  4a | 32/64  78
//
// 8/32 PRE 256 F_CPU 16MHz has 16µs Resolution and ~268 sec max diff time

typedef uint8_t counter_t;

#ifndef PRIx64
#define PRIx64 "llx"
#endif
typedef uint32_t ticks_t;
#define PRIx_ticks_t PRIx32

void init_timer();
ticks_t now(void);

//millis and micros conversion is not checked
//for high ticks they may need casts
unsigned long millis(ticks_t ticks);
unsigned long micros(ticks_t ticks);

#endif
