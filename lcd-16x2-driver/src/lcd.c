#include "../inc/lcd.h"

/* ============================================================================================== */

#include <stdio.h>
#include <string.h>

/* ============================================================================================== */

static void _lcd_command_assert_seq(const lcd_t* self)
{
    self->_bus->rs.ops->set_state(&self->_bus->rs, false);
    self->_bus->en.ops->set_state(&self->_bus->en, true);
    self->_delay->ops->delay_ms(self->_delay, 10);
    self->_bus->en.ops->set_state(&self->_bus->en, false);
}

/* ============================================================================================== */

static void _lcd_data_assert_seq(const lcd_t* self)
{
    self->_bus->rs.ops->set_state(&self->_bus->rs, true);
    self->_bus->en.ops->set_state(&self->_bus->en, true);
    self->_delay->ops->delay_ms(self->_delay, 10);
    self->_bus->en.ops->set_state(&self->_bus->en, false);
}

/* ============================================================================================== */

static void _lcd_send_nibble(const lcd_t* self, uint8_t nibble)
{
    self->_bus->d4.ops->set_state(&self->_bus->d4, (nibble >> 0) & 0x01);
    self->_bus->d5.ops->set_state(&self->_bus->d5, (nibble >> 1) & 0x01);
    self->_bus->d6.ops->set_state(&self->_bus->d6, (nibble >> 2) & 0x01);
    self->_bus->d7.ops->set_state(&self->_bus->d7, (nibble >> 3) & 0x01);
}

/* ============================================================================================== */

static void _lcd_send_command(const lcd_t* self, uint8_t command)
{
    _lcd_send_nibble(self, (command >> 4) & 0x0F);
    _lcd_command_assert_seq(self);
    _lcd_send_nibble(self, command & 0x0F);
    _lcd_command_assert_seq(self);
}

/* ============================================================================================== */

static void _lcd_send_char(const lcd_t* self, char character)
{
    _lcd_send_nibble(self, (character >> 4) & 0x0F);
    _lcd_data_assert_seq(self);
    _lcd_send_nibble(self, character & 0x0F);
    _lcd_data_assert_seq(self);
}

/* ============================================================================================== */

int8_t lcd_create(lcd_t* self)
{
    if (self == NULL || self->_bus == NULL || self->_delay == NULL)
    {
        return -EFAULT;
    }
    self->_initialized = true;
    return 0;
}

/* ============================================================================================== */

int8_t lcd_initialize(lcd_t* self)
{
    if (self == NULL || self == NULL)
    {
        return -EFAULT;
    }
    self->_initialized = true;
    self->_bus->rs.ops->set_state(&self->_bus->rs, false);
    self->_bus->en.ops->set_state(&self->_bus->en, false);

    /* Init. routine. First send only nibbles (4-bit mode not active yet) */
    _lcd_send_nibble(self, 0x03);
    _lcd_command_assert_seq(self);
    self->_delay->ops->delay_ms(self->_delay, 5);
    _lcd_send_nibble(self, 0x03);
    _lcd_command_assert_seq(self);
    self->_delay->ops->delay_us(self->_delay, 150);
    _lcd_send_nibble(self, 0x03);
    _lcd_command_assert_seq(self);
    self->_delay->ops->delay_us(self->_delay, 150);
    _lcd_send_nibble(self, 0x02);
    _lcd_command_assert_seq(self);
    self->_delay->ops->delay_us(self->_delay, 150);

    /* Now send 8-bit commands */
    _lcd_send_command(self, 0x28); /* Function set */
    self->_delay->ops->delay_ms(self->_delay, 2);
    _lcd_send_command(self, 0x08); /* Display off */
    self->_delay->ops->delay_ms(self->_delay, 2);
    _lcd_send_command(self, 0x01); /* Clear display */
    self->_delay->ops->delay_ms(self->_delay, 2);
    _lcd_send_command(self, 0x06); /* Entry mode */
    self->_delay->ops->delay_ms(self->_delay, 2);
    _lcd_send_command(self, 0x0C); /* Display on */
    self->_delay->ops->delay_ms(self->_delay, 2);
    _lcd_send_command(self, 0x01); /* Clear display */
    self->_delay->ops->delay_ms(self->_delay, 2);
    return 0;
}

/* ============================================================================================== */

int8_t lcd_go_to_index(const lcd_t* self, uint8_t x, uint8_t y)
{
    if (self == NULL || self == NULL)
    {
        return -EFAULT;
    }
    if (x > 15 || y > 1)
    {
        return -EINVAL;
    }
    if (!self->_initialized)
    {
        return -ENOENT;
    }
    const uint8_t start[] = {0x80, 0x80 | 0xC0};
    _lcd_send_command(self, start[y] + x);
    return 0;
}

/* ============================================================================================== */

int8_t lcd_clear_display(const lcd_t* self)
{
    if (self == NULL || self == NULL)
    {
        return -EFAULT;
    }
    if (!self->_initialized)
    {
        return -ENOENT;
    }
    _lcd_send_command(self, 0x01);
    return 0;
}

/* ============================================================================================== */

int8_t lcd_print_string(const lcd_t* self, char* string)
{
    if (self == NULL || self == NULL || string == NULL)
    {
        return -EFAULT;
    }
    if (!self->_initialized)
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
            self->_delay->ops->delay_ms(self->_delay, 500);
            lcd_clear_display(self);
            lcd_go_to_index(self, 0, 0);
            char_index = 0;
        }
        _lcd_send_char(self, *string);
        string += 1;
        char_index += 1;
    }
    return 0;
}

/* ============================================================================================== */
