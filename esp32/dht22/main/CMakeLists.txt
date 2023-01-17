idf_component_register(SRCS "main.c" "../vendor/dht22.c"
                    INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
cmake_minimum_required(VERSION 3.16)
