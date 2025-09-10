#include "../inc/lcd.h"

/* ============================================================================================== */

#include <stdio.h>
#include <string.h>

/* ============================================================================================== */

/*
GPIO-0 => PB7 => RS
GPIO-1 => PB6 => EN
GPIO-2 => PB5 => D4
GPIO-3 => PB4 => D5
GPIO-4 => PH6 => D6
GPIO-5 => PH5 => D7
*/

/* ============================================================================================== */

static void _lcd_command_assert_seq(lcd_t* self)
{
    self->rs->set_low();
    self->en->set_high();
    self->delay->set_ms(10);
    self->en->set_low();
}

/* ============================================================================================== */

static void _lcd_data_assert_seq(lcd_t* self)
{
    self->rs->set_high();
    self->en->set_high();
    self->delay->set_ms(10);
    self->en->set_low();
}

/* ============================================================================================== */

static void _lcd_send_nibble(lcd_t* self, uint8_t nibble)
{
    self->d4->set_state((nibble >> 0) & 0x01);
    self->d5->set_state((nibble >> 1) & 0x01);
    self->d6->set_state((nibble >> 2) & 0x01);
    self->d7->set_state((nibble >> 3) & 0x01);
}

/* ============================================================================================== */

static void _lcd_send_command(lcd_t* self, uint8_t command)
{
    _lcd_send_nibble(self, (command >> 4) & 0x0F);
    _lcd_command_assert_seq(self);
    _lcd_send_nibble(self, command & 0x0F);
    _lcd_command_assert_seq(self);
}

/* ============================================================================================== */

static void _lcd_send_char(lcd_t* self, char character)
{
    _lcd_send_nibble(self, (character >> 4) & 0x0F);
    _lcd_data_assert_seq(self);
    _lcd_send_nibble(self, character & 0x0F);
    _lcd_data_assert_seq(self);
}

/* ============================================================================================== */

int8_t lcd_create(lcd_t* self, mcu_manager_interface_t* mcu_mngr, bool eight_bit_mode)
{
    if (self == NULL || mcu_mngr == NULL)
    {
        return -EFAULT;
    }
    memset(self, 0, sizeof(lcd_t));
    self->_mcu_mngr = mcu_mngr;
    self->rs        = mcu_mngr->gpio_0;
    self->en        = mcu_mngr->gpio_1;
    self->d4        = mcu_mngr->gpio_2;
    self->d5        = mcu_mngr->gpio_3;
    self->d6        = mcu_mngr->gpio_4;
    self->d7        = mcu_mngr->gpio_5;
    self->delay     = mcu_mngr->delay;
    if (eight_bit_mode)
    {
        self->d0 = mcu_mngr->gpio_6;
        self->d1 = mcu_mngr->gpio_7;
        self->d2 = mcu_mngr->gpio_8;
        self->d3 = mcu_mngr->gpio_9;
    }
    return 0;
}

/* ============================================================================================== */

int8_t lcd_initialize(lcd_t* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    self->rs->set_low();
    self->en->set_low();

    /* Init. routine. First send only nibbles (4-bit mode not active yet) */
    _lcd_send_nibble(self, 0x03);
    _lcd_command_assert_seq(self);
    self->delay->set_ms(5);
    _lcd_send_nibble(self, 0x03);
    _lcd_command_assert_seq(self);
    self->delay->set_us(150);
    _lcd_send_nibble(self, 0x03);
    _lcd_command_assert_seq(self);
    self->delay->set_us(150);
    _lcd_send_nibble(self, 0x02);
    _lcd_command_assert_seq(self);
    self->delay->set_us(150);

    /* Now send 8-bit commands */
    _lcd_send_command(self, 0x28); /* Function set */
    self->delay->set_ms(2);
    _lcd_send_command(self, 0x08); /* Display off */
    self->delay->set_ms(2);
    _lcd_send_command(self, 0x01); /* Clear display */
    self->delay->set_ms(2);
    _lcd_send_command(self, 0x06); /* Entry mode */
    self->delay->set_ms(2);
    _lcd_send_command(self, 0x0C); /* Display on */
    self->delay->set_ms(2);
    _lcd_send_command(self, 0x01); /* Clear display */
    self->delay->set_ms(2);
    return 0;
}

/* ============================================================================================== */

int8_t lcd_go_to_index(lcd_t* self, uint8_t x, uint8_t y)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    const uint8_t start[] = {0x80 | 0x00, 0x80 | 0xC0};
    _lcd_send_command(self, start[y] + x);
    return 0;
}

/* ============================================================================================== */

int8_t lcd_clear_display(lcd_t* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    _lcd_send_command(self, 0x01);
    return 0;
}

/* ============================================================================================== */

int8_t lcd_print_string(lcd_t* self, char* string)
{
    if (self == NULL || string == NULL)
    {
        return -EFAULT;
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
            self->delay->set_ms(500);
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
