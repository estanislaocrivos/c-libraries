#include "../inc/hd44780_gpio.h"

/* ========================================================================== */

#include <stdint.h>

/* ========================================================================== */

#define HD44780_D7_NIBBLE_MASK (uint8_t)0x08
#define HD44780_D6_NIBBLE_MASK (uint8_t)0x04
#define HD44780_D5_NIBBLE_MASK (uint8_t)0x02
#define HD44780_D4_NIBBLE_MASK (uint8_t)0x01

/* ========================================================================== */

void hd44780_gpio_write_nibble(
    const struct hd44780* self, const uint8_t nibble, bool command)
{
    struct hd44780_gpio_ctx* ctx = (struct hd44780_gpio_ctx*)(self->ops_ctx);

    ctx->rs->ops->set_state(ctx->rs, !command);

    ctx->d_7->ops->set_state(ctx->d_7, nibble & HD44780_D7_NIBBLE_MASK);
    ctx->d_6->ops->set_state(ctx->d_6, nibble & HD44780_D6_NIBBLE_MASK);
    ctx->d_5->ops->set_state(ctx->d_5, nibble & HD44780_D5_NIBBLE_MASK);
    ctx->d_4->ops->set_state(ctx->d_4, nibble & HD44780_D4_NIBBLE_MASK);

    ctx->en->ops->set_state(ctx->en, true);
    self->tmr->ops->delay_us(self->tmr, 1);
    ctx->en->ops->set_state(ctx->en, false);
}

/* ========================================================================== */
