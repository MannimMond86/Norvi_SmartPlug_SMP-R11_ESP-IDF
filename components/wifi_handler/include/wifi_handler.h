#ifndef WIFI_HANLDER_H
#define WIFI_HANLDER_H
#include "esp_event.h"
#include "esp_log.h"

static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
void wifi_init_sta(void);
void connect_wifi(void);

#endif



