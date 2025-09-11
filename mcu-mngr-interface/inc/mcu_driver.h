#ifndef MCU_DRIVER_H
#define MCU_DRIVER_H

/* ============================================================================================== */

/* Mock driver functions for unit testing the mcu manager interface */

/* ============================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "errno.h"

/* ============================================================================================== */

void led_toggle(void);

void led_turn_on(void);

void led_turn_off(void);

/* ============================================================================================== */

void disable_interrupts(void);

void enable_interrupts(void);

/* ============================================================================================== */

typedef void (*mcu_tmr_timeout_callback_t)(void);

int8_t tmr0_set_timeout_callback(mcu_tmr_timeout_callback_t callback);

void tmr0_deactivate_timeout(void);

void tmr0_reset_timeout(void);

int8_t tmr0_set_timeout_ms(uint16_t timeout_ms);

/* ============================================================================================== */

void gpio0_set_high(void);

void gpio0_set_low(void);

void gpio0_set_state(bool state);

void gpio0_get_state(bool* state);

/* ============================================================================================== */

void gpio1_set_high(void);

void gpio1_set_low(void);

void gpio1_set_state(bool state);

void gpio1_get_state(bool* state);

/* ============================================================================================== */

void gpio2_set_high(void);

void gpio2_set_low(void);

void gpio2_set_state(bool state);

void gpio2_get_state(bool* state);

/* ============================================================================================== */

void gpio3_set_high(void);

void gpio3_set_low(void);

void gpio3_set_state(bool state);

void gpio3_get_state(bool* state);

/* ============================================================================================== */

void gpio4_set_high(void);

void gpio4_set_low(void);

void gpio4_set_state(bool state);

void gpio4_get_state(bool* state);

/* ============================================================================================== */

void gpio5_set_high(void);

void gpio5_set_low(void);

void gpio5_set_state(bool state);

void gpio5_get_state(bool* state);

/* ============================================================================================== */

void gpio6_set_high(void);

void gpio6_set_low(void);

void gpio6_set_state(bool state);

void gpio6_get_state(bool* state);

/* ============================================================================================== */

void gpio7_set_high(void);

void gpio7_set_low(void);

void gpio7_set_state(bool state);

void gpio7_get_state(bool* state);

/* ============================================================================================== */

void gpio8_set_high(void);

void gpio8_set_low(void);

void gpio8_set_state(bool state);

void gpio8_get_state(bool* state);

/* ============================================================================================== */

void gpio9_set_high(void);

void gpio9_set_low(void);

void gpio9_set_state(bool state);

void gpio9_get_state(bool* state);

/* ============================================================================================== */

int8_t delay_ms(uint16_t ms);

int8_t delay_us(uint16_t us);

/* ============================================================================================== */

#endif
