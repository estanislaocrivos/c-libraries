/**
 * @file
 * @brief PIC18-K42 MCU system configuration driver.
 */

/* ========================================================================== */

#ifndef PIC18_MCU_H
#define PIC18_MCU_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "errno.h"
#include "mcu.h"
#include "pic18_k42.h"

/* ========================================================================== */

extern const struct mcu_ops pic18_mcu_ops;

/* ========================================================================== */

/**
 * @brief Initializes the PIC18 MCU (oscillator, clocks).
 * @param self Pointer to the MCU structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_mcu_initialize(struct mcu* self);

/**
 * @brief Performs a software reset of the PIC18 MCU.
 * @param self Pointer to the MCU structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_mcu_reset(struct mcu* self);

/**
 * @brief Puts the PIC18 MCU into sleep mode.
 * @param self Pointer to the MCU structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_mcu_sleep(struct mcu* self);

/**
 * @brief Gets the current system clock frequency.
 * @param self Pointer to the MCU structure.
 * @param frequency Pointer to store the clock frequency in Hz.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_mcu_get_clock_frequency(
    const struct mcu* self, uint32_t* frequency);

/* ========================================================================== */

#endif /* PIC18_MCU_H */
