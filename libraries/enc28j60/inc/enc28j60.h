#ifndef ENC28J60_H
#define ENC28J60_H

#include "../../embedded-hal/inc/gpio.h"
#include "../../embedded-hal/inc/spi.h"

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

typedef uint16_t enc28j60_reg_t;

#define MACON1  ((enc28j60_reg_t)(BANK_2 << 8) | 0x00)
#define MACON3  ((enc28j60_reg_t)(BANK_2 << 8) | 0x02)
#define MACON4  ((enc28j60_reg_t)(BANK_2 << 8) | 0x03)
#define MABBIPG ((enc28j60_reg_t)(BANK_2 << 8) | 0x04)
#define MAIPGL  ((enc28j60_reg_t)(BANK_2 << 8) | 0x06)
#define MAIPGH  ((enc28j60_reg_t)(BANK_2 << 8) | 0x07)
#define MAADR1  ((enc28j60_reg_t)(BANK_3 << 8) | 0x04)
#define MAADR2  ((enc28j60_reg_t)(BANK_3 << 8) | 0x05)
#define MAADR3  ((enc28j60_reg_t)(BANK_3 << 8) | 0x02)
#define MAADR4  ((enc28j60_reg_t)(BANK_3 << 8) | 0x03)
#define MAADR5  ((enc28j60_reg_t)(BANK_3 << 8) | 0x00)
#define MAADR6  ((enc28j60_reg_t)(BANK_3 << 8) | 0x01)

#define ERDPTL  ((enc28j60_reg_t)(BANK_0 << 8) | 0x00)
#define EREVID  ((enc28j60_reg_t)(BANK_3 << 8) | 0x12)
#define EIE     ((enc28j60_reg_t)(NO_BANK << 8) | 0x1B)
#define EIR     ((enc28j60_reg_t)(NO_BANK << 8) | 0x1C)
#define ESTAT   ((enc28j60_reg_t)(NO_BANK << 8) | 0x1D)
#define ECON2   ((enc28j60_reg_t)(NO_BANK << 8) | 0x1E)
#define ECON1   ((enc28j60_reg_t)(NO_BANK << 8) | 0x1F)

/* ========================================================================== */

struct enc28j60
{
    const struct gpio* const spi_cs;
    const struct spi* const  spi_bus;
    const uint8_t            mac_address[6];
    bool                     was_initialized;
};

/* ========================================================================== */

int8_t enc28j60_init(struct enc28j60* self);

int8_t enc28j60_write_register(
    const struct enc28j60* self, uint16_t address, uint8_t value);

int8_t enc28j60_read_register(
    const struct enc28j60* self, uint16_t address, uint8_t* value);

static int8_t enc28j60_set_bit(
    const struct enc28j60* self, uint16_t address, uint8_t mask, bool state);

/* ========================================================================== */

#endif /* ENC28J60_H */
