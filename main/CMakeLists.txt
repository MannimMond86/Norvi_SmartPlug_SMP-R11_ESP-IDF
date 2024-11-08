set(srcs
        ../components/ThingsBoard/src/Arduino_HTTP_Client.cpp
        ../components/ThingsBoard/src/Arduino_MQTT_Client.cpp
        ../components/ThingsBoard/src/Arduino_ESP32_Updater.cpp
        ../components/ThingsBoard/src/Arduino_ESP8266_Updater.cpp
        ../components/ThingsBoard/src/HashGenerator.cpp
        ../components/ThingsBoard/src/Helper.cpp
        ../components/ThingsBoard/src/OTA_Update_Callback.cpp
        ../components/ThingsBoard/src/Provision_Callback.cpp
        ../components/ThingsBoard/src/RPC_Request_Callback.cpp
        ../components/ThingsBoard/src/Telemetry.cpp
)

set(dependencies
        esp_common
        #bblanchon__arduinojson
        log
        driver
        modem_interface
        uart_forwarder
        common_variable_handler
        wifi_handler
        nvs_flash
        time_manager
)

set(private_dependencies
        freertos
        esp_timer
        app_update
        esp_common
)

idf_component_register(SRCS "main.cpp" ${srcs}
        INCLUDE_DIRS "." "../components/ThingsBoard/src"
        REQUIRES ${dependencies}
        PRIV_REQUIRES ${private_dependencies})

# Include ESP-IDF components in the build, may be thought as an equivalent of
# add_subdirectory() but with some additional procesing and magic for ESP-IDF build
# specific build processes.
#idf_build_process(esp32)