#include "../inc/enc28j60.h"

#include "../../inc/errno.h"

#include <stdint.h>

static uint8_t tx_buffer[32];
static uint8_t rx_buffer[32];

int8_t read_raw_register(
    const struct enc28j60* self, uint8_t address, uint8_t* value)
{
    if (self->spi_bus.ops->transfer(&self->spi_bus, tx_buffer, rx_buffer, 32))
    {
        return -EIO;
    }
    return 0;
}

/* ========================================================================== */
