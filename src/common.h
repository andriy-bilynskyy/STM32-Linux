/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef DEBUG
  #include <SEGGER_RTT.h>
  
  #define DBG_PRINT(fmt, ...)       SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)
#else
  #define DBG_PRINT(fmt, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif