/*
 * ATmega16 Interface with MPU-6050
 * http://www.electronicwings.com
 *
 */ 

#define F_CPU 16000000UL								/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <util/delay.h>									/* Include delay header file */
#include <inttypes.h>									/* Include integer type header file */
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "I2C_Master.h"						        	/* Include I2C Master header file */
#include "uart.h"								     	/* Include USART header file */

#define BAUD 9600

int main()
{
	I2C_Init();											/* Initialize I2C */
	uart_init(BAUD);                                  	/* Initialize USART with 9600 baud rate */
	printf("Initialisierung abgeschlossen ....");

	int x = 0;						 		
	while(1)
	{
        I2C_Start_Wait(0x10);							/* Start with device write address */
		I2C_Write(x);						          	/* Write to sample rate register */						
		I2C_Stop();
		x++; 
  		if (x > 5) x = 0; 
		printf("Initialisierung abgeschlossen .... %d", x);
	}
}
