#ifndef MCU_MANAGER_H
#define MCU_MANAGER_H

/* ============================================================================================== */

#include "gpio.h"
#include "timer.h"
#include "interrupt.h"

/* ============================================================================================== */

struct mcu_manager
{
    const struct interrupt* interrupts;
    const struct timer*     timers[4];
    const struct gpio*      leds[5];
    const struct gpio*      inputs[10];
    const struct gpio*      outputs[10];
};

/* ============================================================================================== */

#endif
