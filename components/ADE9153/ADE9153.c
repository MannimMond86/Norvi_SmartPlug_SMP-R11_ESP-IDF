/***********************************************************************************************
* FILENAME       : ADE9153.c
 * VERSION        : 0.0.1
 * DESCRIPTION    :
 *
 * PUBLIC FUNCTIONS:
 *    - read_ade9135_data(uint8_t *data, size_t len)
 *
 * NOTES:
 *    Prefixes denote variable or return types:
 *    - "c" for char, "s" for short, "v" for void, "u" for unsigned, and "x" for non-standard types.
 *
 * AUTHOR         : Tjark Ziehm
 * START DATE     : October 1, 2024
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

#include <stdio.h>
#include "ADE9153.h"
#include "spi_flash_mmap.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_err.h"


// SPI configuration
spi_device_handle_t spi;

spi_bus_config_t bus_config = {
    .miso_io_num = GPIO_NUM_12,
    .mosi_io_num = GPIO_NUM_13,
    .sclk_io_num = GPIO_NUM_14,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4096
};

spi_device_interface_config_t dev_config = {
    .command_bits = 0,
    .address_bits = 0,
    .mode = 0,
    .clock_speed_hz = 1000000,
    .spics_io_num = GPIO_NUM_15,
    .queue_size = 7,
    .flags = SPI_DEVICE_HALFDUPLEX
};

/**
 * @brief Reads data from the ADE9135 sensor using SPI.
 *
 * This function performs an SPI transaction to read data from the ADE9135 sensor.
 * The data read from the sensor is stored in the provided buffer.
 *
 * @param data Pointer to the buffer where the read data will be stored.
 * @param len Number of bytes to read from the sensor.
 *
 * @return
 *     - ESP_OK: Success
 *     - ESP_ERR_INVALID_ARG: Parameter error
 *     - ESP_ERR_INVALID_STATE: Driver not installed or not in an appropriate state
 *     - ESP_FAIL: Sending command error, slave hasn't changed state probably due to an error
 */
esp_err_t read_ade9135_data(uint8_t *data, size_t len) {
    spi_transaction_t t = {
        .tx_buffer = NULL,
        .rx_buffer = data,
        .length = len * 8,
        .flags = SPI_TRANS_USE_RXDATA
    };

    // Send read command to ADE9135
    esp_err_t ret = spi_device_transmit(spi, &t);
    return ret;
}
