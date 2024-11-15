/***********************************************************************************************
* FILENAME       : common_variable_handler.c
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

#include "common_variable_handler.h"
#include <stdint.h>
#include "string.h"
#include "driver/uart.h"
#include <stdbool.h>
#include "sdkconfig.h"

int32_t system_starts;
const uart_port_t modem_uart = UART_NUM_1;
const uart_port_t terminal_uart = UART_NUM_0;
const int uart_buffer_size = (1024 * 2);
bool modem_init_success = false;

const char *TAG_MAIN = "Main";
const char *TAG_ADE9153 = "Modem";
const char *TAG_DISPLAY = "Display";
const char *TAG_DISPLAY_SSD1306 = "Display SSD1306";
const char *TAG_RELAY = "Electricity Relay";
const char *TAG_FATFS = "FATFS storage";
const char *TAG_MODEM = "Modem";
const char *TAG_MQTT = "MQTT";
const char *TAG_NVS = "NVS";
const char *TAG_THINGSBOARD = "ThingsBoard";
const char *TAG_TIME = "Time manager";
const char *TAG_BUTTON = "Touch Button";
const char *TAG_UART = "UART";
const char *TAG_WIFI = "WiFi";

bool wifi_is_connected = false;
bool registration_step0_success = false;
bool registration_step1_success = false;
bool registration_step2_success = false;
int failedAttempts = 0;

