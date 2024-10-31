#ifndef MODEM_INTERFACE_H
#define MODEM_INTERFACE_H

#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include "string.h"
#include "esp_log.h"

bool modem_reset();
void modem_enable();
void modem_init();
esp_err_t modem_send_command(const char *command);
esp_err_t modem_read_response(char *response, size_t max_len);
esp_err_t modem_check_network_registration();
esp_err_t modem_add_network_apn();
esp_err_t modem_get_firmware_version();

#endif