#define F_CPU 2666666
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

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
    for (size_t i = 0; i < strlen(str); i++)
    {
        USART3_sendChar(str[i]);
    }
}

void PIN_init()
{
    // Arduino PIN A0
	/* Disable digital input buffer */
    PORTD.PIN0CTRL &= ~PORT_ISC_gm;
    PORTD.PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc;
    
    /* Disable pull-up resistor */
    PORTD.PIN0CTRL &= ~PORT_PULLUPEN_bm;
}

void ADC0_init()
{
    ADC0.CTRLC = ADC_PRESC_DIV4_gc      /* CLK_PER divided by 4 */
               | ADC_REFSEL_VDDREF_gc;  /* Internal reference */
    
    ADC0.CTRLA = ADC_ENABLE_bm          /* ADC Enable: enabled */
               | ADC_RESSEL_8BIT_gc;    /* 8-bit mode */
    
}

int ADC_read(uint8_t channel, bool accum)
{
    /* Select ADC channel */
    //ADC0.MUXPOS  = ADC_MUXPOS_AIN0_gc;
    ADC0.MUXPOS = channel;
    if (accum == true) ADC0.CTRLB = ADC_SAMPNUM_ACC64_gc;

    // Trigger an ADC conversion
    ADC0_COMMAND |= ADC_STCONV_bm;

    // Wait for it to finish. Alternatively, use the interrupt flag RESRDY in ADC0_INTFLAGS.
    while (ADC0_COMMAND & ADC_STCONV_bm);

    // Read result
    char result = 0;
    result = ADC0_RESL;

    return result;
}

int main(void)
{
    PIN_init();
    ADC0_init();
    USART3_init();
    uint8_t result_A0, result_A1;
    while (1)
    {
        result_A0 = ADC_read(0, false);
        result_A1 = ADC_read(0, true);
        char buffer[10];
        itoa(result_A0, buffer, 10);
        USART3_sendString(buffer);
        USART3_sendChar(',');
        itoa(result_A1, buffer, 10);
        USART3_sendString(buffer);
        USART3_sendChar('\n');
        _delay_ms(100);
    }
}