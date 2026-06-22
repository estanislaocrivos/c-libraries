#include "../inc/pic18_nvm.h"

#include "../inc/pic18_critical.h"

/* ========================================================================== */

/**
 * @def EEPROM_ERASE_VALUE
 * @brief Value used to erase EEPROM bytes.
 */
#define EEPROM_ERASE_VALUE 0xFFU

/* ========================================================================== */

const struct nvm_ops pic18_nvm_ops = {
    .initialize = pic18_nvm_initialize,
    .read       = pic18_nvm_read,
    .write      = pic18_nvm_write,
    .erase      = pic18_nvm_erase,
};

/* ========================================================================== */

/**
 * @brief Reads a single byte from the EEPROM.
 * @param address Full 24-bit address (base_address | offset).
 * @param data Pointer to store the read byte.
 * @return int8_t Returns 0 on success.
 */
static int8_t eeprom_read_byte(uint32_t address, uint8_t* data)
{
    CRITICAL_SECTION_STATE_T state;
    CRITICAL_SECTION_ENTER(state);
    NVMADRH            = (uint8_t)((address >> 8) & 0x03);
    NVMADRL            = (uint8_t)(address & 0xFF);
    NVMCON1bits.NVMREG = 0;
    NVMCON1bits.RD     = 1;
    NOP(); /* Delay for high clock */
    NOP();
    NOP();
    while (NVMCON1bits.RD == 1)
    {
        /* Wait until read is complete */
    }
    *data = NVMDAT;
    CRITICAL_SECTION_EXIT(state);
    return 0;
}

/* ========================================================================== */

/**
 * @brief Writes a single byte to the EEPROM.
 * @param address Full 24-bit address (base_address | offset).
 * @param data Byte to write.
 * @return int8_t Returns 0 on success, -EIO on write error.
 */
static int8_t eeprom_write_byte(uint32_t address, uint8_t data)
{
    CRITICAL_SECTION_STATE_T state;
    CRITICAL_SECTION_ENTER(state);
    NVMADRH            = (uint8_t)((address >> 8) & 0x03);
    NVMADRL            = (uint8_t)(address & 0xFF);
    NVMDAT             = data;
    NVMCON1bits.NVMREG = 0;
    NVMCON1bits.WREN   = 1;
    INTCON0bits.GIE    = 0;
    NVMCON2            = 0x55;
    NVMCON2            = 0xAA;
    NVMCON1bits.WR     = 1;
    while (NVMCON1bits.WR)
    {
        /* Wait for write to complete */
    }
    NVMCON1bits.WREN = 0;
    if (NVMCON1bits.WRERR == 1)
    {
        CRITICAL_SECTION_EXIT(state);
        return -EIO;
    }
    CRITICAL_SECTION_EXIT(state);
    return 0;
}

/* ========================================================================== */

int8_t pic18_nvm_initialize(struct nvm* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t pic18_nvm_read(
    const struct nvm* self, uint32_t address, uint8_t* buffer, size_t size)
{
    if (self == NULL || buffer == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (size == 0)
    {
        return -EINVAL;
    }
    if (address + size > self->size)
    {
        return -EINVAL;
    }

    uint32_t full_address = self->base_address | address;

    for (size_t i = 0; i < size; i++)
    {
        int8_t result = eeprom_read_byte(full_address + i, &buffer[i]);
        if (result != 0)
        {
            return result;
        }
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_nvm_write(
    const struct nvm* self, uint32_t address, const uint8_t* buffer, size_t size)
{
    if (self == NULL || buffer == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (size == 0)
    {
        return -EINVAL;
    }
    if (address + size > self->size)
    {
        return -EINVAL;
    }

    uint32_t full_address = self->base_address | address;

    for (size_t i = 0; i < size; i++)
    {
        int8_t result = eeprom_write_byte(full_address + i, buffer[i]);
        if (result != 0)
        {
            return result;
        }
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_nvm_erase(const struct nvm* self, uint32_t address, size_t size)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (size == 0)
    {
        return -EINVAL;
    }
    if (address + size > self->size)
    {
        return -EINVAL;
    }

    uint32_t full_address = self->base_address | address;

    for (size_t i = 0; i < size; i++)
    {
        int8_t result = eeprom_write_byte(full_address + i, EEPROM_ERASE_VALUE);
        if (result != 0)
        {
            return result;
        }
    }

    return 0;
}

/* ========================================================================== */
