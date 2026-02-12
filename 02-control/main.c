#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

const uint LED_PIN = 25;

#define DEVICE_VRSN "0.0.1"


int main(){
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);


    while (true)
    {
            char symbol = getchar();

            switch(symbol){
                        
                case 'e':
	                gpio_put(LED_PIN, true);
	                printf("led enable done\n");
	                break;
                case 'd':
                    gpio_put(LED_PIN, false);
	                printf("led disabled done\n");

                case 'v':
                     printf( "firmware version: %s\n", DEVICE_VRSN);

                default:
                    break;
            }

            printf("received char: %c [ ASCII code: %d ]\n", symbol, symbol);
    }


    

}