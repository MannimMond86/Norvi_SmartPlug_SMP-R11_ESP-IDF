#include "common_variable_handler.h"
#include "string.h"
#include "driver/uart.h"
#include <stdbool.h>

const uart_port_t modem_uart = UART_NUM_1;
const uart_port_t terminal_uart = UART_NUM_0;
const int uart_buffer_size = (1024 * 2);
bool modem_init_success = false;
const char *TAG_MODEM = "Modem";
const char *TAG_UART = "UART";