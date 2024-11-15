#ifndef ELECTRICITY_RELAY_H
#define ELECTRICITY_RELAY_H

#include "esp_event.h"
#include "esp_log.h"
#include "sdkconfig.h"



void init_electricity_relay(void);
void switch_electricity_relay(bool next_relay_state);



#endif