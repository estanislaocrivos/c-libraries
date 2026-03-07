#ifndef NVM_H
#define NVM_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

/* Forward declaration */
struct nvm;

/* ========================================================================== */

/**
 * @brief NVM operations structure. This structure holds function pointers for
 * non-volatile memory operations.
 */
struct nvm_ops
{
    /**
     * @brief Initializes the NVM interface.
     * @param self Pointer to the NVM structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct nvm* self);

    /**
     * @brief Reads data from the NVM.
     * @param self Pointer to the NVM structure.
     * @param address Offset address within the NVM region.
     * @param buffer Pointer to the buffer where read data will be stored.
     * @param size Number of bytes to read.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*read)(
        const struct nvm* self, uint32_t address, uint8_t* buffer, size_t size);

    /**
     * @brief Writes data to the NVM.
     * @param self Pointer to the NVM structure.
     * @param address Offset address within the NVM region.
     * @param buffer Pointer to the data to be written.
     * @param size Number of bytes to write.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*write)(
        const struct nvm* self,
        uint32_t          address,
        const uint8_t*    buffer,
        size_t            size);

    /**
     * @brief Erases a region of the NVM.
     * @param self Pointer to the NVM structure.
     * @param address Offset address within the NVM region.
     * @param size Number of bytes to erase.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note For EEPROM, this typically writes 0xFF to the region. For flash,
     * this performs a sector/page erase.
     */
    int8_t (*erase)(const struct nvm* self, uint32_t address, size_t size);
};

/* ========================================================================== */

/**
 * @brief NVM interface structure.
 */
struct nvm
{
    /**
     * @brief Base address of the NVM region.
     */
    uint32_t base_address;

    /**
     * @brief Total size of the NVM region in bytes.
     */
    uint32_t size;

    /**
     * @brief Pointer to the NVM operations structure.
     */
    const struct nvm_ops* const ops;

    /* private: internal state - do not access directly */
    bool was_initialized;
};

/* ========================================================================== */

#define NVM_DEFINE(name, ops_ptr, base_addr, mem_size) \
    struct nvm name = {                                \
        .base_address    = (base_addr),                \
        .size            = (mem_size),                 \
        .ops             = (ops_ptr),                  \
        .was_initialized = false,                      \
    }

/* ========================================================================== */

#endif /* NVM_H */
