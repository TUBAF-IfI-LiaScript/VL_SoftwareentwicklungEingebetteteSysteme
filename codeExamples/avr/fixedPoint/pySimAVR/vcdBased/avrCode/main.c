#include <avr/io.h>
#include <util/delay.h>
#include "AVR_writeSerial.h"

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

int main()
{
    DDRB |= (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);
    stdin=stdout=&usart0_str;
    USART0Init();

    //just for testing purposes
    PORTB |= (1 << PB0);
    _delay_ms (100);
    PORTB &= ~(1 << PB0);

    int meas[] = {140, 170, 160, 200, 220};
    int count = 101;

    //// unsigned short _Accum
    unsigned short _Accum weight_sfix = 0.9K;
    unsigned short _Accum result_sfix = 0.0K;

    PORTB |= (1 << PB1);
    for (int i=0; i<count; i++){
       result_sfix = meas[i%5] * weight_sfix + result_sfix * (1-weight_sfix);
    }
    PORTB &= ~(1 << PB1);
    printf("\nshort _Accum");
    fprintf(stdout,"result = %8.3f\n", (float)result_sfix);

    //// unsigned long _Accum
    unsigned long _Accum weight_ulfix =  0.9K;
    unsigned long _Accum result_ulfix = 0.0K;

    PORTB |= (1 << PB2);
    for (int i=0; i<count; i++){
       result_ulfix = meas[i%5] * weight_ulfix + result_ulfix * (1-weight_ulfix);
    }
    PORTB &= ~(1 << PB2);
    printf("\nunsigned long _Accum");
    fprintf(stdout,"result = %8.3f\n", (float)result_ulfix);

    //// long _Accum
    long _Accum weight_lfix =  0.9K;
    long _Accum result_lfix = 0.0K;

    PORTB |= (1 << PB3);
    for (int i=0; i<count; i++){
       result_lfix = meas[i%5] * weight_lfix + result_lfix * (1-weight_lfix);
    }
    PORTB &= ~(1 << PB3);
    printf("\nlong _Accum");
    fprintf(stdout,"result = %8.3f", (float)result_lfix);

//  Integrieren Sie eine float Implementierung und bewerten Sie die Resultate!

    return 0;
}
