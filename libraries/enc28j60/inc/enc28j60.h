#ifndef ENC28J60_H
#define ENC28J60_H

#include "../../embedded-hal/inc/spi.h"

#include <stdint.h>

struct enc28j60
{
    struct spi spi_bus;
};

int8_t read_raw_register(
    const struct enc28j60* self, uint8_t address, uint8_t* value);

#endif /* ENC28J60_H */
