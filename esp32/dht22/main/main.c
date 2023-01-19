#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_wifi.h"

#include "./WiFiCredentials.h"
#include "../vendor/dht22.h"

#define DHT22_PIN 33

static int ERROR_LED = 14;

void connect_wifi()
{

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PW,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );
    
}

void enable_error_led()
{
    gpio_set_level(ERROR_LED, true);
}

void disable_error_led()
{
    gpio_set_level(ERROR_LED, false);
}

void app_main()
{

    gpio_reset_pin(ERROR_LED);
    gpio_set_direction(ERROR_LED, GPIO_MODE_OUTPUT);

    printf("DHT Reading started\n\n");
    setDHTgpio(DHT22_PIN);

    connect_wifi();


    int count = 0;
    while (1)
    {
        count++;

        printf("## READING %d:\n", count);
        int ret = readDHT();
        errorHandler(ret);

        if (ret != DHT_OK)
        {
            enable_error_led();
        }
        else
        {
            disable_error_led();
        }

        printf("Temperature: %.1f\n", getTemperature());
        printf("Humidity:    %.1f\n", getHumidity());

        vTaskDelay(100);
    }
}