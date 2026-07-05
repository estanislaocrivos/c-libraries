#ifndef HD44780_H
#define HD44780_H

/* ========================================================================== */

#include "../../embedded-hal/inc/timer.h"

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

struct hd44780;

struct hd44780_ops
{
    void (*write_nibble)(const struct hd44780* self, uint8_t nibble);
    void (*write_rs)(const struct hd44780* self, bool state);
    void (*write_en)(const struct hd44780* self, bool state);
    void (*write_rw)(const struct hd44780* self, bool state);
};

struct hd44780
{
    bool                            eight_bit_mode;
    bool                            was_initialized;
    struct timer*                   tmr;
    void*                           ops_ctx;
    const struct hd44780_ops* const ops;
};

/* ========================================================================== */

int8_t lcd_initialize(struct hd44780* self);

/* ========================================================================== */

int8_t lcd_go_to_index(const struct hd44780* self, uint8_t x, uint8_t y);

/* ========================================================================== */

int8_t lcd_clear_display(const struct hd44780* self);

/* ========================================================================== */

int8_t lcd_print_string(const struct hd44780* self, char* string);

/* ========================================================================== */

#endif  // HD44780_H
