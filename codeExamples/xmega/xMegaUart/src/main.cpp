#define F_CPU 2666666
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char *str);

void USART3_init(void)
{
    // Configure PB4 - 3,TxD -  as output 
    PORTB.DIRSET = PIN4_bm;
    // Configure PB5 - 3,RxD -  as input
    PORTB.DIRCLR = PIN5_bm;
    // Choose alternative pins for USART3
    PORTMUX.USARTROUTEA |= PORTMUX_USART30_bm;

    USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
    // Enable Transmission
    USART3.CTRLB |= USART_TXEN_bm; 
}

void USART3_sendChar(char c)
{
    while (!(USART3.STATUS & USART_DREIF_bm))
    {
        ;
    }        
    USART3.TXDATAL = c;
}

void USART3_sendString(char *str)
{
    for(size_t i = 0; i < strlen(str); i++)
    {
        USART3_sendChar(str[i]);
    }
}

int main(void)
{
    USART3_init();
    USART3_sendString("Hello World!\r\n");
    PORTD.DIRSET = PIN6_bm;
    PORTD.OUTSET = PIN6_bm;
    while (1) 
    {
        _delay_ms(500);
        PORTD.OUTTGL = PIN6_bm;
    }
}