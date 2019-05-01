#include <new>
#include <common.h>
#include <FreeRTOS.h>
#include <task.h>

#define DBG_OUT(lev, fmt, ...) DBG_PRINT(lev, "%s:%04u " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

void* operator new  (size_t count )
{ 
    void *ptr = pvPortMalloc(count);
    DBG_OUT(LOG_DEBUG, "allocate %u bytes at %p", count, ptr);
    return ptr;
}

void* operator new  (size_t count, const std::nothrow_t& tag)
{
    void *ptr = pvPortMalloc(count);
    DBG_OUT(LOG_DEBUG, "allocate %u bytes at %p", count, ptr);
    return ptr;
}

void* operator new[](std::size_t count )
{
    void *ptr = pvPortMalloc(count);
    DBG_OUT(LOG_DEBUG, "allocate %u bytes at %p", count, ptr);
    return ptr;
}

void* operator new[](std::size_t count, const std::nothrow_t& tag)
{
    void *ptr = pvPortMalloc(count);
    DBG_OUT(LOG_DEBUG, "allocate %u bytes at %p", count, ptr);
    return ptr;
}
  
void operator delete  (void* ptr)
{ 
    DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
    vPortFree(ptr);
}

void operator delete[](void* ptr)
{
    DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
    vPortFree(ptr);
}

void operator delete  (void* ptr, const std::nothrow_t& tag)
{
    DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
    vPortFree(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t& tag)
{
    DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
    vPortFree(ptr);
}

void operator delete  (void* ptr, std::size_t sz)
{
    DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
    vPortFree(ptr);
}

void operator delete[](void* ptr, std::size_t sz)
{
    DBG_OUT(LOG_DEBUG, "free memory at %p", ptr); 
    vPortFree(ptr);
}
