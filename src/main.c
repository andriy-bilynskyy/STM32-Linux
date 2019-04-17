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
#include <gpio_def.h>
#include <FreeRTOS.h>
#include <task.h>

void vTask1(void *pvParameters)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0;

    for(;;)
    {
        GPIOInvBit(GPIOC, 13);
        vTaskDelay(500);
    }
    vTaskDelete(NULL);
}

void vTask2(void *pvParameters)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    GPIOB->CRH &= ~(GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
    GPIOB->CRH |= GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0;

    for(;;)
    {
        GPIOInvBit(GPIOB, 12);
        vTaskDelay(100);
    }
    vTaskDelete(NULL);
}

int main(void)
{
    (void)xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    (void)xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
}
