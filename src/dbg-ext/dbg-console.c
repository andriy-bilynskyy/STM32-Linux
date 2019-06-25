/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

#include <SEGGER_RTT.h>
#include <printf.h>
#include <ctype.h>
#include <string.h>
#include "trace.h"
#include "rtos-stat.h"


#define CMD_MAX_LEN     64
#define HELLO_CMD       ":~$ "

/* Console commnds description */
static void set_trc_level(const char * level);
static void get_rtos_stat(const char * type);

void vApplicationIdleHook(void)
{
    struct{
        const char * cmd;
        void (*fptr)(const char *);
    }cmd_data[] = {
        /* Describe available commands here */
        {"trace", set_trc_level},
        {"osstat", get_rtos_stat}
    };

    char cmd_buffer[CMD_MAX_LEN];
    unsigned int cmd_buf_ptr = 0;

    for(;;)
    {
        if(!cmd_buf_ptr)
        {
            printf(HELLO_CMD);
        }
        cmd_buffer[cmd_buf_ptr] = SEGGER_RTT_WaitKey();
        SEGGER_RTT_PutChar(0, cmd_buffer[cmd_buf_ptr]);
        if(cmd_buffer[cmd_buf_ptr] == '\n')
        {
            for(; cmd_buf_ptr && isspace(cmd_buffer[cmd_buf_ptr - 1]); cmd_buf_ptr--);
            cmd_buffer[cmd_buf_ptr] = '\0';

            char *cmd = NULL, *arg = NULL;
            unsigned int i = 0;
            for(; cmd_buffer[i] && isspace(cmd_buffer[i]); i++);
            cmd = &cmd_buffer[i];
            for(; cmd_buffer[i] && !isspace(cmd_buffer[i]); i++);
            cmd_buffer[i] = '\0';
            if( i < cmd_buf_ptr) i++;
            for(; cmd_buffer[i] && isspace(cmd_buffer[i]); i++);
            arg = &cmd_buffer[i];

            if(!strcmp(cmd, "help"))
            {
                printf("available commands:\n - help\n");
                for(unsigned int i = 0; i < sizeof(cmd_data)/sizeof(cmd_data[0]); i++)
                {
                    printf(" - %s\n", cmd_data[i].cmd);
                }   
            }else{
                bool cmd_found = false;
                for(unsigned int i = 0; !cmd_found && i < sizeof(cmd_data)/sizeof(cmd_data[0]); i++)
                {
                    if(!strcmp(cmd, cmd_data[i].cmd))
                    {
                        cmd_data[i].fptr(arg);
                        cmd_found = true;
                    }
                }   
                if(!cmd_found)
                {
                    printf("unknown command. use 'help' to list.\n");
                }
            }
            cmd_buf_ptr = 0;
        }else{
            cmd_buf_ptr++;
            if(cmd_buf_ptr == sizeof(cmd_buffer))
            {
                printf("\ncommand is too long.\n");
                cmd_buf_ptr = 0;
            }
        }
    }
}

/* Commands implementation */

static void set_trc_level(const char * level)
{
    if(!strcmp(level, "help"))
    {
        printf("set tracing level.\navailable levels: ");

        for(unsigned int i = 0; trace_get_available_level_str(i); i++)
        {
            printf("%s ", trace_get_available_level_str(i));
        }
        printf("\nuse: trace <level> - to set\nuse: trace show - to see current\n");
    }else if(!strcmp(level, "show")){
        printf("current trace level %s\n", trace_get_level_str());
    }else if(trace_set_level_str(level)){
        printf("trace %s or more important\n", trace_get_level_str());
    }else{
        printf("unknown argument. use 'help' to list.\n");
    }
}

static void get_rtos_stat(const char * type)
{
    if(!strcmp(type, "help"))
    {
        printf("get FreeRTOS runtime tasks statistics. no arguments.\n");
    }else if(!strcmp(type, "")){
        print_tasks_stat();
    }else{
        printf("unknown argument. use 'help' to list.\n");
    }  
}