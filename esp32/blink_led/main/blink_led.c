#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"



static int LED_GPIO = 33;

int app_main()
{

    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    bool state = true;

    while (1) {
        gpio_set_level(LED_GPIO, state);
        state = !state;
        vTaskDelay(100);
    }
}