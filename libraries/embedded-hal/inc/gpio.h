#ifndef GPIO_H
#define GPIO_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

/**
 * @brief Macro to define a GPIO instance.
 * @param name Variable name for the GPIO instance.
 * @param ops_ptr Pointer to the GPIO operations (e.g., PLATFORM_GPIO_OPS).
 * @param pin_id Pin identifier (platform-specific, defined by driver).
 * @param type GPIO type (GPIO_DIGITAL or GPIO_ANALOG).
 * @param dir Initial direction (GPIO_INPUT or GPIO_OUTPUT).
 */
#define GPIO_DEFINE(name, ops_ptr, pin_id, pin_type, pin_dir) \
    struct gpio name                                          \
        = {.ops             = (ops_ptr),                      \
           .id              = (pin_id),                       \
           .type            = (pin_type),                     \
           .direction       = (pin_dir),                      \
           .was_initialized = false}

/* ========================================================================== */

/**
 * @brief GPIO direction configuration.
 */
enum gpio_direction
{
    GPIO_INPUT  = 0,
    GPIO_OUTPUT = 1,
};

/**
 * @brief GPIO type configuration.
 */
enum gpio_type
{
    GPIO_DIGITAL = 0,
    GPIO_ANALOG  = 1,
};

/* ========================================================================== */

/**
 * @brief GPIO interface structure.
 */
struct gpio;

/* ========================================================================== */

/**
 * @brief GPIO operations structure.
 */
struct gpio_ops
{
    /**
     * @brief Initializes the GPIO pin.
     * @param self Pointer to the GPIO structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct gpio* self);

    /**
     * @brief Sets the GPIO pin direction.
     * @param self Pointer to the GPIO structure.
     * @param direction Desired direction (GPIO_INPUT or GPIO_OUTPUT).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_direction)(struct gpio* self, enum gpio_direction direction);

    /**
     * @brief Sets the GPIO pin to the specified state (high or low).
     * @param self Pointer to the GPIO structure.
     * @param state Desired state (true for high, false for low).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_state)(const struct gpio* self, bool state);

    /**
     * @brief Gets the current GPIO pin state.
     * @param self Pointer to the GPIO structure.
     * @param state Pointer to store the state (true for high, false for low).
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

/* ========================================================================== */

struct gpio
{
    /**
     * @brief GPIO pin identifier (platform-specific, defined by driver).
     */
    uint8_t id;

    /**
     * @brief GPIO pin type.
     */
    enum gpio_type type;

    /**
     * @brief GPIO pin direction.
     */
    enum gpio_direction direction;

    /**
     * @brief Pointer to the GPIO operations structure.
     */
    const struct gpio_ops* const ops;

    /* private: internal state - do not access directly */
    bool was_initialized;
};

/* ========================================================================== */

#endif /* GPIO_H */
