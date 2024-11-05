#ifndef WIFI_HANLDER_H
#define WIFI_HANLDER_H
#include "esp_event.h"
#include "esp_log.h"


void InitWiFi();
void on_got_ip(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);


#endif



