#include "../inc/enc28j60.h"

#include "../../inc/errno.h"

#include <stdint.h>

/* ========================================================================== */

enum ENC28J60_MEM_BANK
{
    BANK_0  = 0x00,
    BANK_1  = 0x01,
    BANK_2  = 0x02,
    BANK_3  = 0x03,
    NO_BANK = 0xFF,
};

enum ENC28J60_INSTRUC_OP_CODE
{
    READ_CTRL_REG   = 0x00,
    READ_BUF_MEM    = 0x01,
    WRITE_CTRL_REG  = 0x02,
    WRITE_BUF_MEM   = 0x03,
    BIT_FIELD_SET   = 0x04,
    BIT_FIELD_CLEAR = 0x05,
    SYSTEM_RESET    = 0x07,
};

/* ========================================================================== */

typedef uint16_t enc28j60_reg_t;

#define EIE   ((enc28j60_reg_t)(NO_BANK << 8) | 0x1B)
#define EIR   ((enc28j60_reg_t)(NO_BANK << 8) | 0x1C)
#define ESTAT ((enc28j60_reg_t)(NO_BANK << 8) | 0x1D)
#define ECON2 ((enc28j60_reg_t)(NO_BANK << 8) | 0x1E)
#define ECON1 ((enc28j60_reg_t)(NO_BANK << 8) | 0x1F)

/* ========================================================================== */

static enum ENC28J60_MEM_BANK current_bank = 0;

/* ========================================================================== */

static int8_t enc28j60_select_bank(
    const struct enc28j60* self, enum ENC28J60_MEM_BANK bank)
{
    if (bank == NO_BANK)
    {
        return 0;
    }
    if (bank == current_bank)
    {
        return 0;
    }

    /* First clear last two bits of ECON1 */
    uint8_t tx_payload[2];
    tx_payload[0] = (BIT_FIELD_CLEAR << 5) | (uint8_t)ECON1;
    tx_payload[1] = 0x03; /* Clear only last to bits (the chip does ~& op.) */
    if (self->spi_bus.ops->transmit(
            &self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        return -EIO;
    }

    /* Now write the last two bits of ECON1 */
    tx_payload[0] = ((BIT_FIELD_SET << 5) | (uint8_t)ECON1);
    tx_payload[1] = (bank & 0x03); /* Only allow 0-3 values */
    if (self->spi_bus.ops->transmit(
            &self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        return -EIO;
    }

    current_bank = bank;
    return 0;
}

/* ========================================================================== */

int8_t write_register(
    const struct enc28j60* self, uint16_t address, uint8_t value)
{
    uint8_t bank = (uint8_t)(address >> 8);
    enc28j60_select_bank(self, bank);

    uint8_t tx_payload[2] = {(WRITE_CTRL_REG << 5) | address, value};
    if (self->spi_bus.ops->transmit(
            &self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        return -EIO;
    }
    return 0;
}

/* ========================================================================== */

int8_t read_register(
    const struct enc28j60* self, uint16_t address, uint8_t* value)
{
    uint8_t bank = (uint8_t)(address >> 8);
    enc28j60_select_bank(self, bank);

    uint8_t tx_payload[] = {(READ_CTRL_REG << 5) | address, 0x00};
    uint8_t rx_payload[2];

    if (self->spi_bus.ops->transfer(
            &self->spi_bus, tx_payload, rx_payload, sizeof(rx_payload)))
    {
        return -EIO;
    }
    *value = rx_payload[1];
    return 0;
}

/* ========================================================================== */
