#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "AVR_writeSerial.h"
#include "AVR_timer.h"

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

int main()
{
    stdin=stdout=&usart0_str;
    USART0Init();
        
    unsigned long start;
    unsigned long stop;  
    init_timer(); 
    sei();
  
    int result;
    start = micros();
    // result = calc(3, global_variable);
    // printf("\nresult = %u",result);
     _delay_ms (100);
    stop = micros();
    printf("\nduration = %ld [us]",(stop - start));
//     float myfloat = 3.14;
// 
//     fprintf(stdout,"\nfloat = %08.3f", myfloat);
    return 0;
}
