// Demoprojekt für die Nutzung von Arduino AVR Boards mit Platfomio ohne eine 
// Einbindung von Arduino.h.

//#define F_CPU 16000000UL            // im Hintergrund bereits definiert								
#define BAUD 9600

#include <avr/io.h>		
#include <stdio.h>								
#include <util/delay.h>						
#include "uart.h"								    

int main(void)
{
    DDRB = 1 << 5;

	uart_init(BAUD);	
	printf("Los geht's\n");
	
	float f = 42.0;
    int i = 42;

    while (1)
    {
        _delay_ms(500);
        PORTB ^= 1 << 5;
		printf("Testlauf %f", f++);  // Achtung hierfür muss eine Bibliothek 
                                     // eingebunden werden, die die Funktion printf() unterstützt.
                                     // Siehe platformio.ini File
        printf(", %d\n", i++);       // Ganzzahlen brauchen diese nicht.
    }
    return 0;
}