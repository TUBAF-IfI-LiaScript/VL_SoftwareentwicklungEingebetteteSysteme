// #define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "AVR_writeSerial.h"
#include "AVR_timer.h"

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

volatile extern unsigned long timer0_overflow_count;

int main()
{
    stdin=stdout=&usart0_str;
    USART0Init();

    init_timer();
    sei();

    int result = 0;

    ticks_t start = now();
    // result = calc(3, global_variable);
    // printf("\nresult = %u",result);
     _delay_ms (100);
    ticks_t stop = now();

    printf("\nduration = %ld [us]\n",micros(stop - start));

    printf("ticks = 0x%"PRI_ticks_t"x\n",stop - start);

    printf("result = %d\n",result);
//     float myfloat = 3.14;
// 
//     fprintf(stdout,"\nfloat = %08.3f", myfloat);
    return 0;
}
