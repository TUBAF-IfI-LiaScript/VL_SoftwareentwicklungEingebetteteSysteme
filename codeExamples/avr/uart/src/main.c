#define F_CPU 16000000UL								
#define BAUD 9600

#include <avr/io.h>		
#include <stdio.h>								
#include <util/delay.h>						
#include "uart.h"								    

#define BAUD 9600

int main()
{
	uart_init(BAUD);			 		
	printf("Testlauf\n");
	printf("Testlauf %d", 55);
}
