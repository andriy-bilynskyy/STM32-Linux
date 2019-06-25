#include <printf.h>
#include <SEGGER_RTT.h>

void _putchar(char character)
{
    SEGGER_RTT_PutChar(0, character);
}
