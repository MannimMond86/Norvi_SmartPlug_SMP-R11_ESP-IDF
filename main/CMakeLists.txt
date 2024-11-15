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
        esp_lcd
        esp_timer
        mqtt
        #bblanchon__arduinojson
        log
        driver
        freertos
        app_update
        nvs_flash
)

set(private_dependencies
        ADE9153
        common_variable_handler
        display_ssd1306
        electricity_relay
        fatfs_storage
        led_ws2812
        modem_interface
        mqtt_interface
        nvs_storage
        ThingsBoard
        time_manager
        touch_button
        uart_forwarder
        wifi_handler

)

idf_component_register(SRCS "main.cpp" ${srcs}
        INCLUDE_DIRS "." "../components/ThingsBoard/src"
        REQUIRES ${dependencies}
        PRIV_REQUIRES ${private_dependencies})

# Include ESP-IDF components in the build, may be thought as an equivalent of
# add_subdirectory() but with some additional procesing and magic for ESP-IDF build
# specific build processes.
#idf_build_process(esp32)