#ifndef COMMON_VARIABLE_HANDLER_H
#define COMMON_VARIABLE_HANDLER_H

#include "driver/uart.h"
#include <stdbool.h>
#include <stdint.h>  // For int32_t
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int32_t system_starts;
extern const uart_port_t modem_uart;
extern const uart_port_t terminal_uart;
extern const int uart_buffer_size;
extern bool modem_init_success;

extern const char *TAG_MAIN;
extern const char *TAG_ADE9153;
extern const char *TAG_DISPLAY;
extern const char *TAG_DISPLAY_SSD1306;
extern const char *TAG_RELAY;
extern const char *TAG_FATFS;
extern const char *TAG_MODEM;
extern const char *TAG_MQTT;
extern const char *TAG_NVS;
extern const char *TAG_THINGSBOARD;
extern const char *TAG_TIME;
extern const char *TAG_BUTTON;
extern const char *TAG_UART;
extern const char *TAG_WIFI;

extern bool wifi_is_connected;
extern bool registration_step0_success;
extern bool registration_step1_success;
extern bool registration_step2_success;
extern int failedAttempts;

#ifdef __cplusplus
}
#endif

#endif