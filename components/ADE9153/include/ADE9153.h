#ifndef ADE9153_H
#define ADE9153_H

#include <stdio.h>
#include "ADE9153.h"
#include "spi_flash_mmap.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t read_ade9135_data(uint8_t *data, size_t len);
spi_device_handle_t spi;
spi_bus_config_t bus_config;
spi_device_interface_config_t dev_config;

#ifdef __cplusplus
}
#endif

#endif
