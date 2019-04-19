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
#include <common.h>
#include <FreeRTOS.h>
#include <task.h>

#define DBG_OUT(fmt, ...) DBG_PRINT("[%08u] %s:%04u " fmt "\n", xTaskGetTickCount(), __FILE__, __LINE__, ##__VA_ARGS__)

void vTask1(void *pvParameters)
{
    DBG_OUT("Task 1 start");

    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0;

    for(;;)
    {
        DBG_OUT("Task 1");
        GPIOInvBit(GPIOC, 13);
        vTaskDelay(500);
    }

    DBG_OUT("Task 1 stop\n");

    vTaskDelete(NULL);
}

void vTask2(void *pvParameters)
{
    DBG_OUT("Task 2 start");

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    GPIOB->CRH &= ~(GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
    GPIOB->CRH |= GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0;

    for(;;)
    {
        DBG_OUT("Task 2");
        GPIOInvBit(GPIOB, 12);
        vTaskDelay(100);
    }

    DBG_OUT("Task 2 stop");

    vTaskDelete(NULL);
}

int main(void)
{
    DBG_OUT("main start");
    (void)xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    (void)xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
}
