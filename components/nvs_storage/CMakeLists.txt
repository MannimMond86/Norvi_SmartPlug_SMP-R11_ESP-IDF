idf_component_register(SRCS "nvs_storage.cpp"
        INCLUDE_DIRS "include" "."
        REQUIRES driver common_variable_handler esp_event esp_wifi esp_system nvs_flash)
