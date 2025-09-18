#ifndef LCD_H
#define LCD_H

/* ============================================================================================== */

#include "../../mcu-mngr-interface/inc/mcu_manager.h"
#include "../../inc/errno.h"

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

typedef struct
{
    bool                      eight_bit_mode;
    bool                      _initialized;
    struct lcd_bus_interface* _bus;
    struct timer*             _delay;
} lcd_t;

/* ============================================================================================== */

int8_t lcd_create(lcd_t* self);

/* ============================================================================================== */

int8_t lcd_initialize(lcd_t* self);

/* ============================================================================================== */

int8_t lcd_go_to_index(const lcd_t* self, uint8_t x, uint8_t y);

/* ============================================================================================== */

int8_t lcd_clear_display(const lcd_t* self);

/* ============================================================================================== */

int8_t lcd_print_string(const lcd_t* self, char* string);

/* ============================================================================================== */

#endif  // LCD_H
