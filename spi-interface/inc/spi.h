#ifndef SPI_H
#define SPI_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>

/* ============================================================================================== */

#include "../inc/errno.h"

/* ============================================================================================== */

/* RX callback prototype. Receives a pointer to the received buffer and its length */
typedef void (*spi_rx_callback_t)(const uint8_t* buffer, size_t length);

/* ============================================================================================== */

typedef int8_t (*spi_initialize_t)(void);
typedef int8_t (*spi_transfer_t)(const uint8_t* tx_buffer, uint8_t* rx_buffer, size_t length);
typedef int8_t (*spi_set_rx_callback_t)(spi_rx_callback_t callback);
typedef void (*spi_clear_buffers_t)(void);

/* ============================================================================================== */

typedef struct
{
    spi_initialize_t      initialize;
    spi_transfer_t        transfer;
    spi_set_rx_callback_t set_rx_callback;
    spi_clear_buffers_t   clear_buffers;
} spi_t;

/* ============================================================================================== */

#endif  // SPI_H
