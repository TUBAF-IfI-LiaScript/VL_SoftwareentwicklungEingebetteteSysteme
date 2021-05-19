#ifndef USART_RS232_H_FILE_H_				
#define USART_RS232_H_FILE_H_

#include <stdio.h>	
#include <avr/io.h>	

#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)	/* Define prescale value */

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
void uart_init(unsigned long);	

#endif				