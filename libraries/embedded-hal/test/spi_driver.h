/**
 * @file
 * @brief This is a mock header file for the SPI1 peripheral driver. This file contains the
 * function prototypes for the SPI1 driver. Used for testing.
 */

/* ============================================================================================== */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>

/* ============================================================================================== */

#include "../inc/spi.h"
#include "../../inc/errno.h"

/* ============================================================================================== */

static void* spi1_callback_context;

static spi_rx_callback_t spi1_callback;

/* ============================================================================================== */

int8_t spi1_initialize(struct spi_port* port);

/* ============================================================================================== */

int8_t spi1_transmit(const struct spi_port* port, const uint8_t* buffer, size_t size);

/* ============================================================================================== */

int8_t spi1_receive(const struct spi_port* port, uint8_t* buffer);

/* ============================================================================================== */

int8_t spi1_set_rx_callback(const struct spi_port* port,
                            spi_rx_callback_t      callback,
                            void*                  callback_context);

/* ============================================================================================== */

int8_t spi1_enable_rx_interrupt(const struct spi_port* port, bool enable);

/* ============================================================================================== */

int8_t spi1_flush_tx(const struct spi_port* port);

/* ============================================================================================== */

int8_t spi1_flush_rx(const struct spi_port* port);

/* ============================================================================================== */

#endif  // SPI_DRIVER_H
