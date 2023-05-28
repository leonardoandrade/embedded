#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "rom/ets_sys.h"

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"


// Generic libraries
#include "../../lib/sensor/dht22.h"
#include "../../lib/network/wifi.h"
#include "../../lib/network/http_client.h"
#include "../../lib/led/gpio_led.h"


// Change according to the sensor you are using
#include "./definitions_sensor_1.h"



void app_main()
{

    printf("DHT Reading started\n\n");
    setDHTgpio(DHT22_PIN);
    
    connect_wifi(WIFI_SSID, WIFI_PW);
    
    scan_wifi_ap(WIFI_SSID);

    int count = 0;
    while (1)
    {
        count++;

        printf("## READING %d:\n", count);
        int ret = readDHT();
        errorHandler(ret);

        if (ret != DHT_OK)
        {
            gpio_led_on(ERROR_LED);
        }
        else
        {
            gpio_led_off(ERROR_LED);
            flash_led(OK_LED, 500);
        }

        printf("Temperature: %.1f\n", getTemperature());
        printf("Humidity:    %.1f\n", getHumidity());



        if(is_connected())
        {

            //TODO: send signal strength
            scan_wifi_ap(WIFI_SSID);
            
            send_message(INFLUX_HOST, INFLUX_AUTH,  "temperature", getTemperature());
            send_message(INFLUX_HOST, INFLUX_AUTH, "humidity",    getHumidity());
        }

        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}