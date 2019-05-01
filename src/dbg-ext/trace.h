/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#ifndef __TRACE_H
#define __TRACE_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FOREACH_DBGLEV(DBG_LEV) \
    DBG_LEV(LOG_INFO)           \
    DBG_LEV(LOG_DEBUG)          \
    DBG_LEV(LOG_WARNING)        \
    DBG_LEV(LOG_ERROR)          \
    DBG_LEV(LOG_CRITICAL)       \

#define GENERATE_ENUM(ENUM) ENUM,

typedef enum{
    FOREACH_DBGLEV(GENERATE_ENUM)
}LOG_level_t;

#undef GENERATE_ENUM

void trace_init(void);
void trace_deinit(void);
void trace_printf(LOG_level_t level, const char * sFormat, ...);
void trace_show_buf(LOG_level_t level, void * data, unsigned int size, const char *comment, ...);
void trace_show_buflong(LOG_level_t level, void * data, unsigned int size, const char *comment, ...);
void trace_set_level(LOG_level_t level);
LOG_level_t trace_get_level(void);
bool trace_set_level_str(const char * level);
const char * trace_get_level_str();
const char * trace_get_available_level_str(unsigned int index);

#ifdef __cplusplus
}
#endif

#endif