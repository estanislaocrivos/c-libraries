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
    /**
     * @brief Initializes the SPI interface.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    spi_initialize_t initialize;

    /**
     * @brief Transmits and receives a generic length 'length' 8-bit buffer through the SPI
     * interface.
     * @param tx_buffer Pointer to the 8-bit buffer to be transmitted.
     * @param rx_buffer Pointer to the 8-bit buffer where the received data will be stored.
     * @param length Length of the buffers to be transmitted and received.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    spi_transfer_t transfer;

    /**
     * @brief Sets the callback function to be called when a full word has been received.
     * @param callback The callback function to set. Its prototype must match the spi_rx_callback_t
     * type.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    spi_set_rx_callback_t set_rx_callback;

    /**
     * @brief Clears the SPI buffers.
     */
    spi_clear_buffers_t clear_buffers;
} spi_t;

/* ============================================================================================== */

/*
int main(void)
{
    const spi_t spi = {spi1_initialize, spi1_transfer, spi1_set_rx_callback, spi1_clear_buffers};
    spi.initialize();
    return 0;
}
*/

/* ============================================================================================== */

#endif  // SPI_H
