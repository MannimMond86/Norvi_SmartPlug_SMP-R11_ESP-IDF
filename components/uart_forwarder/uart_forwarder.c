/***********************************************************************************************
* FILENAME       : uart_forwarder.c
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
#include "driver/uart.h"
#include "esp_log.h"
#include "string.h"
#include "uart_forwarder.h"
#include "common_variable_handler.h"
#include "sdkconfig.h"

// set the bidirectional communication to Modem via AT Commands
#define GSM_UART_TX CONFIG_BC28_RX_PIN
#define GSM_UART_RX CONFIG_BC28_TX_PIN

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