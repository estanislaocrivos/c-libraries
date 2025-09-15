#ifndef MCU_MANAGER_H
#define MCU_MANAGER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================================== */

typedef int8_t (*delay_ms_t)(uint16_t);
typedef int8_t (*delay_us_t)(uint16_t);
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
typedef void (*gpio_set_state_t)(bool);
typedef void (*gpio_get_state_t)(bool*);
typedef void (*gpio_toggle_t)(void);
typedef void (*disable_interrupts_t)(void);
typedef void (*enable_interrupts_t)(void);

/* ============================================================================================== */

typedef struct
{
    disable_interrupts_t disable;
    enable_interrupts_t  enable;
} interrupt_ctrl_interface_t;

typedef struct
{
    tmr_set_timeout_t          set_timeout;
    tmr_reset_timeout_t        reset_timeout;
    tmr_deact_timeout_t        deactivate_timeout;
    tmr_set_timeout_callback_t set_callback;
} timer_interface_t;

typedef struct
{
    delay_ms_t set_ms;
    delay_us_t set_us;
} delay_interface_t;

typedef struct
{
    led_on_t     on;
    led_off_t    off;
    led_toggle_t toggle;
} led_interface_t;

typedef struct
{
    gpio_set_state_t set_state;
    gpio_get_state_t get_state;
} gpio_interface_t;

/* ============================================================================================== */

typedef struct
{
    interrupt_ctrl_interface_t* interrupt_ctrl;
    timer_interface_t*          timer0;
    timer_interface_t*          timer1;
    timer_interface_t*          timer2;
    timer_interface_t*          timer3;
    delay_interface_t*          delay;
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
