#ifndef ENC28J60_H
#define ENC28J60_H

#include "../../embedded-hal/inc/gpio.h"
#include "../../embedded-hal/inc/spi.h"

#include <stdint.h>

/* ========================================================================== */

struct enc28j60_private
{
    uint16_t erdpt; /* Next RX buffer packet pointer */
    uint8_t  current_mem_bank;
    bool     initialized;
};

struct enc28j60
{
    const struct gpio* const spi_cs;
    const struct spi* const  spi_bus;
    const uint8_t            mac_address[6];
    const uint16_t           rx_buf_end_addr;
    struct enc28j60_private private;
};

/* ========================================================================== */

int8_t enc28j60_init(struct enc28j60* self);

int8_t enc28j60_reset(const struct enc28j60* self);

int8_t enc28j60_get_epktcnt(const struct enc28j60* self, uint8_t* epktcnt);

int8_t enc28j60_receive_packet(
    struct enc28j60* self, uint8_t* buffer, uint16_t* size);

int8_t enc28j60_transmit_packet(
    struct enc28j60* self, uint8_t* frame, uint16_t size);

/* ========================================================================== */

#endif /* ENC28J60_H */
