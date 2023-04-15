idf_component_register(SRCS "wifi.c" "http_client.c" 
                    INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
