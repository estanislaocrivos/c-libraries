#ifndef SPI_H
#define SPI_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Prototype for the SPI receive callback function.
 * @param buffer Pointer to the received data buffer.
 * @param length Length of the received data.
 * @param callback_context Pointer to user-defined callback_context data (can be NULL).
 */
typedef void (*spi_rx_callback_t)(void*          callback_context,
                                  const uint8_t* buffer,
                                  size_t         buffer_size);

/* ============================================================================================== */

/**
 * @brief SPI port structure. This structure holds the configuration and state of a SPI port.
 */
struct spi_port;

/**
 * @brief SPI operations structure. This structure holds function pointers for SPI operations.
 */
struct spi_ops
{
    /**
     * @brief Initializes the SPI interface.
     * @param self Pointer to the SPI port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct spi_port* self);

    /**
     * @brief Transmits data through the SPI interface.
     * @param self Pointer to the SPI port structure.
     * @param buffer Pointer to the 8-bit buffer to be transmitted.
     * @param size Size of the buffer to be transmitted.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*transmit)(struct spi_port* self, const uint8_t* buffer, size_t size);

    /**
     * @brief Receives data through the SPI interface.
     * @param self Pointer to the SPI port structure.
     * @param buffer Pointer to the 8-bit buffer where the received data will be stored.
     * @param size Size of the buffer to be received.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*receive)(struct spi_port* self, uint8_t* buffer, size_t size);

    /**
     * @brief Sets the callback function to be called when a full word has been received.
     * @param self Pointer to the SPI port structure.
     * @param callback The callback function to set. Its prototype must match the spi_rx_callback_t
     * type.
     * @param callback_context Pointer to user-defined callback_context data (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_rx_callback)(struct spi_port*  self,
                              spi_rx_callback_t callback,
                              void*             callback_context);

    /**
     * @brief Enables or disables the SPI receive interrupt.
     * @param self Pointer to the SPI port structure.
     * @param enable Set to true to enable the interrupt, false to disable it.
     */
    void (*enable_rx_interrupt)(struct spi_port* self, bool enable);

    /**
     * @brief Clears the SPI buffers.
     * @param self Pointer to the SPI port structure.
     */
    void (*clear_buffers)(struct spi_port* self);
};

/* ============================================================================================== */

struct spi_port
{
    /**
     * @brief SPI port identifier (e.g., 0 for SPI1, 1 for SPI2, etc.).
     */
    uint8_t port_id;

    /**
     * @brief SPI mode (0-3).
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
     * @brief Pointer to the receive buffer. This buffer must be created by the user, assigned, and
     * kept alive throughout the lifetime of the SPI port.
     */
    uint8_t* rx_buffer;

    /**
     * @brief Size of the receive buffer in bytes. This size must match the actual size of the
     * buffer created by the user.
     */
    size_t rx_buffer_size;

    /**
     * @brief Pointer to the user-defined callback_context data (can be NULL).
     */
    void* callback_context;

    /**
     * @brief Pointer to the SPI operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct spi_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct spi_ops* ops;
};

/* ============================================================================================== */

#endif  // SPI_H
