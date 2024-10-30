#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "modem_interface.h"
#include "uart_forwarder.h"
#include "common_variable_handler.h"


// Main task to initialize and communicate with modem
void modem_task(void *param) {
    //modem_init();
    bool modem_data_sent = false;
    while(1){
    if ( modem_init_success == true && modem_data_sent == false ) {

        // Example: Get network registration status
        if (modem_check_network_registration() == ESP_OK) {
            ESP_LOGI(TAG_MODEM, "Network registration check successful.");
        } else {
            ESP_LOGE(TAG_MODEM, "Failed to check network registration.");
        }

        // Additional AT commands can be sent similarly...
        modem_data_sent = true;
        vTaskDelete(NULL); // End the task
    }
    }
}


void uart_task(void *pvParameter) {
    uint8_t *data = (uint8_t *) malloc(1024);

    while (1) {
        // Read data from terminal UART
        int len = uart_read_bytes(terminal_uart, data, 1024, 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0';  // Null-terminate the string
            uart_write_bytes(modem_uart, (const char *)data, len);  // Send to modem
            ESP_LOGI(TAG_UART, "Sent to modem: %s", data);
        }

        // Read feedback from modem UART
        len = uart_read_bytes(modem_uart, data, 1024, 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0';  // Null-terminate the string
            uart_write_bytes(terminal_uart, (const char *)data, len);  // Relay to terminal
            ESP_LOGI(TAG_UART, "Received from modem: %s", data);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    free(data);
}


void app_main() {
    vTaskDelay(50 / portTICK_PERIOD_MS);
    uart_init();
    vTaskDelay(50 / portTICK_PERIOD_MS);
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    modem_reset();
    vTaskDelay(50 / portTICK_PERIOD_MS);
    modem_enable();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xTaskCreate(modem_task, "modem_task", 4096, NULL, 5, NULL);
}
