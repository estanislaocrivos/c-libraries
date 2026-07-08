#include "../inc/hd44780_i2c.h"

/* ========================================================================== */

#include <stdint.h>

/* ========================================================================== */

#define HD44780_BACKLIGHT_PIN_PCF8574_MASK 0x08
#define HD44780_EN_PIN_PCF8574_MASK        0x04
#define HD44780_RW_PIN_PCF8574_MASK        0x02 /* Low for write (defult) */
#define HD44780_RS_PIN_PCF8574_MASK        0x01

/* ========================================================================== */

void hd44780_pcf8574_write_nibble(
    const struct hd44780* self, const uint8_t nibble, bool command)
{
    struct hd44780_pcf8574_ctx* ctx
        = (struct hd44780_pcf8574_ctx*)(self->ops_ctx);

    uint8_t byte = (uint8_t)(nibble << 4) | HD44780_BACKLIGHT_PIN_PCF8574_MASK;
    if (!command)
    {
        byte |= HD44780_RS_PIN_PCF8574_MASK;
    }

    ctx->i2c_bus->ops->write_raw(ctx->i2c_bus, &byte, 1);

    byte |= HD44780_EN_PIN_PCF8574_MASK;
    ctx->i2c_bus->ops->write_raw(ctx->i2c_bus, &byte, 1);

    self->tmr->ops->delay_us(self->tmr, 1);

    byte &= ~HD44780_EN_PIN_PCF8574_MASK;
    ctx->i2c_bus->ops->write_raw(ctx->i2c_bus, &byte, 1);
}

/* ========================================================================== */
