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

#define EREVID ((enc28j60_reg_t)(BANK_3 << 8) | 0x12)
#define EIE    ((enc28j60_reg_t)(NO_BANK << 8) | 0x1B)
#define EIR    ((enc28j60_reg_t)(NO_BANK << 8) | 0x1C)
#define ESTAT  ((enc28j60_reg_t)(NO_BANK << 8) | 0x1D)
#define ECON2  ((enc28j60_reg_t)(NO_BANK << 8) | 0x1E)
#define ECON1  ((enc28j60_reg_t)(NO_BANK << 8) | 0x1F)

/* ========================================================================== */

struct enc28j60
{
    const struct gpio* const spi_cs;
    const struct spi* const  spi_bus;
};

/* ========================================================================== */

int8_t enc28j60_init(const struct enc28j60* self);

int8_t enc28j60_write_register(
    const struct enc28j60* self, uint16_t address, uint8_t value);

int8_t enc28j60_read_register(
    const struct enc28j60* self, uint16_t address, uint8_t* value);

/* ========================================================================== */

#endif /* ENC28J60_H */
