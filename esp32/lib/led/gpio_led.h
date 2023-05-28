
#ifndef GPIO_LED_H
#define GPIO_LED_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>


// Turn on the LED
void gpio_led_on(gpio_num_t gpio_num);

// Turn off the LED
void gpio_led_off(gpio_num_t gpio_num);

// Flash the LED with a given delay
void flash_led(gpio_num_t gpio_num, int delay_ms);

#endif /* GPIO_LED_H */



