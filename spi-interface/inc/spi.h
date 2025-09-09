/**
 * @file
 * @brief This library contains the spi interface implementation. This interface allows
 * communication with SPI devices through a standardized API. Functionality can be injected through
 * the use of driver function pointers.
 */

/* ============================================================================================== */

#ifndef SPI_H
#define SPI_H

/* ============================================================================================== */

#include <stdint.h>
#include <stdio.h>

/* ============================================================================================== */

#include "../inc/errno.h"

/* ============================================================================================== */

/* Callback function prototype */
typedef void (*spi_rx_callback_t)(uint16_t);

/* ============================================================================================== */

typedef int8_t (*spi_initialize_t)(void);
typedef int8_t (*spi_write_word_t)(uint16_t);
typedef int8_t (*spi_read_word_t)(uint16_t*);
typedef int8_t (*spi_set_rx_callback_t)(spi_rx_callback_t);
typedef void (*spi_clear_buffers_t)(void);

/* ============================================================================================== */

/**
 * @brief This structure contains the driver function pointers for the SPI driver.
 */
typedef struct
{
    spi_initialize_t      initialize;
    spi_write_word_t      write_word;
    spi_read_word_t       read_word;
    spi_set_rx_callback_t set_rx_callback;
    spi_clear_buffers_t   clear_buffers;
} spi_drivers_t;

/**
 * @brief This structure contains the definition of the class SPI. All SPI objects created will
 * be instances of this class.
 */
typedef struct
{
    /**
     * @brief Initializes the SPI interface.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    spi_initialize_t initialize;

    /**
     * @brief Transmits a 16-bit word through the SPI interface.
     * @param word The 16-bit word to transmit.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    spi_write_word_t write_word;

    /**
     * @brief Receives a 16-bit word from the SPI interface.
     * @param word Pointer to the 16-bit variable where the received word will be stored.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    spi_read_word_t read_word;

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

/**
 * @brief This function creates a new instance of the spi_t. It also initializes
 * the object attributes and methods.
 * @return Returns a new instance already initialized of the spi_t.
 */
int8_t spi_create(spi_t* self, const spi_drivers_t* drivers);

/* ============================================================================================== */

/*
 * Usage example:
 *
 * int main(void)
 * {
 *  spi_t spi;
 *  spi_drivers_t drivers = {
 *      .initialize = spi_initialize,
 *      .write_word = spi_write_word,
 *      .read_word = spi_read_word,
 *      .set_rx_callback = spi_set_rx_callback,
 *      .clear_buffers = spi_clear_buffers
 *  };
 *  spi_create(&spi, &drivers);
 *  spi.initialize();
 *  return 0;
 * }
 */

/* ============================================================================================== */

#endif /* SPI_H */
