#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"



#define LED_GPIO 8

void app_main(void)
{

 gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


    bool state = true;
    while (1) {
    gpio_set_level(BLINK_GPIO, state);
    state = !state;

    }
}

