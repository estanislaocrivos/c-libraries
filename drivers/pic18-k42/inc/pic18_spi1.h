/**
 * @file
 * @brief PIC18-K42 SPI1 driver header file. This driver implements the spi.h
 * interface for the PIC18-K42 SPI1 peripheral. Supports both master and slave
 * modes with configurable clock polarity, phase, bit order, and frequency.
 */

#ifndef PIC18_SPI1_H
#define PIC18_SPI1_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "errno.h"
#include "pic18_k42.h"
#include "spi.h"

/* ========================================================================== */

/**
 * @brief SPI1 operations structure for use with the spi interface.
 */
extern const struct spi_ops pic18_spi1_ops;

/* ========================================================================== */

/**
 * @brief Initializes the SPI1 interface.
 *
 * Configures the SPI1 peripheral based on the settings in the spi structure:
 * - mode: SPI mode (0-3) for CPOL/CPHA configuration
 * - master: true for master mode, false for slave mode
 * - frequency: Clock frequency in Hz (master mode only)
 * - msb_first: true for MSB first, false for LSB first
 *
 * @param self Pointer to the SPI port structure.
 * @return 0 on success, -EFAULT if NULL, -EINVAL if invalid configuration.
 */
int8_t pic18_spi1_initialize(struct spi* self);

/* ========================================================================== */

/**
 * @brief Transmits data through the SPI1 interface (blocking).
 *
 * In master mode, generates clock and sends data.
 * In slave mode, loads TX buffer and waits for master clock.
 *
 * @param self Pointer to the SPI port structure.
 * @param buffer Pointer to the buffer to be transmitted.
 * @param size Number of bytes to transmit.
 * @return 0 on success, -EFAULT if NULL, -EPERM if not initialized.
 */
int8_t pic18_spi1_transmit(
    const struct spi* self, const uint8_t* buffer, size_t size);

/* ========================================================================== */

/**
 * @brief Receives a single byte through the SPI1 interface.
 *
 * In master mode, transmits a dummy byte to generate clock.
 *
 * @param self Pointer to the SPI port structure.
 * @param data Pointer to store the received byte.
 * @return 0 on success, -EFAULT if NULL, -EPERM if not initialized.
 */
int8_t pic18_spi1_receive(const struct spi* self, uint8_t* data);

/* ========================================================================== */

/**
 * @brief Performs a full-duplex SPI transfer.
 *
 * Simultaneously transmits from tx_buffer and receives into rx_buffer.
 *
 * @param self Pointer to the SPI port structure.
 * @param tx_buffer Pointer to transmit buffer.
 * @param rx_buffer Pointer to receive buffer.
 * @param size Number of bytes to transfer.
 * @return 0 on success, -EFAULT if NULL, -EPERM if not initialized.
 */
int8_t pic18_spi1_transfer(
    const struct spi* self,
    const uint8_t*    tx_buffer,
    uint8_t*          rx_buffer,
    size_t            size);

/* ========================================================================== */

/**
 * @brief Controls the Chip Select (CS) line.
 *
 * @note This function is a placeholder. CS control should be handled
 * externally via GPIO for flexibility with multiple devices.
 *
 * @param self Pointer to the SPI port structure.
 * @param active true to activate CS, false to deactivate.
 * @return 0 on success, -EFAULT if NULL.
 */
int8_t pic18_spi1_set_cs(const struct spi* self, bool active);

/* ========================================================================== */

/**
 * @brief Changes SPI1 clock frequency dynamically.
 *
 * Only applicable in master mode.
 *
 * @param self Pointer to the SPI port structure.
 * @param frequency New frequency in Hz.
 * @return 0 on success, -EFAULT if NULL, -EPERM if not master mode.
 */
int8_t pic18_spi1_set_frequency(const struct spi* self, uint32_t frequency);

/* ========================================================================== */

/**
 * @brief Sets the RX callback function.
 *
 * The callback is invoked from the RX ISR when a byte is received.
 *
 * @param self Pointer to the SPI port structure.
 * @param callback Callback function (can be NULL to disable).
 * @param callback_context User context passed to callback.
 * @return 0 on success, -EFAULT if self is NULL.
 */
int8_t pic18_spi1_set_rx_callback(
    const struct spi* self, spi_rx_callback_t callback, void* callback_context);

/* ========================================================================== */

/**
 * @brief Enables or disables the SPI1 RX interrupt.
 *
 * @param self Pointer to the SPI port structure.
 * @param enable true to enable, false to disable.
 * @return 0 on success, -EFAULT if NULL.
 */
int8_t pic18_spi1_enable_rx_interrupt(const struct spi* self, bool enable);

/* ========================================================================== */

/**
 * @brief Flushes the SPI1 transmit buffer.
 *
 * @param self Pointer to the SPI port structure.
 * @return 0 on success, -EFAULT if NULL.
 */
int8_t pic18_spi1_flush_tx(const struct spi* self);

/* ========================================================================== */

/**
 * @brief Flushes the SPI1 receive buffer.
 *
 * @param self Pointer to the SPI port structure.
 * @return 0 on success, -EFAULT if NULL.
 */
int8_t pic18_spi1_flush_rx(const struct spi* self);

/* ========================================================================== */

#endif /* PIC18_SPI1_H */
