#include "stdio-task/stdio-task.h"
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "0.0.1"

// Колбэки для команд светодиода
void led_on_callback(const char* args)
{
    printf("LED: включение\n");
    led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args)
{
    printf("LED: выключение\n");
    led_task_state_set(LED_STATE_OFF);
}


void led_blink_set_period_ms_callback(const char* args)
{
    unsigned int period_ms = 0;
    
    // Парсим аргумент как целое число (миллисекунды)
    sscanf(args, "%u", &period_ms);
    
    // Проверяем, что период не равен нулю
    if (period_ms == 0) {
        printf("Ошибка: период мигания должен быть больше 0\n");
        printf("Использование: blink_period <миллисекунды>\n");
        return;
    }
    
    // Вызываем функцию установки периода из LED задачи
    led_task_set_blink_period_ms(period_ms);
    
    printf("Период мигания установлен: %u мс\n", period_ms);
}

void led_blink_callback(const char* args)
{
    printf("LED: мигание\n");
    led_task_state_set(LED_STATE_BLINK);
}

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

api_t device_api[] =
{
    {"setperiod", led_blink_set_period_ms_callback, "period was set"},
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "turn LED on"},
    {"off", led_off_callback, "turn LED off"},
    {"blink", led_blink_callback, "make LED blink"},
    {NULL, NULL, NULL},
};

int main(){
    stdio_task_init();
    stdio_init_all();
    
    // Инициализация LED задачи
    led_task_init();
    
    protocol_task_init(device_api);

    while (1)
    {
       char* cmd = stdio_task_handle();
       protocol_task_handle(cmd);
       
       // Обработка LED задачи в каждом цикле
       led_task_handler();
    }
}