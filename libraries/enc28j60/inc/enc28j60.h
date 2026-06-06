#ifndef ENC28J60_H
#define ENC28J60_H

#include "spi.h"

#include <stdint.h>

struct enc28j60
{
    uint8_t    id;
    struct spi spi_bus;
}

#endif /* ENC28J60_H */
