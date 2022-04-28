#include <stdio.h>
#include <math.h>
#include <avr/io.h>

#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART0Init(void);
int USART0SendByte(char u8Data, FILE *stream);
int USART0ReceiveByte(FILE *stream);
