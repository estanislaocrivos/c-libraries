#ifndef SPI_H
#define SPI_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

/**
 * @brief Prototype for the SPI receive callback function.
 * @param callback_context Pointer to user-defined callback_context data (can be
 * NULL).
 * @param byte The received byte.
 */
typedef void (*spi_rx_callback_t)(void* callback_context, uint8_t byte);

/* ========================================================================== */

/**
 * @brief SPI interface structure.
 */
struct spi;

/**
 * @brief SPI operations structure. This structure holds function pointers for
 * SPI operations.
 */
struct spi_ops
{
    /**
     * @brief Initializes the SPI interface.
     * @param self Pointer to the SPI port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct spi* self);

    /**
     * @brief Transmits data through the SPI interface.
     * @param self Pointer to the SPI port structure.
     * @param buffer Pointer to the 8-bit buffer to be transmitted.
     * @param size Size of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*transmit)(
        const struct spi* self, const uint8_t* buffer, size_t size);

    /**
     * @brief Receives data through the SPI interface.
     * @param self Pointer to the SPI port structure.
     * @param data Pointer to the data byte where the received data will be
     * stored.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*receive)(const struct spi* self, uint8_t* data);

    /**
     * @brief Performs a full-duplex transfer (simultaneous transmit and
     * receive).
     * @param self Pointer to the SPI port structure.
     * @param tx_buffer Pointer to transmit buffer.
     * @param rx_buffer Pointer to receive buffer.
     * @param size Number of bytes to transfer.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*transfer)(
        const struct spi* self,
        const uint8_t*    tx_buffer,
        uint8_t*          rx_buffer,
        size_t            size);

    /**
     * @brief Controls the Chip Select (CS) line manually.
     * @param self Pointer to the SPI port structure.
     * @param active true to activate CS, false to deactivate.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*set_cs)(const struct spi* self, bool active);

    /**
     * @brief Changes SPI frequency dynamically (useful for multi-device
     * communication).
     * @param self Pointer to the SPI port structure.
     * @param frequency New frequency in Hz.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*set_frequency)(const struct spi* self, uint32_t frequency);

    /**
     * @brief Sets the callback function to be called when a full word has been
     * received.
     * @param self Pointer to the SPI port structure.
     * @param callback The callback function to set. Its prototype must match
     * the spi_rx_callback_t type.
     * @param callback_context Pointer to user-defined callback_context data
     * (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_callback)(
        const struct spi* self,
        spi_rx_callback_t callback,
        void*             callback_context);

    /**
     * @brief Enables or disables the SPI receive interrupt.
     * @param self Pointer to the SPI port structure.
     * @param enable Set to true to enable the interrupt, false to disable it.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*enable_rx_interrupt)(const struct spi* self, bool enable);

    /**
     * @brief Flushes the transmit buffer.
     * @param self Pointer to the SPI structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*flush_tx)(const struct spi* self);

    /**
     * @brief Flushes the receive buffer.
     * @param self Pointer to the SPI structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*flush_rx)(const struct spi* self);
};

/* ========================================================================== */

struct spi
{
    /**
     * @brief SPI mode (0-3): CPOL and CPHA configuration.
     * Mode 0: CPOL=0, CPHA=0
     * Mode 1: CPOL=0, CPHA=1
     * Mode 2: CPOL=1, CPHA=0
     * Mode 3: CPOL=1, CPHA=1
     */
    uint8_t mode;

    /**
     * @brief Indicates whether the SPI port is in Master or Slave mode.
     */
    bool master;

    /**
     * @brief Frequency for SPI communication.
     */
    uint32_t frequency;

    /**
     * @brief Bit order: true for MSB first, false for LSB first.
     */
    bool msb_first;

    /**
     * @brief Chip Select (CS) polarity: true for active high, false for active
     * low.
     */
    bool cs_active_high;

    /**
     * @brief Enable/disable hardware CS control. If false, user must control CS
     * manually.
     */
    bool hardware_cs;

    /**
     * @brief Pointer to the SPI operations structure.
     */
    const struct spi_ops* const ops;

    /* private: internal state - do not access directly */
    bool  was_initialized;
    void* callback_context;
};

/* ========================================================================== */

#endif  // SPI_H
