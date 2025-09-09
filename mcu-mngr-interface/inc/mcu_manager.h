#ifndef MCU_MANAGER_H
#define MCU_MANAGER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "errno.h"

/* ============================================================================================== */

typedef void (*tmr_timeout_callback_t)(void);
typedef int8_t (*tmr_set_timeout_t)(uint16_t);
typedef void (*tmr_deact_timeout_t)(void);
typedef void (*tmr_reset_timeout_t)(void);
typedef int8_t (*tmr_set_timeout_callback_t)(tmr_timeout_callback_t);
typedef void (*led_on_t)(void);
typedef void (*led_off_t)(void);
typedef void (*led_toggle_t)(void);
typedef void (*gpio_high_t)(void);
typedef void (*gpio_low_t)(void);
typedef void (*gpio_set_state_t)(uint8_t, bool);
typedef void (*gpio_toggle_t)(void);
typedef void (*disable_interrupts_t)(void);
typedef void (*enable_interrupts_t)(void);

/* ============================================================================================== */

typedef struct
{
    disable_interrupts_t disable_interrupts;
    enable_interrupts_t  enable_interrupts;
} interrupt_ctrl_interface_t;

typedef struct
{
    tmr_set_timeout_t          set_timeout;
    tmr_reset_timeout_t        reset_timeout;
    tmr_set_timeout_callback_t set_callback;
} timer_interface_t;

typedef struct
{
    led_on_t     led_on;
    led_off_t    led_off;
    led_toggle_t led_toggle;
} led_interface_t;

typedef struct
{
    gpio_high_t      gpio_set_high;
    gpio_low_t       gpio_set_low;
    gpio_set_state_t gpio_set_state;
    gpio_toggle_t    gpio_toggle;
} gpio_interface_t;

/* ============================================================================================== */

typedef struct
{
    interrupt_ctrl_interface_t* interrupt_ctrl;
    timer_interface_t*          timer0;
    timer_interface_t*          timer1;
    timer_interface_t*          timer2;
    timer_interface_t*          timer3;
    led_interface_t*            led_0;
    led_interface_t*            led_1;
    led_interface_t*            led_2;
    gpio_interface_t*           gpio_0;
    gpio_interface_t*           gpio_1;
    gpio_interface_t*           gpio_2;
    gpio_interface_t*           gpio_3;
    gpio_interface_t*           gpio_4;
    gpio_interface_t*           gpio_5;
    gpio_interface_t*           gpio_6;
    gpio_interface_t*           gpio_7;
    gpio_interface_t*           gpio_8;
    gpio_interface_t*           gpio_9;
    gpio_interface_t*           gpio_10;
    gpio_interface_t*           gpio_11;
    gpio_interface_t*           gpio_12;
} mcu_manager_interface_t;

/* ============================================================================================== */

#endif
