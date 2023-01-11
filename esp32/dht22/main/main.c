#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "../vendor/dht22.h"

#define DHT22_PIN 33

void app_main()
{
    printf("DHT Reading started\n\n");
    setDHTgpio(DHT22_PIN);

    int count = 0;
    while (1)
    {
        count++;

        printf("## READING %d:\n", count);
        int ret = readDHT();

        errorHandler(ret);

        printf("Temperature: %.1f\n", getTemperature());
        printf("Humidity:    %.1f\n", getHumidity());

        vTaskDelay(100);
    }
}