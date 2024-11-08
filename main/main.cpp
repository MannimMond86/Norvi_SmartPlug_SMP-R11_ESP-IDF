/***********************************************************************************************
* FILENAME       : main.cpp
 * VERSION        : 0.0.1
 * DESCRIPTION    : This code defines an ESP32 application that manages modem operations and facilitates UART communication between a terminal and the modem using FreeRTOS tasks.
 *
 * PUBLIC FUNCTIONS:
 *    - mode_task()
 *    - uart_task()
 *    - app_main()
 *
 * NOTES:
 *    Prefixes denote variable or return types:
 *    - "c" for char, "s" for short, "v" for void, "u" for unsigned, and "x" for non-standard types.
 *
 * AUTHOR         : Tjark Ziehm
 * START DATE     : July 6, 2024
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
#include <ArduinoJson.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_netif.h>
#include <esp_wifi.h>
#include "esp_task_wdt.h"
#include "nvs_flash.h"

#include "modem_interface.h"
#include "uart_forwarder.h"
#include "common_variable_handler.h"
#include "wifi_handler.h"
#include "thingsboard_interface.h"

#include "sdkconfig.h"

bool internet_connected = false;

//############################################---THINGDSBOARD---############################################//
#include <esp_random.h>
#include <Espressif_MQTT_Client.h>
#include <ThingsBoard.h>

//TODO Write Documentation
void thingsboard_task(void *param) {
    esp_task_wdt_add(NULL);
    // Initalize the Mqtt client instance
    Espressif_MQTT_Client<> mqttClient;
    // Initialize ThingsBoard instance with the maximum needed buffer size
    ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);
    //initialize_thingsboard();

    #if ENCRYPTED
        mqttClient.set_server_certificate(ROOT_CERT);
    #endif // ENCRYPTED

    for (;;) {
        // Wait until we connected to internet
        //if (!wifi_connected) {
        //    vTaskDelay(1000 / portTICK_PERIOD_MS);
        //    continue;
        //}
        esp_task_wdt_reset();
		if( internet_connected == true ){
            if (!tb.connected()) {
          	    ESP_LOGI(TAG_THINGSBOARD, "[THINGSBOARD] Start..");
                tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT);
            }
		    esp_task_wdt_reset();
		    ESP_LOGI(TAG_THINGSBOARD, "[THINGSBOARD] Start..");
            tb.sendTelemetryData(TEMPERATURE_KEY, esp_random());
            tb.sendTelemetryData(HUMIDITY_KEY, esp_random());

            tb.loop();

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    esp_task_wdt_delete(NULL);
}


//############################################---WIFI---#####################################################//
//TODO check void *pvParameter in all functions @main.cpp
//void wifi_task(void *pvParameter) {
void wifi_task(void *param) {
    if(wifi_is_connected == false) {
        ESP_LOGW(TAG_WIFI, "Connecting WIFI...");
        InitWiFi();
    }

    for (;;) {
        // Wait until we connected to WiFi
        if (!wifi_is_connected) {
            internet_connected = false;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        internet_connected = true;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


//############################################---MODEM---#####################################################//
/**
 * @brief Task to manage modem operations including initializing the network APN and checking network registration.
 *
 * This task begins by adding the network APN to the modem. If successful, it proceeds to check the network registration status.
 * Once the network is registered, the task will send initial data and end itself. It continuously loops with small delays to ensure the operations are completed sequentially.
 *
 * @param param Pointer to the parameters passed to the task (not used in this function).
 */
void modem_task(void *param) {
    esp_task_wdt_add(NULL);
    //modem_init();
    bool apn_defined = false;
    bool modem_data_communication = false;
    bool modem_received_revision_version = false;   //default:false
    bool modem_received_firmware_version = false;   //default:false
    bool modem_received_product_number = false;   //default:false
	failedAttempts = 0;

    while (1) {

        if (modem_received_revision_version  == false) {
            //modem_request_product_serial_number();
            if (modem_get_revision_version() == ESP_OK) {
                ESP_LOGW(TAG_MODEM, "Received revision version.");
                modem_received_revision_version = true;
            } else {
                ESP_LOGE(TAG_MODEM, "Failed to get revision version.");
            }
        }

        esp_task_wdt_reset();
		vTaskDelay(5000 / portTICK_PERIOD_MS);
        esp_task_wdt_reset();
        if (modem_received_firmware_version  == false) {
            //modem_get_revision();
            if (modem_get_firmware_version() == ESP_OK) {
                ESP_LOGW(TAG_MODEM, "Received firmware version.");
                modem_received_firmware_version  = true;
            } else {
                ESP_LOGE(TAG_MODEM, "Failed to get firmware version.");
            }
        }

		esp_task_wdt_reset();
		vTaskDelay(5000 / portTICK_PERIOD_MS);
        esp_task_wdt_reset();
        if (modem_received_product_number== false) {
            //modem_get_firmware_version();
            if (modem_get_product_number() == ESP_OK) {
                ESP_LOGW(TAG_MODEM, "Received product number.");
                modem_received_product_number = true;
            } else {
                ESP_LOGE(TAG_MODEM, "Failed to get product number.");
            }
        }

        while (failedAttempts < 5){
            if ( registration_step0_success == false && registration_step1_success==false && registration_step2_success==false ){
                esp_task_wdt_reset();
                if (modem_set_CFUN_0() == ESP_OK) {
                    esp_task_wdt_reset();
                    ESP_LOGW(TAG_MODEM, "Modem in init State.");
                } else {
                    esp_task_wdt_reset();
                    ESP_LOGE(TAG_MODEM, "Failed to create init state.");
                }
		        esp_task_wdt_reset();
		        vTaskDelay(2000 / portTICK_PERIOD_MS);
                esp_task_wdt_reset();

                if (modem_set_CFUN_1() == ESP_OK) {
                    esp_task_wdt_reset();
                    ESP_LOGW(TAG_MODEM, "Modem in active state.");
        	    } else {
        	        esp_task_wdt_reset();
                    ESP_LOGE(TAG_MODEM, "Failed to create active state.");
        	    }
        	    esp_task_wdt_reset();
			    vTaskDelay(5000 / portTICK_PERIOD_MS);

        	    esp_task_wdt_reset();
			    if (modem_set_network_registration() == ESP_OK) {
                    ESP_LOGW(TAG_MODEM, "Modem set network registration.");
			        esp_task_wdt_reset();
        	    } else {
                    ESP_LOGE(TAG_MODEM, "Failed to set network registration.");
        	        esp_task_wdt_reset();
        	    }
        	    esp_task_wdt_reset();
			    vTaskDelay(4000 / portTICK_PERIOD_MS);

                esp_task_wdt_reset();
			    if (modem_add_network_apn() == ESP_OK) {
                    ESP_LOGW(TAG_MODEM, "Modem added APN.");
			        esp_task_wdt_reset();
        	    } else {
                    ESP_LOGE(TAG_MODEM, "Failed to add APN.");
        	    }
        	    esp_task_wdt_reset();
			    vTaskDelay(5000 / portTICK_PERIOD_MS);
                esp_task_wdt_reset();
                registration_step0_success = true;
            }
            ESP_LOGI(TAG_MODEM, "registration_step0_success.");

            esp_task_wdt_reset();
            if ( registration_step0_success == true ){
                //#####################---NETWORK-REGISTRATION---#######################################


			    if (modem_send_CGATT() == ESP_OK && (registration_step1_success==false) && (registration_step2_success==false)){
			        ESP_LOGW(TAG_MODEM, "registration_step1_success");
			        registration_step1_success=true;
			        esp_task_wdt_reset();
			        if (modem_read_CGATT(10000) == ESP_OK){
			            ESP_LOGW(TAG_MODEM, "1xxxxxxxxxx");
			            esp_task_wdt_reset();
                    }else{
                        ESP_LOGW(TAG_MODEM, "2xxxxxxxxxx");
			            esp_task_wdt_reset();
			            failedAttempts++;
                        break;
                    }
			        esp_task_wdt_reset();
                } else if ( registration_step1_success==true && registration_step2_success==false){
                    ESP_LOGW(TAG_MODEM, "3xxxxxxxxxx");
                    esp_task_wdt_reset();
                    modem_read_CGATT(5000);
                    esp_task_wdt_reset();
                    break;
                }
            }

            if ( failedAttempts == 5){
                ESP_LOGW(TAG_MODEM, "Reached 5 failled Attempts");
                failedAttempts=0;
                modem_reset();
                esp_task_wdt_reset();
            }

            ESP_LOGW(TAG_MODEM, "1MODEM TASK DONE");
        	esp_task_wdt_reset();
            esp_task_wdt_delete(NULL);
            vTaskDelete(NULL); // End the task
        }
        // Additional AT commands can be sent similarly...
        //esp_task_wdt_reset();
        internet_connected = true;

        if( internet_connected == true ){
            modem_ping();
            //xTaskCreate(thingsboard_task, "thingsboard_task", 1024 * 4, NULL, 5, NULL);
            ESP_LOGI(TAG_MAIN, "[APP] ThingsBoard Task activated");
        }
        ESP_LOGW(TAG_MODEM, "2MODEM TASK DONE");
        esp_task_wdt_delete(NULL);
        vTaskDelete(NULL); // End the task

    }
}


//############################################---UART---#####################################################//
/**
 * @brief Task to handle UART communication between terminal and modem.
 *
 * This function reads data from the terminal UART, sends it to the modem UART,
 * then reads feedback from the modem UART and sends it back to the terminal UART.
 * It continuously loops with a small delay to handle UART data transfer efficiently.
 *
 * @param pvParameter Pointer to the parameters passed to the task (not used in this function).
 */
void uart_task(void *pvParameter) {
    uint8_t *data = (uint8_t *) malloc(1024);

    while (1) {
        // Read data from terminal UART
        int len = uart_read_bytes(terminal_uart, data, 1024, 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0'; // Null-terminate the string
            uart_write_bytes(modem_uart, (const char *) data, len); // Send to modem
            ESP_LOGI(TAG_UART, "Sent to modem: %s", data);
        }

        // Read feedback from modem UART
        len = uart_read_bytes(modem_uart, data, 1024, 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0'; // Null-terminate the string
            uart_write_bytes(terminal_uart, (const char *) data, len); // Relay to terminal
            ESP_LOGI(TAG_UART, "Received from modem: %s", data);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    free(data);
}

void test_network_task(void *pvParameter) {

}


//############################################---MAIN---#####################################################//
extern "C" void app_main() {
    vTaskDelay(50 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG_MAIN, "[APP] Startup..");
    ESP_LOGI(TAG_MAIN, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG_MAIN, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);

    ESP_ERROR_CHECK(nvs_flash_init());
    vTaskDelay(50 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(esp_netif_init());
    vTaskDelay(50 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    vTaskDelay(50 / portTICK_PERIOD_MS);


    if(CONFIG_WIFI_ACTIVATED == 1){

        ESP_LOGW(TAG_MAIN, "[APP] Startup WIFI...");
        // Initialize NVS (Non-Volatile Storage)
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);

        // Create a FreeRTOS task to handle Wi-Fi connection
        xTaskCreate(wifi_task, "wifi_task", 1024 * 4, NULL, 5, NULL);
        //TODO refactor thingsboard init
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG_MAIN, "[APP] WIFI Task activated");
        //xTaskCreate(thingsboard_task, "thingsboard_task", 1024 * 4, NULL, 5, NULL);

    }else if (CONFIG_WIFI_ACTIVATED == 0){

        ESP_LOGW(TAG_MAIN, "[APP] Startup Modem...");

        if(CONFIG_IS_ACTIVATED_UART == true){
            uart_init();
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        xTaskCreate(uart_task, "uart_task", 1024 * 2, NULL, 10, NULL);
        ESP_LOGI(TAG_MAIN, "[APP] UART Task activated");

        vTaskDelay(50 / portTICK_PERIOD_MS);
        modem_reset();
        vTaskDelay(50 / portTICK_PERIOD_MS);
        modem_enable();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        xTaskCreate(modem_task, "modem_task", 1024 * 4, NULL, 5, NULL);
        ESP_LOGI(TAG_MAIN, "[APP] Modem Task activated");

        //TODO refactor thingsboard init
        //vTaskDelay(20000 / portTICK_PERIOD_MS);

        //ESP_LOGI(TAG_MAIN, "[APP] ThingsBoard connected?");
        //xTaskCreate(test_network_task, "test_network_task", 1024 * 4, NULL, 5, NULL);
    }else{
        ESP_LOGE(TAG_MAIN, "[APP] Failed to connect to Internet");
    }

}
