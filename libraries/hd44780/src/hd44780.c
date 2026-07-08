#include "../inc/hd44780.h"

/* ========================================================================== */

#include "../../inc/errno.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* ========================================================================== */

static void hd44780_send_command(const struct hd44780* self, uint8_t command)
{
    self->ops->write_nibble(self, (command >> 4) & 0x0F, true);
    self->ops->write_nibble(self, (command) & 0x0F, true);
}

/* ========================================================================== */

static void hd44780_send_char(const struct hd44780* self, char character)
{
    self->ops->write_nibble(self, (character >> 4) & 0x0F, false);
    self->ops->write_nibble(self, character & 0x0F, false);
}

/* ========================================================================== */

int8_t lcd_initialize(struct hd44780* self)
{
    if (self == NULL || self->ops->write_nibble == NULL
        || self->tmr->ops->delay_ms == NULL || self->tmr->ops->delay_us == NULL)
    {
        return -EFAULT;
    }

    /* Init. routine. First send only nibbles (4-bit mode not active yet) */
    self->ops->write_nibble(self, 0x03, true);
    self->tmr->ops->delay_ms(self->tmr, 5);
    self->ops->write_nibble(self, 0x03, true);
    self->tmr->ops->delay_us(self->tmr, 150);
    self->ops->write_nibble(self, 0x03, true);
    self->tmr->ops->delay_us(self->tmr, 150);
    self->ops->write_nibble(self, 0x02, true);
    self->tmr->ops->delay_us(self->tmr, 150);

    /* Now send 8-bit commands */
    hd44780_send_command(self, 0x28); /* Function set */
    self->tmr->ops->delay_ms(self->tmr, 2);
    hd44780_send_command(self, 0x08); /* Display off */
    self->tmr->ops->delay_ms(self->tmr, 2);
    hd44780_send_command(self, 0x01); /* Clear display */
    self->tmr->ops->delay_ms(self->tmr, 2);
    hd44780_send_command(self, 0x06); /* Entry mode */
    self->tmr->ops->delay_ms(self->tmr, 2);
    hd44780_send_command(self, 0x0C); /* Display on */
    self->tmr->ops->delay_ms(self->tmr, 2);
    hd44780_send_command(self, 0x01); /* Clear display */
    self->tmr->ops->delay_ms(self->tmr, 2);

    self->private.initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t lcd_go_to_index(const struct hd44780* self, uint8_t x, uint8_t y)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    if (x > 15 || y > 1)
    {
        return -EINVAL;
    }
    if (!self->private.initialized)
    {
        return -ENOENT;
    }

    const uint8_t start[] = {0x80, 0x80 | 0xC0};
    hd44780_send_command(self, start[y] + x);
    return 0;
}

/* ========================================================================== */

int8_t lcd_clear_display(const struct hd44780* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->private.initialized)
    {
        return -ENOENT;
    }

    hd44780_send_command(self, 0x01);
    self->tmr->ops->delay_ms(self->tmr, 2);
    return 0;
}

/* ========================================================================== */

int8_t lcd_print_string(const struct hd44780* self, char* string)
{
    if (self == NULL || string == NULL)
    {
        return -EFAULT;
    }
    if (!self->private.initialized)
    {
        return -ENOENT;
    }

    uint8_t char_index = 0;
    lcd_clear_display(self);
    while (*string != '\0')
    {
        if (char_index == 16)
        {
            lcd_go_to_index(self, 0, 1);
        }
        else if (char_index == 32)
        {
            self->tmr->ops->delay_ms(self->tmr, 500);
            lcd_clear_display(self);
            lcd_go_to_index(self, 0, 0);
            char_index = 0;
        }
        hd44780_send_char(self, *string);
        string += 1;
        char_index += 1;
    }
    return 0;
}

/* ========================================================================== */
