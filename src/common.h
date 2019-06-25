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
  #include <trace.h>
  
  #define DBG_PRINT(lev, fmt, ...)       trace_printf(lev, fmt, ##__VA_ARGS__)
  #define DBG_INIT()                     trace_init()
  #define DBG_DEINIT()                   trace_deinit()
  #define DBG_CRASH(fmt, ...)            trace_crash(fmt, ##__VA_ARGS__)
#else
  #define DBG_PRINT(lev, fmt, ...)
  #define DBG_INIT()
  #define DBG_DEINIT()
  #define DBG_CRASH(fmt, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif