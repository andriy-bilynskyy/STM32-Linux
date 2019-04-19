/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#include "startup.h"
#include <string.h>
#include <stdbool.h>
#include "stm32f10x.h"

#define F_HSI_HZ 8000000u

extern int main(void);
#ifdef DEBUG
extern void SEGGER_RTT_Init(void);
#endif
static bool HSE_ON = false;
static void HSE_Fail(void);

uint32_t SystemCoreClock = F_HSI_HZ;

extern const char _sidata;
extern char _sdata;
extern char _edata;
extern char _sbss;
extern char _ebss;
extern char _estack;

void Reset_Handler(void)
{
    memcpy(&_sdata, &_sidata, &_edata - &_sdata);
    memset(&_sbss, 0, &_ebss - &_sbss);
#ifdef DEBUG
    SEGGER_RTT_Init();
#endif
    SystemInit();
    if(HSE_ON)
    {
        (void)main();
    }
    else
    {
        HSE_Fail();
    }
    for(;;);
}

void Dummy_Handler(void)
{
    /* Dummy interrupt handler */
}

void NMI_Handler(void)                  __attribute__ ((weak, alias ("Dummy_Handler")));
void HardFault_Handler(void)            __attribute__ ((weak, alias ("Dummy_Handler")));
void MemManage_Handler(void)            __attribute__ ((weak, alias ("Dummy_Handler")));
void BusFault_Handler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void UsageFault_Handler(void)           __attribute__ ((weak, alias ("Dummy_Handler")));
void SVC_Handler(void)                  __attribute__ ((weak, alias ("Dummy_Handler")));
void DebugMon_Handler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void PendSV_Handler(void)               __attribute__ ((weak, alias ("Dummy_Handler")));
void SysTick_Handler(void)              __attribute__ ((weak, alias ("Dummy_Handler")));
void WWDG_IRQHandler(void)              __attribute__ ((weak, alias ("Dummy_Handler")));
void PVD_IRQHandler(void)               __attribute__ ((weak, alias ("Dummy_Handler")));
void TAMPER_IRQHandler(void)            __attribute__ ((weak, alias ("Dummy_Handler")));
void RTC_IRQHandler(void)               __attribute__ ((weak, alias ("Dummy_Handler")));
void FLASH_IRQHandler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void RCC_IRQHandler(void)               __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI0_IRQHandler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI1_IRQHandler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI2_IRQHandler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI3_IRQHandler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI4_IRQHandler(void)             __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Channel1_IRQHandler(void)     __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Channel2_IRQHandler(void)     __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Channel3_IRQHandler(void)     __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Channel4_IRQHandler(void)     __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Channel5_IRQHandler(void)     __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Channel6_IRQHandler(void)     __attribute__ ((weak, alias ("Dummy_Handler")));
void DMA1_Channel7_IRQHandler(void)     __attribute__ ((weak, alias ("Dummy_Handler")));
void ADC1_2_IRQHandler(void)            __attribute__ ((weak, alias ("Dummy_Handler")));
void USB_HP_CAN1_TX_IRQHandler(void)    __attribute__ ((weak, alias ("Dummy_Handler")));
void USB_LP_CAN1_RX0_IRQHandler(void)   __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN1_RX1_IRQHandler(void)          __attribute__ ((weak, alias ("Dummy_Handler")));
void CAN1_SCE_IRQHandler(void)          __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI9_5_IRQHandler(void)           __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_BRK_IRQHandler(void)          __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_UP_IRQHandler(void)           __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_TRG_COM_IRQHandler(void)      __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM1_CC_IRQHandler(void)           __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM2_IRQHandler(void)              __attribute__ ((weak, alias ("Dummy_Handler")));
void TIM3_IRQHandler(void)              __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C1_EV_IRQHandler(void)           __attribute__ ((weak, alias ("Dummy_Handler")));
void I2C1_ER_IRQHandler(void)           __attribute__ ((weak, alias ("Dummy_Handler")));
void SPI1_IRQHandler(void)              __attribute__ ((weak, alias ("Dummy_Handler")));
void USART1_IRQHandler(void)            __attribute__ ((weak, alias ("Dummy_Handler")));
void USART2_IRQHandler(void)            __attribute__ ((weak, alias ("Dummy_Handler")));
void EXTI15_10_IRQHandler(void)         __attribute__ ((weak, alias ("Dummy_Handler")));
void RTCAlarm_IRQHandler(void)          __attribute__ ((weak, alias ("Dummy_Handler")));
void USBWakeUp_IRQHandler(void)         __attribute__ ((weak, alias ("Dummy_Handler")));
void BootRAM(void)                      __attribute__ ((weak, alias ("Dummy_Handler")));

void (* const NVIC_VectorTable[])(void)  __attribute__((section(".isr_vector"))) = 
{
    (void (*)(void))&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    (void (*)(void))0,
    (void (*)(void))0,
    (void (*)(void))0,
    (void (*)(void))0,
    SVC_Handler,
    DebugMon_Handler,
    (void (*)(void))0,
    PendSV_Handler,
    SysTick_Handler,
    WWDG_IRQHandler,
    PVD_IRQHandler,
    TAMPER_IRQHandler,
    RTC_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_IRQHandler,
    DMA1_Channel3_IRQHandler,
    DMA1_Channel4_IRQHandler,
    DMA1_Channel5_IRQHandler,
    DMA1_Channel6_IRQHandler,
    DMA1_Channel7_IRQHandler,
    ADC1_2_IRQHandler,
    USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler,
    TIM1_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    (void (*)(void))0,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    (void (*)(void))0,
    (void (*)(void))0,
    SPI1_IRQHandler,
    (void (*)(void))0,
    USART1_IRQHandler,
    USART2_IRQHandler,
    (void (*)(void))0,
    EXTI15_10_IRQHandler,
    RTCAlarm_IRQHandler,
    USBWakeUp_IRQHandler,    
    (void (*)(void))0,
    (void (*)(void))0,
    (void (*)(void))0,
    (void (*)(void))0,
    (void (*)(void))0,
    (void (*)(void))0,
    (void (*)(void))0,
    BootRAM
};

void SystemInit(void)
{
    /* reset clock state */
    RCC->CR |= RCC_CR_HSION;
    RCC->CFGR &= ~(RCC_CFGR_SW | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_ADCPRE | RCC_CFGR_MCO);
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON);
    RCC->CR &= ~(RCC_CR_HSEBYP);
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL | RCC_CFGR_USBPRE);
    RCC->CIR = RCC_CIR_LSIRDYC | RCC_CIR_LSERDYC | RCC_CIR_HSIRDYC | RCC_CIR_HSERDYC | RCC_CIR_PLLRDYC | RCC_CIR_CSSC;
    SystemCoreClock = F_HSI_HZ;
    /* interrupt table at flash begin */
    SCB->VTOR = FLASH_BASE;
    /* clock setup */
    SystemCoreClockUpdate();
}

void SystemCoreClockUpdate(void)
{
    RCC->CR |= RCC_CR_HSEON;
    for(unsigned int start_cnt = HSE_STARTUP_TIMEOUT; start_cnt && !HSE_ON; start_cnt--)
    {
        HSE_ON = (bool)(RCC->CR & RCC_CR_HSERDY);
    }
    if(HSE_ON)
    {
        SystemCoreClock = F_HSE_HZ;

        FLASH->ACR |= FLASH_ACR_PRFTBE;
        FLASH->ACR &= ~(FLASH_ACR_LATENCY);
        FLASH->ACR |= FLASH_ACR_LATENCY_2;

        RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
        RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

        RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
        RCC->CFGR |= RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9;
        RCC->CR |= RCC_CR_PLLON;
        for(; !(RCC->CR & RCC_CR_PLLRDY););

        RCC->CFGR &= ~RCC_CFGR_SW;
        RCC->CFGR |= RCC_CFGR_SW_PLL;

        for(; (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1;);

        SystemCoreClock = F_HSE_HZ * 9u;
    }
}

static void HSE_Fail(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0;

    GPIOC->BRR = GPIO_BRR_BR13;
    for(;;);
}