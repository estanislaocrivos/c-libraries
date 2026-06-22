#ifndef PIC18_NVM_H
#define PIC18_NVM_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "errno.h"
#include "nvm.h"
#include "pic18_k42.h"

/* ========================================================================== */

/**
 * @def PIC18_EEPROM_START_ADDRESS
 * @brief Starting address of the EEPROM on PIC18-K42 devices.
 */
#define PIC18_EEPROM_START_ADDRESS 0x310000U

/**
 * @def PIC18_EEPROM_SIZE
 * @brief Size of the EEPROM in bytes on PIC18-K42 devices.
 */
#define PIC18_EEPROM_SIZE          1024U

/* ========================================================================== */

/**
 * @brief NVM operations for PIC18-K42 EEPROM.
 */
extern const struct nvm_ops pic18_nvm_ops;

/* ========================================================================== */

/**
 * @brief Initializes the PIC18 NVM interface.
 * @param self Pointer to the NVM structure.
 * @return int8_t Returns 0 on success or -EFAULT if self is NULL.
 */
int8_t pic18_nvm_initialize(struct nvm* self);

/**
 * @brief Reads data from the PIC18 EEPROM.
 * @param self Pointer to the NVM structure.
 * @param address Offset address within the EEPROM region.
 * @param buffer Pointer to the buffer where read data will be stored.
 * @param size Number of bytes to read.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_nvm_read(
    const struct nvm* self, uint32_t address, uint8_t* buffer, size_t size);

/**
 * @brief Writes data to the PIC18 EEPROM.
 * @param self Pointer to the NVM structure.
 * @param address Offset address within the EEPROM region.
 * @param buffer Pointer to the data to be written.
 * @param size Number of bytes to write.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_nvm_write(
    const struct nvm* self,
    uint32_t          address,
    const uint8_t*    buffer,
    size_t            size);

/**
 * @brief Erases a region of the PIC18 EEPROM by writing 0xFF.
 * @param self Pointer to the NVM structure.
 * @param address Offset address within the EEPROM region.
 * @param size Number of bytes to erase.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_nvm_erase(const struct nvm* self, uint32_t address, size_t size);

/* ========================================================================== */

#endif /* PIC18_NVM_H */
