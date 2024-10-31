/***********************************************************************************************
* FILENAME       : main.cpp
 * VERSION        : 0.0.1
 * DESCRIPTION    : This code defines an ESP32 application that manages modem operations and facilitates UART communication between a terminal and the modem using FreeRTOS tasks.
 *
 * PUBLIC FUNCTIONS:
 *    - mode_task()
 *    - uart_task()
 *    - app_main()
 *
 * NOTES:
 *    Prefixes denote variable or return types:
 *    - "c" for char, "s" for short, "v" for void, "u" for unsigned, and "x" for non-standard types.
 *
 * AUTHOR         : Tjark Ziehm
 * START DATE     : July 6, 2024
 * COPYRIGHT      : © 2024 Tjark Ziehm
 * LICENCE        : CC BY-NC-ND 4.0
 * VERSION FORMAT : <major>.<minor>.<patch>
 *
 * Contributors   :
 *
 * CHANGE LOG:
 *    - v0.0.1:
 *    - v0.0.2:
 *    - v0.1.0:
 *    - v1.0.0:
 ***********************************************************************************************/

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
#include "sdkconfig.h"


/**
 * @brief Task to manage modem operations including initializing the network APN and checking network registration.
 *
 * This task begins by adding the network APN to the modem. If successful, it proceeds to check the network registration status.
 * Once the network is registered, the task will send initial data and end itself. It continuously loops with small delays to ensure the operations are completed sequentially.
 *
 * @param param Pointer to the parameters passed to the task (not used in this function).
 */
void modem_task(void *param) {
    //modem_init();
    bool apn_defined = false;
    bool modem_data_sent = false;
    bool modem_received_firmware_version = true;   //default:false
    bool modem_received_revision_version = true;   //default:false
    bool modem_received_software_version = true;   //default:false

    while (1) {

        if (modem_received_software_version== false) {
            modem_request_product_serial_number();
            if (modem_get_revision() == ESP_OK) {
                ESP_LOGI(TAG_MODEM, "Received software version.");
                modem_received_software_version  = true;
            }else {
                ESP_LOGE(TAG_MODEM, "Failed to get firmware version.");
            }
        }

        if (modem_received_revision_version== false) {
            modem_get_revision();
            if (modem_get_revision() == ESP_OK) {
                ESP_LOGI(TAG_MODEM, "Received revision version.");
                modem_received_revision_version = true;
            }else {
                ESP_LOGE(TAG_MODEM, "Failed to get firmware version.");
            }
        }

        if (modem_received_firmware_version == false) {
            modem_get_firmware_version();
            if (modem_get_firmware_version() == ESP_OK) {
                ESP_LOGI(TAG_MODEM, "Received firmware version.");
                modem_received_firmware_version = true;
            }else {
                ESP_LOGE(TAG_MODEM, "Failed to get firmware version.");
            }
        }

        if (apn_defined == false) {
            modem_add_network_apn();
            //apn_defined = true;  //TODO delete variable when test ok
            if (modem_add_network_apn() == ESP_OK) {
                ESP_LOGI(TAG_MODEM, "APN added to Modem.");
                apn_defined = true;
            }else {
                ESP_LOGE(TAG_MODEM, "Failed to add apn.");
            }
        }
        if (modem_init_success == true && modem_data_sent == false && apn_defined == true) {
            // Example: Get network registration status
            if (modem_check_network_registration() == ESP_OK) {
                modem_data_sent = true;
                ESP_LOGI(TAG_MODEM, "Network registration check successful.");
            } else {
                ESP_LOGE(TAG_MODEM, "Failed to check network registration.");
            }

            // Additional AT commands can be sent similarly...
            vTaskDelete(NULL); // End the task
        }
    }
}


/**
 * @brief Task to handle UART communication between terminal and modem.
 *
 * This function reads data from the terminal UART, sends it to the modem UART,
 * then reads feedback from the modem UART and sends it back to the terminal UART.
 * It continuously loops with a small delay to handle UART data transfer efficiently.
 *
 * @param pvParameter Pointer to the parameters passed to the task (not used in this function).
 */
void uart_task(void *pvParameter) {
    uint8_t *data = (uint8_t *) malloc(1024);

    while (1) {
        // Read data from terminal UART
        int len = uart_read_bytes(terminal_uart, data, 1024, 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0'; // Null-terminate the string
            uart_write_bytes(modem_uart, (const char *) data, len); // Send to modem
            ESP_LOGI(TAG_UART, "Sent to modem: %s", data);
        }

        // Read feedback from modem UART
        len = uart_read_bytes(modem_uart, data, 1024, 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0'; // Null-terminate the string
            uart_write_bytes(terminal_uart, (const char *) data, len); // Relay to terminal
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
