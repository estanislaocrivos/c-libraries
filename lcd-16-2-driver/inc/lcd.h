#ifndef LCD_H
#define LCD_H

/* ============================================================================================== */

#include "../../mcu-mngr-interface/inc/mcu_manager.h"
#include "errno.h"

/* ============================================================================================== */

typedef struct
{
    gpio_interface_t rs;
    gpio_interface_t en;
    gpio_interface_t d0;
    gpio_interface_t d1;
    gpio_interface_t d2;
    gpio_interface_t d3;
    gpio_interface_t d4;
    gpio_interface_t d5;
    gpio_interface_t d6;
    gpio_interface_t d7;
} lcd_bus_interface_t;

typedef struct
{
    lcd_bus_interface_t* bus;
    delay_interface_t*   delay;
} lcd_interface_t;

typedef struct
{
    bool             initialized;
    bool             eight_bit_mode;
    lcd_interface_t* iface;
} lcd_t;

/* ============================================================================================== */

int8_t lcd_create(lcd_t* self, lcd_interface_t* iface, bool eight_bit_mode);

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
