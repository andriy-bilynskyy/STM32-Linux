#include <stm32f10x_conf.h>
#include <common.h>
#include <vector>
#include <FreeRTOS.h>
#include <task.h>

#define DBG_OUT(lev, fmt, ...) DBG_PRINT(lev, " %s:%04u " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

extern "C" void vTask3(void *pvParameters)
{
    DBG_OUT(LOG_DEBUG, "Task 3 start");

    {
        std::vector<int> v;
        for(unsigned int i = 0; i < 5; i++)
        {
             v.push_back(157 + i);
        }
        for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
        {
            DBG_OUT(LOG_DEBUG, "[]=%d", *it);
        }
    }

    DBG_OUT(LOG_DEBUG, "Task 3 stop");

    vTaskDelete(NULL);
}
