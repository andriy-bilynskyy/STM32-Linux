/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#include <stm32f10x_conf.h>
#include <common.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdbool.h>

#define DBG_OUT(lev, fmt, ...) DBG_PRINT(lev, " %s:%04u " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

void vTask1(void *pvParameters)
{
    DBG_OUT(LOG_DEBUG, "Task 1 start");

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    bool value = false;
    for(;;)
    {
        DBG_OUT(LOG_INFO, "Task 1");
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, value);
        value = !value;
        vTaskDelay(500);
    }

    DBG_OUT(LOG_DEBUG, "Task 1 stop\n");

    vTaskDelete(NULL);
}

void vTask2(void *pvParameters)
{
    DBG_OUT(LOG_DEBUG, "Task 2 start");

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    bool value = false;
    for(;;)
    {
        DBG_OUT(LOG_INFO, "Task 2");
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, value);
        value = !value;
        vTaskDelay(100);
    }

    DBG_OUT(LOG_DEBUG, "Task 2 stop");

    vTaskDelete(NULL);
}

int main(void)
{
    DBG_INIT();
    DBG_OUT(LOG_DEBUG, "main start");

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    (void)xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    (void)xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    DBG_OUT(LOG_CRITICAL, "[YOUR BUNNY WROTE]");
    DBG_DEINIT();
    for(;;);
}

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
{
    (void)pxTask;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);

    DBG_PRINT(LOG_CRITICAL, "[STACK OVERFLOW] task: %s\n", pcTaskName);
}

void assert_failed(uint8_t* file, uint32_t line)
{
    DBG_PRINT(LOG_CRITICAL, "[ASSERT FAILED AT] %s:%04u\n", file, line);
    for(;;);
}

void SysClockFailed(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);

    DBG_PRINT(LOG_CRITICAL, "[SYSTEM CLOCK FAILED]\n");
    for(;;); 
}

__attribute__((naked)) void HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                    \n"
        " ite eq                        \n"
        " mrseq r0, msp                 \n"
        " mrsne r0, psp                 \n"
        " bl prvGetRegistersFromStack   \n"
    );
    DBG_PRINT(LOG_CRITICAL, "[HARD FAULT]\n");
    for(;;);
}

void prvGetRegistersFromStack(unsigned int * pStack)
{
    DBG_PRINT(LOG_CRITICAL, "[CORE DUMP]\n");
    DBG_PRINT(LOG_CRITICAL, "R0  = %08x\n", pStack[0]);
    DBG_PRINT(LOG_CRITICAL, "R1  = %08x\n", pStack[1]);
    DBG_PRINT(LOG_CRITICAL, "R2  = %08x\n", pStack[2]);
    DBG_PRINT(LOG_CRITICAL, "R3  = %08x\n", pStack[3]);
    DBG_PRINT(LOG_CRITICAL, "R12 = %08x\n", pStack[4]);
    DBG_PRINT(LOG_CRITICAL, "LR  = %08x\n", pStack[5]);
    DBG_PRINT(LOG_CRITICAL, "PC  = %08x\n", pStack[6]);
    DBG_PRINT(LOG_CRITICAL, "PSR = %08x\n", pStack[7]);
}
