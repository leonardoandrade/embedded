idf_component_register(SRCS "main.c" "../sensor/dht22.c" "../network/wifi.c" "../network/http_client.c" 
                    INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
cmake_minimum_required(VERSION 3.16)
