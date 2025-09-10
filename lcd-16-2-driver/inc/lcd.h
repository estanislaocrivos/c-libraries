#ifndef LCD_H
#define LCD_H

/* ============================================================================================== */

#include "../../mcu-mngr-interface/inc/mcu_manager.h"
#include "errno.h"

/* ============================================================================================== */

typedef struct
{
    mcu_manager_interface_t* _mcu_mngr;
    delay_interface_t*       delay;
    gpio_interface_t*        rs;
    gpio_interface_t*        en;
    gpio_interface_t*        d0;
    gpio_interface_t*        d1;
    gpio_interface_t*        d2;
    gpio_interface_t*        d3;
    gpio_interface_t*        d4;
    gpio_interface_t*        d5;
    gpio_interface_t*        d6;
    gpio_interface_t*        d7;
} lcd_t;

/* ============================================================================================== */

int8_t lcd_create(lcd_t* self, mcu_manager_interface_t* mcu_mngr, bool eight_bit_mode);

/* ============================================================================================== */

int8_t lcd_initialize(lcd_t* self);

/* ============================================================================================== */

int8_t lcd_go_to_index(lcd_t* self, uint8_t x, uint8_t y);

/* ============================================================================================== */

int8_t lcd_clear_display(lcd_t* self);

/* ============================================================================================== */

int8_t lcd_print_string(lcd_t* self, char* string);

/* ============================================================================================== */

#endif
