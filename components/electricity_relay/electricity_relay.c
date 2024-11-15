#include "electricity_relay.h"
#include "common_variable_handler.h"
#include "sdkconfig.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include <stdio.h>

#define GPIO_OUTPUT_PIN 12
// #define GPIO_OUTPUT_IO_0    CONFIG_GPIO_OUTPUT_0

void turn_relay_on(void){
    // Turn the relay on
    gpio_set_level(GPIO_OUTPUT_PIN, 1);
    ESP_LOGI(TAG_RELAY, "Relay ON");
}


void turn_relay_off(void){
    // Turn the relay off
    gpio_set_level(GPIO_OUTPUT_PIN, 0);
    ESP_LOGI(TAG_RELAY, "Relay OFF");
}


 void init_electricity_relay(void){
    gpio_reset_pin(GPIO_OUTPUT_PIN);
    gpio_set_direction(GPIO_OUTPUT_PIN, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG_RELAY, "Relay initialized");
}

void switch_electricity_relay(bool next_relay_state){
    if ( next_relay_state == 1 ){
        turn_relay_on();

    } else if ( next_relay_state == 0 ){
        turn_relay_off();
    } else {
        ESP_LOGW(TAG_RELAY, "Problem with relay state");
    }

}