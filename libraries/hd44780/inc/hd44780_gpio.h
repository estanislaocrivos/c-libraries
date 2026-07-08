#ifndef HD44780_GPIO_H
#define HD44780_GPIO_H

/* ========================================================================== */

#include "../../embedded-hal/inc/gpio.h"
#include "hd44780.h"

/* ========================================================================== */

struct hd44780_gpio_ctx
{
    struct gpio* en;
    struct gpio* rs;
    struct gpio* d_4;
    struct gpio* d_5;
    struct gpio* d_6;
    struct gpio* d_7;
};

/* ========================================================================== */

/**
 * @brief Write a nibble to an HD44780 via parallel GPIO bus.
 * @param self Pointer to the hd44780 instance. ops_ctx must point to a valid
 * struct hd44780_gpio_ctx.
 * @param nibble Lower 4 bits are written to D4-D7.
 * @param command true for command mode (RS=0), false for data mode (RS=1).
 */
void hd44780_gpio_write_nibble(
    const struct hd44780* self, uint8_t nibble, bool command);

/* ========================================================================== */

#endif /* HD44780_GPIO_H */
