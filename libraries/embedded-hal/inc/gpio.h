#ifndef GPIO_H
#define GPIO_H

/* ============================================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief GPIO structure. This structure holds all information related to a GPIO pin instance.
 */
struct gpio;

/* ============================================================================================== */

/**
 * @brief GPIO operations structure. This structure holds function pointers for GPIO operations.
 */
struct gpio_ops
{
    /**
     * @brief Sets the GPIO pin to the specified state (high or low).
     * @param self Pointer to the GPIO structure.
     * @param state Desired state (true for high, false for low).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_state)(const struct gpio* self, bool state);

    /**
     * @brief Sets the GPIO pin to high.
     * @param self Pointer to the GPIO structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*get_state)(const struct gpio* self, bool* state);

    /**
     * @brief Toggles the GPIO pin state.
     * @param self Pointer to the GPIO structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*toggle)(const struct gpio* self);
};

/* ============================================================================================== */

struct gpio
{
    /**
     * @brief GPIO pin identifier. Serves to identify different GPIO pins within the same driver.
     */
    uint8_t id;

    /**
     * @brief Pointer to the GPIO operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct gpio_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct gpio_ops* ops;
};

/* ============================================================================================== */

#endif
