/**
 * @file
 * @brief PIC18-K42 GPIO driver header file.
 */

/* ========================================================================== */

#ifndef PIC18_GPIO_H
#define PIC18_GPIO_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "errno.h"
#include "gpio.h"
#include "pic18_k42.h"

/* ========================================================================== */

extern const struct gpio_ops pic18_gpio_ops;

/* ========================================================================== */

/**
 * @brief GPIO IDs mapping assumed for this driver. When using the GPIO
 * interface, the ID used for each GPIO will correspond to the following
 * mapping.
 */
#define GPIO_A0_ID 0
#define GPIO_A1_ID 1
#define GPIO_A2_ID 2
#define GPIO_A3_ID 3
#define GPIO_A4_ID 4
#define GPIO_A5_ID 5
#define GPIO_A6_ID 6
#define GPIO_A7_ID 7
#define GPIO_B0_ID 8
#define GPIO_B1_ID 9
#define GPIO_B2_ID 10
#define GPIO_B3_ID 11
#define GPIO_B4_ID 12
#define GPIO_B5_ID 13
#define GPIO_B6_ID 14
#define GPIO_B7_ID 15
#define GPIO_C0_ID 16
#define GPIO_C1_ID 17
#define GPIO_C2_ID 18
#define GPIO_C3_ID 19
#define GPIO_C4_ID 20
#define GPIO_C5_ID 21
#define GPIO_C6_ID 22
#define GPIO_C7_ID 23
#if defined(PIC18F47K42) || defined(PIC18F57K42)
#define GPIO_D0_ID 24
#define GPIO_D1_ID 25
#define GPIO_D2_ID 26
#define GPIO_D3_ID 27
#define GPIO_D4_ID 28
#define GPIO_D5_ID 29
#define GPIO_D6_ID 30
#define GPIO_D7_ID 31
#define GPIO_E0_ID 32
#define GPIO_E1_ID 33
#define GPIO_E2_ID 34
#endif
#if defined(PIC18F57K42)
#define GPIO_F0_ID 40
#define GPIO_F1_ID 41
#define GPIO_F2_ID 42
#define GPIO_F3_ID 43
#define GPIO_F4_ID 44
#define GPIO_F5_ID 45
#define GPIO_F6_ID 46
#define GPIO_F7_ID 47
#endif

/* ========================================================================== */

/* Port indices */
#define PORT_A       0
#define PORT_B       1
#define PORT_C       2
#define PORT_D       3
#define PORT_E       4
#define PORT_F       5
#define PORT_INVALID 0xFF

/* ========================================================================== */

/**
 * @brief Retrieves the pin number (0-7) from the GPIO ID.
 * @param id GPIO ID.
 * @return uint8_t Pin number (0-7).
 */
inline uint8_t pic18_get_pin_from_id(uint8_t id);

/**
 * @brief Retrieves the port identifier from the GPIO ID.
 * @param id GPIO ID.
 * @return uint8_t Port identifier (e.g., PORT_A, PORT_B, etc.).
 */
inline uint8_t pic18_get_port_from_id(uint8_t id);

/* ========================================================================== */

/**
 * @brief Initializes the GPIO pin.
 * @param self Pointer to the GPIO structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_gpio_initialize(struct gpio* self);

/**
 * @brief Sets the GPIO pin direction.
 * @param self Pointer to the GPIO structure.
 * @param direction Desired direction (GPIO_INPUT or GPIO_OUTPUT).
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_gpio_set_direction(
    struct gpio* self, enum gpio_direction direction);

/**
 * @brief Sets the GPIO pin to the specified state (high or low).
 * @param self Pointer to the GPIO structure.
 * @param state Desired state (true for high, false for low).
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_gpio_set_state(const struct gpio* self, bool state);

/**
 * @brief Gets the current GPIO pin state.
 * @param self Pointer to the GPIO structure.
 * @param state Pointer to store the state (true for high, false for low).
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_gpio_get_state(const struct gpio* self, bool* state);

/**
 * @brief Toggles the GPIO pin state.
 * @param self Pointer to the GPIO structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_gpio_toggle(const struct gpio* self);

/* ========================================================================== */

#endif /* PIC18_GPIO_H */
