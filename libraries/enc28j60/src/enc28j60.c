#include "../inc/enc28j60.h"

#include "../../inc/errno.h"

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

enum enc28j60_mem_bank
{
    BANK_0  = 0x00,
    BANK_1  = 0x01,
    BANK_2  = 0x02,
    BANK_3  = 0x03,
    NO_BANK = 0xFF,
};

typedef uint16_t enc28j60_reg_t;

#define MIREGADR             ((enc28j60_reg_t)(BANK_2 << 8) | 0x14)
#define MIWRL                ((enc28j60_reg_t)(BANK_2 << 8) | 0x16)
#define MIWRH                ((enc28j60_reg_t)(BANK_2 << 8) | 0x17)
#define MISTAT               ((enc28j60_reg_t)(BANK_3 << 8) | 0x0A)

#define MACON1               ((enc28j60_reg_t)(BANK_2 << 8) | 0x00)
#define MACON3               ((enc28j60_reg_t)(BANK_2 << 8) | 0x02)
#define MACON4               ((enc28j60_reg_t)(BANK_2 << 8) | 0x03)
#define MABBIPG              ((enc28j60_reg_t)(BANK_2 << 8) | 0x04)
#define MAIPGL               ((enc28j60_reg_t)(BANK_2 << 8) | 0x06)
#define MAIPGH               ((enc28j60_reg_t)(BANK_2 << 8) | 0x07)
#define MAADR1               ((enc28j60_reg_t)(BANK_3 << 8) | 0x04)
#define MAADR2               ((enc28j60_reg_t)(BANK_3 << 8) | 0x05)
#define MAADR3               ((enc28j60_reg_t)(BANK_3 << 8) | 0x02)
#define MAADR4               ((enc28j60_reg_t)(BANK_3 << 8) | 0x03)
#define MAADR5               ((enc28j60_reg_t)(BANK_3 << 8) | 0x00)
#define MAADR6               ((enc28j60_reg_t)(BANK_3 << 8) | 0x01)

#define ERDPTL               ((enc28j60_reg_t)(BANK_0 << 8) | 0x00)
#define ERDPTH               ((enc28j60_reg_t)(BANK_0 << 8) | 0x01)
#define EWRPTL               ((enc28j60_reg_t)(BANK_0 << 8) | 0x02)
#define EWRPTH               ((enc28j60_reg_t)(BANK_0 << 8) | 0x03)
#define ETXSTL               ((enc28j60_reg_t)(BANK_0 << 8) | 0x04)
#define ETXSTH               ((enc28j60_reg_t)(BANK_0 << 8) | 0x05)
#define ETXNDL               ((enc28j60_reg_t)(BANK_0 << 8) | 0x06)
#define ETXNDH               ((enc28j60_reg_t)(BANK_0 << 8) | 0x07)
#define ERXSTL               ((enc28j60_reg_t)(BANK_0 << 8) | 0x08)
#define ERXSTH               ((enc28j60_reg_t)(BANK_0 << 8) | 0x09)
#define ERXNDL               ((enc28j60_reg_t)(BANK_0 << 8) | 0x0A)
#define ERXNDH               ((enc28j60_reg_t)(BANK_0 << 8) | 0x0B)
#define ERXRDPTL             ((enc28j60_reg_t)(BANK_0 << 8) | 0x0C)
#define ERXRDPTH             ((enc28j60_reg_t)(BANK_0 << 8) | 0x0D)
#define EREVID               ((enc28j60_reg_t)(BANK_3 << 8) | 0x12)
#define EPKTCNT              ((enc28j60_reg_t)(BANK_1 << 8) | 0x19)
#define EIE                  ((enc28j60_reg_t)(NO_BANK << 8) | 0x1B)
#define EIR                  ((enc28j60_reg_t)(NO_BANK << 8) | 0x1C)
#define ESTAT                ((enc28j60_reg_t)(NO_BANK << 8) | 0x1D)
#define ECON2                ((enc28j60_reg_t)(NO_BANK << 8) | 0x1E)
#define ECON1                ((enc28j60_reg_t)(NO_BANK << 8) | 0x1F)

/* ========================================================================== */

#define ENC28J60_RXBUF_START 0x0000U
#define ENC28J60_TXBUF_END   0x1FFFU

/* ========================================================================== */

enum enc28j60_instruction_opcode
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

static int8_t enc28j60_select_bank(
    struct enc28j60* self, enum enc28j60_mem_bank bank)
{
    if (bank == NO_BANK)
    {
        return 0;
    }
    if (bank == self->private.current_mem_bank)
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

    self->private.current_mem_bank = (uint8_t)bank;
    return 0;
}

/* ========================================================================== */

static int8_t enc28j60_write_register(
    const struct enc28j60* self, uint16_t address, uint8_t value)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    int8_t                 ret  = 0;
    enum enc28j60_mem_bank bank = (enum enc28j60_mem_bank)(address >> 8);
    enc28j60_select_bank(self, bank);

    uint8_t tx_payload[2] = {(uint8_t)((WRITE_CTRL_REG << 5) | address), value};
    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transmit(
            self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        ret = -EIO;
        goto done;
    }

done:
    self->spi_cs->ops->set_state(self->spi_cs, true);
    return ret;
}

/* ========================================================================== */

static int8_t enc28j60_read_eth_register(
    const struct enc28j60* self, uint16_t address, uint8_t* value)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    int8_t                 ret  = 0;
    enum enc28j60_mem_bank bank = (enum enc28j60_mem_bank)(address >> 8);
    enc28j60_select_bank(self, bank);

    uint8_t tx_payload[] = {(uint8_t)((READ_CTRL_REG << 5) | address), 0x00};
    uint8_t rx_payload[2];

    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transfer(
            self->spi_bus, tx_payload, rx_payload, sizeof(rx_payload)))
    {
        ret = -EIO;
        goto done;
    }
    *value = rx_payload[1];

done:
    self->spi_cs->ops->set_state(self->spi_cs, true);
    return ret;
}

/* ========================================================================== */

/* Set bit through hardware operations. Only available for ETH registers. */
static int8_t enc28j60_set_eth_bit(
    const struct enc28j60* self, uint16_t address, uint8_t mask, bool state)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    enc28j60_select_bank(self, (enum enc28j60_mem_bank)(address >> 8));

    int8_t  ret = 0;
    uint8_t tx_payload[2];
    if (state)
    {
        tx_payload[0] = ((BIT_FIELD_SET << 5) | (uint8_t)address);
    }
    else
    {
        tx_payload[0] = (BIT_FIELD_CLEAR << 5) | (uint8_t)address;
    }

    tx_payload[1] = mask;

    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transmit(
            self->spi_bus, tx_payload, sizeof(tx_payload)))
    {
        ret = -EIO;
        goto done;
    }

done:
    self->spi_cs->ops->set_state(self->spi_cs, true);
    return ret;
}

/* ========================================================================== */

static int8_t enc28j60_read_register(
    const struct enc28j60* self, uint16_t address, uint8_t* value)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    int8_t                 ret  = 0;
    enum enc28j60_mem_bank bank = (enum enc28j60_mem_bank)(address >> 8);
    enc28j60_select_bank(self, bank);

    /* Need to exchange 3 bytes */
    uint8_t tx_payload[]
        = {(uint8_t)((READ_CTRL_REG << 5) | address), 0x00, 0x00};
    uint8_t rx_payload[3];

    self->spi_cs->ops->set_state(self->spi_cs, false);
    if (self->spi_bus->ops->transfer(
            self->spi_bus, tx_payload, rx_payload, sizeof(rx_payload)))
    {
        ret = -EIO;
        goto done;
    }
    *value = rx_payload[2];

done:
    self->spi_cs->ops->set_state(self->spi_cs, true);
    return ret;
}

/* ========================================================================== */

/* Set bit through software operations. Available for all registers. */
static int8_t enc28j60_set_bit(
    const struct enc28j60* self, uint16_t address, uint8_t mask, bool state)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    enc28j60_select_bank(self, (enum enc28j60_mem_bank)(address >> 8));

    uint8_t value = 0;
    enc28j60_read_register(self, address, &value);

    if (state)
    {
        value |= mask;
    }
    else
    {
        value &= ~mask;
    }

    enc28j60_write_register(self, address, value);
    return 0;
}

/* ========================================================================== */

static int8_t enc28j60_get_bit(
    const struct enc28j60* self, uint16_t address, uint8_t mask, bool* state)
{
    uint8_t value = 0;
    enc28j60_read_register(self, address, &value);

    *state = (bool)(value & mask);
    return 0;
}

/* ========================================================================== */

static int8_t enc28j60_mac_init(const struct enc28j60* self)
{
    enc28j60_set_bit(self, MACON1, 0x01, true);
    enc28j60_set_bit(self, MACON3, 0xF0, true);
    enc28j60_set_bit(self, MACON4, 0x40, true);

    enc28j60_write_register(self, MABBIPG, 0x12);
    enc28j60_write_register(self, MAIPGL, 0x12);
    enc28j60_write_register(self, MAIPGH, 0x0C);

    enc28j60_write_register(self, MAADR1, self->mac_address[0]);
    enc28j60_write_register(self, MAADR2, self->mac_address[1]);
    enc28j60_write_register(self, MAADR3, self->mac_address[2]);
    enc28j60_write_register(self, MAADR4, self->mac_address[3]);
    enc28j60_write_register(self, MAADR5, self->mac_address[4]);
    enc28j60_write_register(self, MAADR6, self->mac_address[5]);
    return 0;
}

static int8_t enc28j60_phy_init(const struct enc28j60* self)
{
    /* Configure PHY for half-duplex operation */
    enc28j60_write_register(self, MIREGADR, 0x10);
    enc28j60_write_register(self, MIWRL, 0x00);
    enc28j60_write_register(self, MIWRH, 0x01);

    /* Read busy bit */
    bool state = true;
    while (state)
    {
        enc28j60_get_bit(self, MISTAT, 0x01, &state);
    }
    return 0;
}

static int8_t enc28j60_rxbuf_init(const struct enc28j60* self)
{
    enc28j60_write_register(
        self, ERXSTL, (uint8_t)(ENC28J60_RXBUF_START & 0xFF));
    enc28j60_write_register(self, ERXSTH, (uint8_t)(ENC28J60_RXBUF_START >> 8));
    enc28j60_write_register(
        self, ERXNDL, (uint8_t)(self->rx_buf_end_addr & 0xFF));
    enc28j60_write_register(
        self, ERXNDH, (uint8_t)(self->rx_buf_end_addr >> 8));
    enc28j60_write_register(
        self, ERXRDPTL, (uint8_t)(self->rx_buf_end_addr & 0xFF));
    enc28j60_write_register(
        self, ERXRDPTH, (uint8_t)(self->rx_buf_end_addr >> 8));
    return 0;
}

static int8_t enc28j60_txbuf_init(const struct enc28j60* self)
{
    enc28j60_write_register(
        self, ETXSTL, (uint8_t)((self->rx_buf_end_addr + 1) & 0xFF));
    enc28j60_write_register(
        self, ETXSTH, (uint8_t)((self->rx_buf_end_addr + 1) >> 8));
    enc28j60_write_register(
        self, EWRPTL, (uint8_t)((self->rx_buf_end_addr + 1) & 0xFF));
    enc28j60_write_register(
        self, EWRPTH, (uint8_t)((self->rx_buf_end_addr + 1) >> 8));
    return 0;
}

/* ========================================================================== */

int8_t enc28j60_init(struct enc28j60* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (self->spi_bus == NULL || self->spi_cs == NULL)
    {
        return -EFAULT;
    }
    enc28j60_rxbuf_init(self);
    enc28j60_txbuf_init(self);
    enc28j60_mac_init(self);
    enc28j60_phy_init(self);
    enc28j60_set_eth_bit(self, ECON1, 0x04, true);
    self->private.erdpt       = ENC28J60_RXBUF_START;
    self->private.initialized = true;
    return 0;
}

int8_t enc28j60_reset(const struct enc28j60* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    self->spi_cs->ops->set_state(self->spi_cs, false);

    /* Issue RESET command */
    uint8_t reset_cmd[] = {(uint8_t)((SYSTEM_RESET << 5) | 0x1F)};
    self->spi_bus->ops->transmit(self->spi_bus, reset_cmd, sizeof(reset_cmd));

    self->spi_cs->ops->set_state(self->spi_cs, true);
    return 0;
}

int8_t enc28j60_get_epktcnt(const struct enc28j60* self, uint8_t* epktcnt)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->private.initialized)
    {
        return -EPERM;
    }

    uint8_t value = 0;
    if (enc28j60_read_eth_register(self, EPKTCNT, &value))
    {
        return -EIO;
    }
    *epktcnt = value;
    return 0;
}

int8_t enc28j60_receive_packet(
    struct enc28j60* self, uint8_t* buffer, uint16_t* size)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->private.initialized)
    {
        return -EPERM;
    }

    uint8_t epktcnt = 0;
    enc28j60_get_epktcnt(self, &epktcnt);
    if (epktcnt == 0)
    {
        return -ENODATA;
    }

    enc28j60_write_register(
        self, ERDPTL, (uint8_t)(self->private.erdpt & 0xFF));
    enc28j60_write_register(self, ERDPTH, (uint8_t)(self->private.erdpt >> 8));

    self->spi_cs->ops->set_state(self->spi_cs, false);

    /* Issue READ BUF MEM command */
    uint8_t read_buf_mem_cmd[] = {(uint8_t)((READ_BUF_MEM << 5) | 0x1A)};
    self->spi_bus->ops->transmit(
        self->spi_bus, read_buf_mem_cmd, sizeof(read_buf_mem_cmd));

    /* Read first 6 bytes corresponding to next pkt pointer and receive status
     * vector */
    uint8_t rx_buffer[6] = {};
    self->spi_bus->ops->transfer(
        self->spi_bus, rx_buffer, rx_buffer, sizeof(rx_buffer));

    uint16_t next_pkt_ptr = ((uint16_t)(rx_buffer[1]) << 8)
                            | (uint16_t)rx_buffer[0];

    uint16_t eth_pkt_size = ((uint16_t)(rx_buffer[3]) << 8)
                            | (uint16_t)rx_buffer[2];

    self->spi_bus->ops->transfer(self->spi_bus, buffer, buffer, eth_pkt_size);

    self->spi_cs->ops->set_state(self->spi_cs, true);

    /* Update ERXRDPT (must be odd per silicon errata) */
    uint16_t erxrdpt = (next_pkt_ptr == ENC28J60_RXBUF_START) ?
                           self->rx_buf_end_addr :
                           next_pkt_ptr - 1;
    enc28j60_write_register(self, ERXRDPTL, (uint8_t)(erxrdpt & 0xFF));
    enc28j60_write_register(self, ERXRDPTH, (uint8_t)(erxrdpt >> 8));

    self->private.erdpt = next_pkt_ptr;

    enc28j60_set_eth_bit(self, ECON2, 0x40, true);

    *size = eth_pkt_size;

    return 0;
}

int8_t enc28j60_transmit_packet(
    struct enc28j60* self, uint8_t* frame, uint16_t size)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->private.initialized)
    {
        return -EPERM;
    }

    uint16_t tx_start = self->rx_buf_end_addr + 1;
    uint16_t tx_end   = tx_start + size;

    enc28j60_write_register(self, EWRPTL, (uint8_t)(tx_start & 0xFF));
    enc28j60_write_register(self, EWRPTH, (uint8_t)(tx_start >> 8));

    enc28j60_write_register(self, ETXNDL, (uint8_t)(tx_end & 0xFF));
    enc28j60_write_register(self, ETXNDH, (uint8_t)(tx_end >> 8));

    self->spi_cs->ops->set_state(self->spi_cs, false);

    uint8_t write_buf_mem_cmd[] = {(uint8_t)((WRITE_BUF_MEM << 5) | 0x1A)};
    self->spi_bus->ops->transmit(
        self->spi_bus, write_buf_mem_cmd, sizeof(write_buf_mem_cmd));

    uint8_t ctrl_byte = 0x00;
    self->spi_bus->ops->transmit(self->spi_bus, &ctrl_byte, 1);

    self->spi_bus->ops->transmit(self->spi_bus, frame, size);

    self->spi_cs->ops->set_state(self->spi_cs, true);

    enc28j60_set_eth_bit(self, ECON1, 0x08, true);

    bool txrts  = true;
    bool txerif = false;
    while (txrts)
    {
        enc28j60_get_bit(self, ECON1, 0x08, &txrts);
        enc28j60_get_bit(self, EIR, 0x02, &txerif);
        if (txerif)
        {
            /* Errata #12: reset transmitter, clear error flag, retry once */
            enc28j60_set_eth_bit(self, ECON1, 0x80, true);
            enc28j60_set_eth_bit(self, ECON1, 0x80, false);
            enc28j60_set_eth_bit(self, EIR, 0x02, false);

            /* Retry */
            enc28j60_set_eth_bit(self, ECON1, 0x08, true);
            txerif = false;
        }
    }

    return 0;
}

/* ========================================================================== */
