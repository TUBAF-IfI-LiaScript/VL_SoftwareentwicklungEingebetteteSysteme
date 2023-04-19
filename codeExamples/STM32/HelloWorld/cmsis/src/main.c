#include "stm32f4xx.h"
#include <stdbool.h>

void ms_delay(int ms)
{
   while (ms-- > 0) {
      volatile int x=500;
      while (x-- > 0)
         __asm("nop");
   }
}

// Version 1: Basic Register Usage

int main(void){
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
   GPIOA->MODER |= GPIO_MODER_MODER5_0; 
   while(true){
      ms_delay(1500);
      GPIOA->ODR ^= GPIO_BSRR_BS5;
   }
}

// Version 2: STM Macros for Bit manipulation

//int main(void)
//{
//   SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
//   MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER5,  GPIO_MODER_MODER5_0);
//
//   while(true){
//      ms_delay(500);
//      WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS_5);
//      ms_delay(500);
//      WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BR_5);
//   }
//
//   return 0;
//}