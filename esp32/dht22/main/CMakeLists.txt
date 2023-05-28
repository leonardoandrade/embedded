idf_component_register(SRCS "main.c" "../../lib/sensor/dht22.c" "../../lib/network/wifi.c" "../../lib/network/http_client.c" "../../lib/led/gpio_led.c" 
                    INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
cmake_minimum_required(VERSION 3.16)
