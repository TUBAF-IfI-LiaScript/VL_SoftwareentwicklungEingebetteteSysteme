#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
    PORTD.DIRSET = PIN6_bm;
    PORTD.OUTSET = PIN6_bm;
    while (1) 
    {
        _delay_ms(500);
        PORTD.OUTTGL = PIN6_bm;
    }
}