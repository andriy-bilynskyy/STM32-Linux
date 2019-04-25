/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#include "rtos-stat.h"
#include <SEGGER_RTT.h>
#include <stm32f10x_conf.h>
#include <FreeRTOS.h>
#include <task.h>

#define RTOS_STAT_TIMER_HZ (20 * 1000)

void print_tasks_stat(void)
{
    UBaseType_t task_cnt = uxTaskGetNumberOfTasks();
    TaskStatus_t * tasks_stat = pvPortMalloc(task_cnt * sizeof(TaskStatus_t));
    if(tasks_stat)
    {
        unsigned long run_time;
        task_cnt = uxTaskGetSystemState(tasks_stat, task_cnt, &run_time);
        SEGGER_RTT_printf(0, "available %u task(s)\n", task_cnt);
        if(run_time && task_cnt)
        {
            SEGGER_RTT_WriteString(0, "ID   State      P   PI  Time        Stack       %     Name\n");
            for(UBaseType_t i = 0; i < task_cnt; i++)
            {
                static const char * task_states[] = {"Running  ", "Ready    ", "Blocked  ", "Suspended", "Deleted  ", "Invalid  "};
                SEGGER_RTT_printf(0, "%-3u  ", tasks_stat[i].xTaskNumber);
                UBaseType_t percentage = (unsigned long long)tasks_stat[i].ulRunTimeCounter * 1000 / run_time;
                SEGGER_RTT_WriteString(0, task_states[tasks_stat[i].eCurrentState]);
                SEGGER_RTT_printf(0, "  %-2u  %-2u  %-10u  %-10u  %2u.%1u  ", tasks_stat[i].uxBasePriority,
                                                                              tasks_stat[i].uxCurrentPriority,
                                                                              tasks_stat[i].ulRunTimeCounter,
                                                                              tasks_stat[i].usStackHighWaterMark,
                                                                              percentage/10, percentage%10);
                SEGGER_RTT_WriteString(0, tasks_stat[i].pcTaskName);
                SEGGER_RTT_PutChar(0, '\n');
            }
        }
        SEGGER_RTT_printf(0, "total time: %u sec\n", run_time / RTOS_STAT_TIMER_HZ);
        vPortFree(tasks_stat);
    }else{
        SEGGER_RTT_printf(0, "available %u task(s)\n", task_cnt);
        SEGGER_RTT_WriteString(0, "can't allocate memory for task stat.\n");
    }
    SEGGER_RTT_printf(0, "current free heap size: %u\nminimal free heap size: %u\n", xPortGetFreeHeapSize(),
                                                                                     xPortGetMinimumEverFreeHeapSize());
}

void RtosStatTimerInit(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM_TimeBaseInitTypeDef timer_base;
    TIM_TimeBaseStructInit(&timer_base);
    timer_base.TIM_Prescaler = (SystemCoreClock / RTOS_STAT_TIMER_HZ) - 1u;
    TIM_TimeBaseInit(TIM3, &timer_base);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

static volatile uint16_t timer = 0;

uint32_t RtosStatTimerGet(void)
{
    TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
    uint32_t result = ((uint32_t)timer << 16u) | TIM3->CNT;
    TIM3->CR1 |= TIM_CR1_CEN;
    return result;
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        timer++;
    }
}
