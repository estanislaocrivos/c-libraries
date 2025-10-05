#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "config.h"
#include "../../c-libraries/inc/errno.h"
#include "../../c-libraries/inc/c_libraries.h"

/* ============================================================================================== */

enum gpio_id
{
    GPIO_A0 = 0,
    GPIO_A1,
    GPIO_A2,
    GPIO_A3,
    GPIO_A4,
    GPIO_A5,
    GPIO_A6,
    GPIO_A7,
    GPIO_B0,
    GPIO_B1,
    GPIO_B2,
    GPIO_B3,
    GPIO_B4,
    GPIO_B5,
    GPIO_B6,
    GPIO_B7,
    GPIO_C0,
    GPIO_C1,
    GPIO_C2,
    GPIO_C3,
    GPIO_C4,
    GPIO_C5,
    GPIO_C6,
    GPIO_C7,
    GPIO_D0,
    GPIO_D1,
    GPIO_D2,
    GPIO_D3,
    GPIO_D4,
    GPIO_D5,
    GPIO_D6,
    GPIO_D7,
    GPIO_E0,
    GPIO_E1,
    GPIO_E2,
    GPIO_E3,
    GPIO_E4,
    GPIO_E5,
    GPIO_E6,
    GPIO_E7,
    GPIO_F0,
    GPIO_F1,
    GPIO_F2,
    GPIO_F3,
    GPIO_F4,
    GPIO_F5,
    GPIO_F6,
    GPIO_F7,
    GPIO_G0,
    GPIO_G1,
    GPIO_G2,
    GPIO_G3,
    GPIO_G4,
    GPIO_G5,
    GPIO_G6,
    GPIO_G7,
    GPIO_H0,
    GPIO_H1,
    GPIO_H2,
    GPIO_H3,
    GPIO_H4,
    GPIO_H5,
    GPIO_H6,
    GPIO_H7,
    GPIO_J0,
    GPIO_J1,
    GPIO_J2,
    GPIO_J3,
    GPIO_J4,
    GPIO_J5,
    GPIO_J6,
    GPIO_J7,
    GPIO_K0,
    GPIO_K1,
    GPIO_K2,
    GPIO_K3,
    GPIO_K4,
    GPIO_K5,
    GPIO_K6,
    GPIO_K7,
    GPIO_L0,
    GPIO_L1,
    GPIO_L2,
    GPIO_L3,
    GPIO_L4,
    GPIO_L5,
    GPIO_L6,
    GPIO_L7,
};

/* ============================================================================================== */

int8_t gpio_set_as_input(struct gpio* self, bool input);

/* ============================================================================================== */

int8_t gpio_set_state(struct gpio* self, bool state);

/* ============================================================================================== */

int8_t gpio_get_state(struct gpio* self, bool* state);

/* ============================================================================================== */

int8_t gpio_toggle(struct gpio* self);

/* ============================================================================================== */

#endif  // GPIO_DRIVER_H
