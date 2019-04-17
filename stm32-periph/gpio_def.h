/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#ifndef __STM32_GPIO_DEF_H
#define __STM32_GPIO_DEF_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif 


#define GPIOSetBit(PORT, PIN)                   \
    PORT->BSRR = ((uint32_t)1 << PIN);

#define GPIOClrBit(PORT, PIN)                   \
    PORT->BRR = ((uint32_t)1 << PIN);

#define GPIOInvBit(PORT, PIN)                   \
    do{                                         \
        if(PORT->ODR & ((uint32_t)1 << PIN))    \
        {                                       \
            PORT->BRR = ((uint32_t)1 << PIN);   \
        }else{                                  \
            PORT->BSRR = ((uint32_t)1 << PIN);  \
        }                                       \
    }while(0)

#ifdef __cplusplus
}
#endif

#endif
