idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver modem_interface uart_forwarder common_variable_handler wifi_handler nvs_flash
                    PRIV_REQUIRES freertos )

# Include ESP-IDF components in the build, may be thought as an equivalent of
# add_subdirectory() but with some additional procesing and magic for ESP-IDF build
# specific build processes.
#idf_build_process(esp32)