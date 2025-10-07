#ifndef LCD_H
#define LCD_H

/* ============================================================================================== */

#include "../../embedded-hal/inc/gpio.h"
#include "../../embedded-hal/inc/timer.h"
#include "../../../inc/errno.h"

/* ============================================================================================== */

struct lcd_bus_interface
{
    struct gpio rs;
    struct gpio en;
    struct gpio d0;
    struct gpio d1;
    struct gpio d2;
    struct gpio d3;
    struct gpio d4;
    struct gpio d5;
    struct gpio d6;
    struct gpio d7;
};

struct lcd
{
    bool                      eight_bit_mode;
    bool                      _initialized;
    struct lcd_bus_interface* _bus;
    struct timer*             _delay;
};

/* ============================================================================================== */

int8_t lcd_create(struct lcd* self);

/* ============================================================================================== */

int8_t lcd_initialize(struct lcd* self);

/* ============================================================================================== */

int8_t lcd_go_to_index(const struct lcd* self, uint8_t x, uint8_t y);

/* ============================================================================================== */

int8_t lcd_clear_display(const struct lcd* self);

/* ============================================================================================== */

int8_t lcd_print_string(const struct lcd* self, char* string);

/* ============================================================================================== */

#endif  // LCD_H
