#ifndef WIFI_HANLDER_H
#define WIFI_HANLDER_H
#include "esp_event.h"
#include "esp_log.h"
#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

void InitWiFi();
void on_got_ip(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#ifdef __cplusplus
}
#endif

#endif



