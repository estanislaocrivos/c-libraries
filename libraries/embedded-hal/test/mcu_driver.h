#ifndef MCU_DRIVER_H
#define MCU_DRIVER_H

/* ============================================================================================== */

/* Mock driver functions for unit testing the mcu manager interface */

/* ============================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../inc/gpio.h"
#include "../inc/timer.h"
#include "../inc/interrupt.h"
#include "../../inc/errno.h"

/* ============================================================================================== */

void enable_interrupts(bool enable);

/* ============================================================================================== */

int8_t gpio_set_state(struct gpio* self, bool state);

int8_t gpio_get_state(struct gpio* self, bool* state);

int8_t gpio_toggle(struct gpio* self);

/* ============================================================================================== */

typedef void (*mcu_tmr_timeout_callback_t)(void);

int8_t tmr_delay_ms(struct timer* self, uint16_t ms);

int8_t tmr_delay_us(struct timer* self, uint16_t us);

int8_t tmr_set_timeout_ms(struct timer* self, uint16_t ms);

void tmr_reset_timeout(struct timer* self);

void tmr_deactivate_timeout(struct timer* self);

int8_t tmr_set_timeout_callback(struct timer* self, timer_callback_t callback, void* context);

/* ============================================================================================== */

#endif
