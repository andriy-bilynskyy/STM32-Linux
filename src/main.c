/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#include <stm32f10x.h>

void sleep_ms(unsigned int ms)
{
    while(ms--)
    {
        volatile unsigned int tmp = 5971u;
        while(tmp--)
        {
            __asm("nop");
        }
    }
}

unsigned int delay_ms = 500;

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1 | GPIO_CRH_CNF13_0;

    for(;;)
    {    
        sleep_ms(delay_ms);
        GPIOC->BSRR = GPIO_BSRR_BS13;
        
        sleep_ms(delay_ms);
        GPIOC->BRR = GPIO_BRR_BR13;
    }
}
