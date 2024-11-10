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
#include "modem_api.h"
#include "common_variable_handler.h"
#include "sdkconfig.h"

#define GSM_RESET CONFIG_BC28_ENABLE_PIN
#define GSM_ENABLE CONFIG_BC28_RESET_PIN
int TIMEOUT_MS = 1000;                 // UART timeout


bool modem_reset() {
    gpio_set_direction(GSM_RESET, GPIO_MODE_OUTPUT);
    gpio_set_level(GSM_RESET, 0);  // Reset the modem
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(GSM_RESET, 1);  // Release reset
    vTaskDelay(5000 / portTICK_PERIOD_MS);
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
    ESP_LOGW(TAG_MODEM, "Sent command: %s", command);
    return (sent == len) ? ESP_OK : ESP_FAIL;
}


// Read modem response
esp_err_t modem_read_response(char *response, size_t max_len) {
    int length = uart_read_bytes(modem_uart, (uint8_t *)response, max_len, pdMS_TO_TICKS(TIMEOUT_MS));
    if (length > 0) {
        response[length] = '\0';  // Null-terminate the response
        ESP_LOGW(TAG_MODEM, "Received response: %s", response);
        return ESP_OK;
    }
    return ESP_FAIL;
}


// Read multi modem responses
esp_err_t  modem_read_multi_CGATT(char *response, size_t max_len,int timeout) {
    int length = uart_read_bytes(modem_uart, (uint8_t *)response, max_len, pdMS_TO_TICKS(timeout));

    if (length > 0) {
      	response[length] = '\0';  // Null-terminate the response
       	ESP_LOGW(TAG_MODEM, "Received response: %s", response);

       	if ( response == "+CEREG:2"){
			registration_step1_success = true;
            return ESP_OK;
        }

        if ( response == "+CEREG:5"){
			registration_step2_success = true;
            return ESP_OK;
        }
    } else {
        ESP_LOGE(TAG_MODEM, "Error reading from UART");
        return ESP_FAIL;  // Fehlercode als NULL zurückgeben
    }

    return ESP_FAIL;
}


esp_err_t modem_read_CGATT(int timeout){
    char response[uart_buffer_size];
    if ( modem_read_multi_CGATT(response,uart_buffer_size,timeout) == ESP_OK){
        return ESP_OK;
    }else{
        return ESP_FAIL;
    }
}


esp_err_t modem_send_CGATT(){
    ESP_LOGI(TAG_MODEM, "AT+CGATT=1");

    return modem_send_command("AT+CGATT=1");
}


// Example AT Command function for network registration
esp_err_t modem_check_network_registration() {
    ESP_LOGI(TAG_MODEM, "AT+CEREG?");
    modem_send_command("AT+CEREG?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here (parse "+CEREG: 1,1" etc.)
        return ESP_OK;
    }
    return ESP_FAIL;
}

// Example AT Command function for network registration
esp_err_t modem_set_network_registration() {
    ESP_LOGI(TAG_MODEM, "AT+CEREG=1");
    modem_send_command("AT+CEREG=1");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here (parse "+CEREG: 1,1" etc.)
        return ESP_OK;
    }
    return ESP_FAIL;
}

// Example AT Command function for network registration
esp_err_t modem_add_network_apn() {
    ESP_LOGI(TAG_MODEM, "AT+CGMR=1,IP,wsim");
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
    ESP_LOGI(TAG_MODEM, "AT+CGMR");
    modem_send_command("AT+CGMR");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here (parse "+CGMR: 1,1" etc.)
        return ESP_OK;
    }
    return ESP_FAIL;
}

// Example AT Command function for network registration
esp_err_t modem_get_revision_version() {
    ESP_LOGI(TAG_MODEM, "ATI");
    modem_send_command("ATI");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

// Example AT Command function for network registration
esp_err_t modem_get_product_number() {
    ESP_LOGI(TAG_MODEM, "AT+CGSN=2");
    modem_send_command("AT+CGSN=2");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_check_network_connection(){
    ESP_LOGI(TAG_MODEM, "AT+CGATT?");
    modem_send_command("AT+CGATT?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_ping(){
    ESP_LOGI(TAG_MODEM, "AT+NPING");
    modem_send_command("AT+NPING=\"8.8.8.8\"");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_get_ip(){
    ESP_LOGI(TAG_MODEM, "AT+CGPADDR");
    modem_send_command("AT+CGPADDR");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_set_dns_server(){
    ESP_LOGI(TAG_MODEM, "AT+QIDNSCFG=DNS");
    modem_send_command("AT+QIDNSCFG=\"1.1.1.1\",\"8.8.8.8\"");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_get_dns_server(){
    ESP_LOGI(TAG_MODEM, "AT+QIDNSCFG?");
    modem_send_command("AT+QIDNSCFG?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_get_CFUN(){
    ESP_LOGI(TAG_MODEM, "AT+CFUN?");
    modem_send_command("AT+CFUN?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_set_CFUN_0(){
    ESP_LOGI(TAG_MODEM, "AT+CFUN=0");
    modem_send_command("AT+CFUN=0");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_set_CFUN_1(){
    ESP_LOGI(TAG_MODEM, "AT+CFUN=1");
    modem_send_command("AT+CFUN=1");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_get_connection_Status(){
    ESP_LOGI(TAG_MODEM, "AT+CSCON?");
    modem_send_command("AT+CSCON?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}


esp_err_t modem_get_PLMN(){
    ESP_LOGI(TAG_MODEM, "AT+COPS?");
    modem_send_command("AT+COPS?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_set_PLMN(){
    //ESP_LOGI(TAG_MODEM, "AT+COPS=1,2,\"26201\"");
    //modem_send_command("AT+COPS=1,2,\"26201\"");
    ESP_LOGI(TAG_MODEM, "AT+COPS=0");
    modem_send_command("AT+COPS=0");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t modem_get_NBAND(){
    ESP_LOGI(TAG_MODEM, "AT+NBAND=?");
    modem_send_command("AT+NBAND=?");
    char response[uart_buffer_size];
    if (modem_read_response(response, uart_buffer_size) == ESP_OK) {
        // Process response here
        return ESP_OK;
    }
    return ESP_FAIL;
}