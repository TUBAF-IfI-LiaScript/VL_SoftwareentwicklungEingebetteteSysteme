#include <avr/io.h>
#include <util/delay.h>
#include <stdfix.h>
#include "AVR_writeSerial.h"
#include "AVR_timer.h"

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

volatile extern unsigned long timer0_overflow_count;

void printDuration(ticks_t start, ticks_t stop){
  printf("\nduration = %ld [us]\n",micros(stop - start));
  printf("ticks    = 0x%"PRIx_ticks_t"\n",stop - start);
}

int main()
{
    stdin=stdout=&usart0_str;
    USART0Init();

    init_timer();
    sei();

    printf("\n-----------------------------\n");

    //just for testing purposes
    ticks_t start = now();
    _delay_ms (100);
    ticks_t stop = now();
    printf("\n1. 100ms delay");
    printf("\nduration = %ld [us]\n",micros(stop - start));
    printf("ticks    = 0x%"PRIx_ticks_t"\n",stop - start);
    

    int meas[] = {140, 170, 160, 200, 220};
    int count = 101;

    //// unsigned short _Accum

    unsigned short _Accum weight_sfix = 0.9K;
    unsigned short _Accum result_sfix = 0.0K;

    start = now();
    for (int i=0; i<count; i++){
       result_sfix = meas[i%5] * weight_sfix + result_sfix * (1-weight_sfix);
    }
    stop = now();
    printf("\n2. short _Accum");
    printDuration(start, stop);
    fprintf(stdout,"result = %8.3f\n", (float)result_sfix);

    //// unsigned long _Accum

    unsigned long _Accum weight_ulfix =  0.9K;
    unsigned long _Accum result_ulfix = 0.0K;

    start = now();
    for (int i=0; i<count; i++){
       result_ulfix = meas[i%5] * weight_ulfix + result_ulfix * (1-weight_ulfix);
    }
    stop = now();
    printf("\n3. unsigned long _Accum");
    printDuration(start, stop);
    fprintf(stdout,"result = %8.3f\n", (float)result_ulfix);

    //// long _Accum

    long _Accum weight_lfix =  0.9K;
    long _Accum result_lfix = 0.0K;

    start = now();
    for (int i=0; i<count; i++){
       result_lfix = meas[i%5] * weight_lfix + result_lfix * (1-weight_lfix);
    }
    stop = now();
    printf("\n4. long _Accum");
    printDuration(start, stop);
    fprintf(stdout,"result = %8.3f", (float)result_lfix);

//  Integrieren Sie eine float Implementierung und bewerten Sie die Resultate!

    return 0;
}