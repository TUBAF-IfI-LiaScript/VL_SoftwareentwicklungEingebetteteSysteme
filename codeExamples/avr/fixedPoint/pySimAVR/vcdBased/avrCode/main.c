#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRB |= (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);

    //just for testing purposes
    PORTB |= (1 << PB0);
    _delay_ms (100);
    PORTB &= ~(1 << PB0);

    int meas[] = {140, 170, 160, 200, 220};
    int count = 101;

    //// unsigned short _Accum
    unsigned short _Accum weight_sfix = 0.9K;
    volatile unsigned short _Accum result_sfix = 0.0K;

    PORTB |= (1 << PB1);
    for (int i=0; i<count; i++){
       result_sfix = meas[i%5] * weight_sfix + result_sfix * (1-weight_sfix);
    }
    PORTB &= ~(1 << PB1);

    //// unsigned long _Accum
    unsigned long _Accum weight_ulfix =  0.9K;
    volatile unsigned long _Accum result_ulfix = 0.0K;

    PORTB |= (1 << PB2);
    for (int i=0; i<count; i++){
       result_ulfix = meas[i%5] * weight_ulfix + result_ulfix * (1-weight_ulfix);
    }
    PORTB &= ~(1 << PB2);

    //// long _Accum
    long _Accum weight_lfix =  0.9K;
    volatile long _Accum result_lfix = 0.0K;

    PORTB |= (1 << PB3);
    for (int i=0; i<count; i++){
       result_lfix = meas[i%5] * weight_lfix + result_lfix * (1-weight_lfix);
    }
    PORTB &= ~(1 << PB3);

//  Integrieren Sie eine float Implementierung und bewerten Sie die Resultate!

    return 0;
}
