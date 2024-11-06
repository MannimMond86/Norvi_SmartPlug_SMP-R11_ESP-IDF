#ifndef COMMON_VARIABLE_HANDLER_H
#define COMMON_VARIABLE_HANDLER_H

#include "driver/uart.h"
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uart_port_t modem_uart ;
extern const uart_port_t terminal_uart ;
extern const int uart_buffer_size ;
extern bool modem_init_success;
extern const char *TAG_MODEM;
extern const char *TAG_UART;
extern const char *TAG_MAIN;
extern const char *TAG_WIFI;
extern bool wifi_is_connected;

#ifdef __cplusplus
}
#endif

#endif