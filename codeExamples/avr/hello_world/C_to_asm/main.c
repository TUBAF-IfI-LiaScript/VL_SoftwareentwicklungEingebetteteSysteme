#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main (void) {
   
   DDRB |= (1 << PB0);
   // Equivalent to
   // (*(volatile uint8_t *)(0x24)) |= 1; 

   while(1) {
       PORTB ^= (1 << PB0);
       _delay_ms(254);
   }

   return 0;
}
