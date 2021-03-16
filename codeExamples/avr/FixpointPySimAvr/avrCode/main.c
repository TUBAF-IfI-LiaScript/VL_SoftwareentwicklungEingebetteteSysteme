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

    //just for testing purposes
    ticks_t start = now();
     _delay_ms (100);
    ticks_t stop = now();
    printf("\n100ms delay");
    printf("\nduration = %ld [us]\n",micros(stop - start));
    printf("ticks    = 0x%"PRIx_ticks_t"\n",stop - start);

    int meas[] = {340, 370, 360, 400, 420};
    unsigned short _Accum weight_sfix = 0.9K;
    unsigned short _Accum result_sfix = 0.0K;

    start = now();
    for (int i=0; i<100; i++){
       result_sfix = meas[i%5] * weight_sfix + result_sfix * (1-weight_sfix);
    }
    stop = now();
    printf("\nshort _Accum");
    printf("\nduration = %ld [us]\n",micros(stop - start));
    printf("ticks    = 0x%"PRIx_ticks_t"\n",stop - start);
    fprintf(stdout,"result = %08.3f\n", (float)result_sfix);

    unsigned long _Accum weight_lfix =  0.9K;
    unsigned long _Accum result_lfix = 0.0K;

    start = now();
    for (int i=0; i<100; i++){
       result_lfix = meas[i%5] * weight_lfix + result_lfix * (1-weight_lfix);
    }
    stop = now();
    printf("\nlong _Accum");
    printf("\nduration = %ld [us]\n",micros(stop - start));
    printf("ticks    = 0x%"PRIx_ticks_t"\n",stop - start);
    fprintf(stdout,"result = %08.3f", (float)result_lfix);

//     float myfloat = 3.14;
//
//     fprintf(stdout,"\nfloat = %08.3f", myfloat);
    return 0;
}
