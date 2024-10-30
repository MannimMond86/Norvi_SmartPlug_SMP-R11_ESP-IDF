#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "string.h"
#include "uart_forwarder.h"
#include "common_variable_handler.h"

#define GSM_UART_TX 14
#define GSM_UART_RX 15
#define UART0_TX 1
#define UART0_RX 0
#define TIMEOUT_MS 1000                                       // UART timeout


void uart_init() {
    // Configure UART for terminal
    uart_config_t terminal_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Configure UART for modem
    uart_config_t modem_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Install UART drivers
    uart_driver_install(terminal_uart, uart_buffer_size, 0, 0, NULL, 0);
    uart_driver_install(modem_uart, uart_buffer_size, uart_buffer_size, 0, NULL, 0);

    // Set UART parameters
    uart_param_config(terminal_uart, &terminal_config);
    uart_param_config(modem_uart, &modem_config);

    // Set UART pins
    uart_set_pin(terminal_uart, UART0_TX, UART0_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_set_pin(modem_uart, GSM_UART_TX, GSM_UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}