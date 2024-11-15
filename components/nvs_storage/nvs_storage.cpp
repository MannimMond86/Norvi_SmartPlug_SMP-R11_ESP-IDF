#include "sdkconfig.h"
#include "common_variable_handler.h"
#include <inttypes.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_storage.h"

extern "C" void test_nvs_storage(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // Open
    ESP_LOGI(TAG_NVS, "Opening Non-Volatile Storage (NVS) handle... ");

    // Handle will automatically close when going out of scope or when it's reset.
    std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle("storage", NVS_READWRITE, &err);
    if (err != ESP_OK) {
        //printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        ESP_LOGE(TAG_NVS, "Error (%s) opening NVS handle!",esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG_NVS, "Done");
        // Read
        ESP_LOGI(TAG_NVS, "Reading restart counter from NVS ... ");
        int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
        err = handle->get_item("restart_counter", restart_counter);
        system_starts = restart_counter ;
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG_NVS, "Done");
                ESP_LOGE(TAG_NVS, "Restart counter = %" PRIu32 "",restart_counter);
                //printf("Restart counter = %" PRIu32 "\n", restart_counter);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGW(TAG_NVS, "The value is not initialized yet!");
                break;
            default :
                ESP_LOGE(TAG_NVS, "Error (%s) reading!", esp_err_to_name(err));
        }

        // Write
        ESP_LOGW(TAG_NVS, "Updating restart counter in NVS ... ");

        restart_counter++;
        err = handle->set_item("restart_counter", restart_counter);
        //printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        if ( err != ESP_OK ) {
            ESP_LOGE(TAG_NVS, "Failed!");
        }else{
            ESP_LOGI(TAG_NVS, "Done");
        }
        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        ESP_LOGI(TAG_NVS, "Committing updates in NVS ... ");

        err = handle->commit();
        if ( err != ESP_OK ) {
            ESP_LOGE(TAG_NVS, "Failed!");
        }else{
            ESP_LOGI(TAG_NVS, "Done");
        }
    }

    printf("\n");

    // Restart module
    for (int i = 60; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

extern "C" void read_nvs_storage(void)
{

}

extern "C" void write_nvs_storage(void)
{

}

extern "C" void inti_nvs_storage(void){
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    ESP_LOGI(TAG_NVS, "[APP] Startup..");
    // Handle will automatically close when going out of scope or when it's reset.
    std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle("storage", NVS_READWRITE, &err);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Done\n");

        // Read
        printf("Reading restart counter from NVS ... ");
        int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
        err = handle->get_item("restart_counter", restart_counter);
        system_starts = restart_counter ;
        switch (err) {
            case ESP_OK:
                printf("Done\n");
            printf("Restart counter = %" PRIu32 "\n", restart_counter);
            break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
            break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
    }
}

