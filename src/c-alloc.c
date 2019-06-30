#include <stdlib.h>
#include <string.h>
#include <common.h>
#include <FreeRTOS.h>
#include <task.h>

#define DBG_OUT(lev, fmt, ...) DBG_PRINT(lev, "%s:%04u " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

void * malloc(size_t size)
{ 
    void *ptr = pvPortMalloc(size);
    DBG_OUT(LOG_DEBUG, "allocate %u bytes at %p", size, ptr);
    return ptr;
}

void * calloc (size_t num, size_t size)
{ 
    void *ptr = pvPortMalloc(size * num);
    DBG_OUT(LOG_DEBUG, "allocate %u bytes at %p", size * num, ptr);
    if(ptr)
    {
        memset(ptr, 0, size * num);
    }
    return ptr;
}

void * realloc(void * ptr, size_t size)
{
    void *ptr_n = pvPortMalloc(size);
    DBG_OUT(LOG_DEBUG, "allocate %u bytes at %p", size, ptr_n);
    if(ptr_n && ptr)
    {
        memcpy(ptr_n, ptr, size);
        DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
        vPortFree(ptr);
    }
    return ptr_n;
}

void free(void * ptr)
{
    DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
    vPortFree(ptr);
}
