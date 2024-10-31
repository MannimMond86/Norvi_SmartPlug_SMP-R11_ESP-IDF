/***********************************************************************************************
* FILENAME       : modem_interface.c
 * VERSION        : 0.0.1
 * DESCRIPTION    :
 *
 * PUBLIC FUNCTIONS:
 *    - func()
 *
 * NOTES:
 *    Prefixes denote variable or return types:
 *    - "c" for char, "s" for short, "v" for void, "u" for unsigned, and "x" for non-standard types.
 *
 * AUTHOR         : Tjark Ziehm
 * START DATE     : October 1, 2024
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
#include "string.h"
#include "esp_log.h"
#include "modem_interface.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "modem_interface.h"
#include "modem_api.h"
#include "common_variable_handler.h"
#include "sdkconfig.h"

#define GSM_RESET CONFIG_CUSTOM_BC28_ENABLE_PIN
#define GSM_ENABLE CONFIG_CUSTOM_BC28_RESET_PIN
#define TIMEOUT_MS 1000                 // UART timeout


bool modem_reset() {
    gpio_set_direction(GSM_RESET, GPIO_MODE_OUTPUT);
    gpio_set_level(GSM_RESET, 0);  // Reset the modem
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(GSM_RESET, 1);  // Release reset
    vTaskDelay(4000 / portTICK_PERIOD_MS);
    modem_init_success = true;
    return modem_init_success;
}


void modem_enable() {
    gpio_set_direction(GSM_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_level(GSM_ENABLE, 1);  // Enable the modem
}


// Send AT command to modem
esp_err_t modem_send_command(const char *command) {
    const int len = strlen(command);
    const int sent = uart_write_bytes(modem_uart, command, len);
    uart_write_bytes(modem_uart, "\r\n", 2);  // Send CR LF
    ESP_LOGI(TAG_MODEM, "Sent command: %s", command);
    return (sent == len) ? ESP_OK : ESP_FAIL;
}


// Read modem response
esp_err_t modem_read_response(char *response, size_t max_len) {
    int length = uart_read_bytes(modem_uart, (uint8_t *)response, max_len, pdMS_TO_TICKS(TIMEOUT_MS));
    if (length > 0) {
        response[length] = '\0';  // Null-terminate the response
        ESP_LOGI(TAG_MODEM, "Received response: %s", response);
        return ESP_OK;
    }
    return ESP_FAIL;
}


// Example AT Command function for network registration
esp_err_t modem_check_network_registration() {
    modem_send_command("AT+CEREG?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here (parse "+CEREG: 1,1" etc.)
        return ESP_OK;
    }
    return ESP_FAIL;
}

// Example AT Command function for network registration
esp_err_t modem_add_network_apn() {
    //char command[50];
    //const char *apn = "wsim";
    //snprintf(command, sizeof(command), "AT+CGDCONT=1,\"IP\",\"%s\"\r\n", apn);
    //modem_send_command(strlen(*command));
    modem_send_command("AT+CGDCONT=1,\"IP\",\"wsim\"\r\n");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here (parse "+CGDCONT:" etc.)
        return ESP_OK;
    }
    return ESP_FAIL;
}

// Example AT Command function for network registration
esp_err_t modem_get_firmware_version() {
    ESP_LOGI(TAG_MODEM, "Get Firmware version:");
    modem_send_command("AT+CGMR");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here (parse "+CGMR: 1,1" etc.)
        return ESP_OK;
    }
    return ESP_FAIL;
}

// Example AT Command function for network registration
esp_err_t modem_get_revision() {
    ESP_LOGI(TAG_MODEM, "Get Revision version:");
    modem_send_command("ATI");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}



// Example AT Command function for network registration
esp_err_t modem_request_product_serial_number() {
    ESP_LOGI(TAG_MODEM, "Get Product Serial Number & :");
    modem_send_command("AT+CGSN=2");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}