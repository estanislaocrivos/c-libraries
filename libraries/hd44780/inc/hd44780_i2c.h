#ifndef HD44780_I2C_H
#define HD44780_I2C_H

/* ========================================================================== */

#include "../../embedded-hal/inc/gpio.h"
#include "../../embedded-hal/inc/i2c.h"
#include "hd44780.h"

/* ========================================================================== */

struct hd44780_pcf8574_ctx
{
    struct i2c*  i2c_bus;
    struct gpio* enable;
};

/* ========================================================================== */

/**
 * @brief Write a nibble to an HD44780 via PCF8574 I2C adapter. Composes the
 * full PCF8574 byte (D4-D7, RS, RW=0, EN, backlight), pulses EN high then low,
 * and transmits via I2C. Intended to be assigned to hd44780_ops.write_nibble.
 * @param self Pointer to the hd44780 instance. ops_ctx must point to a valid
 * struct hd44780_pcf8574_ctx.
 * @param nibble Lower 4 bits are written to D4-D7.
 * @param command true for command mode (RS=0), false for data mode (RS=1).
 */
void hd44780_pcf8574_write_nibble(
    const struct hd44780* self, uint8_t nibble, bool command);

/* ========================================================================== */

#endif /* HD44780_I2C_H */
