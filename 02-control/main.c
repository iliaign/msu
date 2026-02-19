#include "stdio-task/stdio-task.h"
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "protocol-task/protocol-task.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "0.0.1"


void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
	{NULL, NULL, NULL},
};

int main(){
    stdio_task_init();
    stdio_init_all();
    
    protocol_task_init(device_api);

    while (1)
    {
       char* cmd = stdio_task_handle();
       protocol_task_handle(cmd);
    }
}