#ifndef MODEM_INTERFACE_H
#define MODEM_INTERFACE_H

#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include "string.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif

bool modem_reset();
void modem_enable();
void modem_init();

// TODO: check need and usage
esp_err_t modem_send_command(const char *command);
esp_err_t modem_read_response(char *response, size_t max_len);
esp_err_t modem_read_multi_responses(char *response, size_t max_len, int awaited_responses);
esp_err_t modem_check_network_registration();
esp_err_t modem_set_network_registration();
esp_err_t modem_add_network_apn();
esp_err_t modem_get_firmware_version();
esp_err_t modem_get_revision_version();
esp_err_t modem_get_product_number();

esp_err_t modem_set_network_connection();
esp_err_t modem_ping();
esp_err_t modem_get_ip();
esp_err_t modem_set_dns_server();
esp_err_t modem_get_dns_server();
esp_err_t modem_get_CFUN();
esp_err_t modem_set_CFUN_0();
esp_err_t modem_set_CFUN_1();
esp_err_t modem_send_CGATT();
esp_err_t modem_read_CGATT(int time);

esp_err_t modem_check_network_connection();
esp_err_t modem_get_connection_Status();

esp_err_t modem_get_PLMN();
esp_err_t modem_set_PLMN();

esp_err_t modem_get_NBAND();

#ifdef __cplusplus
}
#endif

#endif