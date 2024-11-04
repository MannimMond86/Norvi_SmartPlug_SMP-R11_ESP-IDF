idf_component_register(SRCS "wifi_handler.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver common_variable_handler esp_event esp_wifi esp_system nvs_flash)
