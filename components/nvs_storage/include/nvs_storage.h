#ifndef NVS_STORAGE_H
#define NVS_STORAGE_H
#include "esp_event.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "nvs_flash.h"
#ifdef __cplusplus
extern "C" {
#endif

    extern "C" void inti_nvs_storage(void);
    extern "C" void read_nvs_storage(void);
    extern "C" void write_nvs_storage(void);
    extern "C" void test_nvs_storage(void);

#ifdef __cplusplus
}
#endif

#endif
