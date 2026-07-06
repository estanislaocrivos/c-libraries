#include "../inc/hd44780_i2c.h"

/* ========================================================================== */

#include "../../inc/errno.h"

#include <stdint.h>

/* ========================================================================== */

void hd44780_pcf8574_write_nibble(
    const struct hd44780* self, const uint8_t nibble, bool command)
{
    struct hd44780_pcf8574_ctx* ctx
        = (struct hd44780_pcf8574_ctx*)(self->ops_ctx);

    uint8_t byte = 0;
    if (command)
    {
        byte = nibble | 1;
    }
    else
    {
        byte = nibble & 0;
    }

    ctx->i2c_bus->ops->write_raw(ctx->i2c_bus, &byte, 1);

    ctx->enable->ops->set_state(ctx->enable, true);
    self->tmr->ops->delay_ms(self->tmr, 10);
    ctx->enable->ops->set_state(ctx->enable, false);
}

/* ========================================================================== */
