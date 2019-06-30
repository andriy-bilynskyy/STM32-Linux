#include <printf.h>

#undef printf
#undef sprintf
#undef snprintf
#undef vsnprintf
#undef vprintf

int printf(const char * format, ...)
{
    va_list arg;
    va_start(arg, format);
    int result = vprintf_(format, arg);
    va_end(arg);
    return result;
}

int vprintf(const char * format, va_list arg)
{
    return vprintf_(format, arg);
}

int sprintf(char * str, const char * format, ...)
{
    va_list arg;
    va_start(arg, format);
    int result = vsnprintf_(str, (size_t)-1, format, arg);
    va_end(arg);
    return result;
}

int vsprintf(char * str, const char * format, va_list arg)
{
    return vsnprintf_(str, (size_t)-1, format, arg);
}

int snprintf(char * str, size_t n, const char * format, ...)
{
    va_list arg;
    va_start(arg, format);
    int result = vsnprintf_(str, n, format, arg);
    va_end(arg);
    return result;
}

int vsnprintf(char * str, size_t n, const char * format, va_list arg )
{
    return vsnprintf_(str, n, format, arg);
}
