#include "../inc/enc28j60.h"

#include "../../inc/errno.h"

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

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

    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transmit(
            self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        self->spi_cs->ops->set_state(self->spi_cs, true);
        return -EIO;
    }
    self->spi_cs->ops->set_state(self->spi_cs, true);

    /* Now write the last two bits of ECON1 */
    tx_payload[0] = ((BIT_FIELD_SET << 5) | (uint8_t)ECON1);
    tx_payload[1] = (bank & 0x03); /* Only allow 0-3 values */

    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transmit(
            self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        self->spi_cs->ops->set_state(self->spi_cs, true);
        return -EIO;
    }
    self->spi_cs->ops->set_state(self->spi_cs, true);

    current_bank = bank;
    return 0;
}

/* ========================================================================== */

int8_t enc28j60_write_register(
    const struct enc28j60* self, uint16_t address, uint8_t value)
{
    uint8_t bank = (uint8_t)(address >> 8);
    enc28j60_select_bank(self, bank);

    uint8_t tx_payload[2] = {(uint8_t)((WRITE_CTRL_REG << 5) | address), value};
    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transmit(
            self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        self->spi_cs->ops->set_state(self->spi_cs, true);
        return -EIO;
    }
    self->spi_cs->ops->set_state(self->spi_cs, true);
    return 0;
}

/* ========================================================================== */

int8_t enc28j60_read_register(
    const struct enc28j60* self, uint16_t address, uint8_t* value)
{
    uint8_t bank = (uint8_t)(address >> 8);
    enc28j60_select_bank(self, bank);

    uint8_t tx_payload[] = {(uint8_t)((READ_CTRL_REG << 5) | address), 0x00};
    uint8_t rx_payload[2];

    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transfer(
            self->spi_bus, tx_payload, rx_payload, sizeof(rx_payload)))
    {
        self->spi_cs->ops->set_state(self->spi_cs, true);
        return -EIO;
    }
    self->spi_cs->ops->set_state(self->spi_cs, true);
    *value = rx_payload[1];
    return 0;
}

/* ========================================================================== */

int8_t enc28j60_init(const struct enc28j60* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (self->spi_bus == NULL || self->spi_cs == NULL)
    {
        return -EFAULT;
    }
}
