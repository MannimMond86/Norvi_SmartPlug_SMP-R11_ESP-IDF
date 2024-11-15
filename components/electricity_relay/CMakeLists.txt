idf_component_register(SRCS "electricity_relay.c"
        INCLUDE_DIRS "include" "."
        REQUIRES driver common_variable_handler esp_event esp_system)
