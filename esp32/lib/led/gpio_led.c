
#include "./gpio_led.h"

static const char *TAG = "led library";


typedef struct
{
    gpio_num_t gpio_num;
    int delay_ms;
} FlashParams;

void gpio_led_on(gpio_num_t gpio_num)
{
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num, 1);
}

void gpio_led_off(gpio_num_t gpio_num)
{
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num, 0);
}

void _flash_led_detached_thread(void *parameter)
{
    FlashParams *params = (FlashParams *)parameter;
    gpio_num_t gpio_num = params->gpio_num;
    int delay_ms = params->delay_ms;
    ESP_LOGI(TAG, "starting..gpio=%d delay = %d ms\n",gpio_num,delay_ms);

    gpio_set_level(gpio_num, 1);
    ESP_LOGI(TAG, "LED %d ON\n", gpio_num);
    vTaskDelay(delay_ms / portTICK_PERIOD_MS);

    gpio_set_level(gpio_num, 0);
    ESP_LOGI(TAG, "LED %d OFF\n", gpio_num);

    free(params);
    vTaskDelete(NULL);
}

void flash_led(gpio_num_t gpio_num, int delay_ms)
{
    FlashParams* params = (FlashParams*)malloc(sizeof(FlashParams));
    params->gpio_num = gpio_num;
    params->delay_ms = delay_ms;

    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    //_flash_led_detached_thread((void*)params);

    // Detached thread, so that does not wait for the LED to go off to continue
    xTaskCreate(_flash_led_detached_thread, "_flash_led_detached_thread_task", configMINIMAL_STACK_SIZE, (void*)params, configMAX_PRIORITIES - 1, NULL);


}
