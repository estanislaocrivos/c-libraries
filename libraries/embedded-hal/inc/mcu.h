/**
 * @file mcu.h
 * @brief MCU system configuration interface.
 *
 * This interface provides platform-agnostic MCU initialization and control,
 * including oscillator configuration, clock setup, and power management.
 */

/* ========================================================================== */

#ifndef MCU_H
#define MCU_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

#define MCU_DEFINE(name, ops_ptr) \
    struct mcu name = {.ops = (ops_ptr), .was_initialized = false}

/* ========================================================================== */

/**
 * @brief MCU interface structure.
 */
struct mcu;

/* ========================================================================== */

/**
 * @brief MCU operations structure.
 */
struct mcu_ops
{
    /**
     * @brief Initializes the MCU (oscillator, clocks, essential peripherals).
     * @param self Pointer to the MCU structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct mcu* self);

    /**
     * @brief Performs a software reset of the MCU.
     * @param self Pointer to the MCU structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     * @note This function may not return if reset is successful.
     */
    int8_t (*reset)(struct mcu* self);

    /**
     * @brief Puts the MCU into sleep/low-power mode.
     * @param self Pointer to the MCU structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*sleep)(struct mcu* self);

    /**
     * @brief Gets the current system clock frequency in Hz.
     * @param self Pointer to the MCU structure.
     * @param frequency Pointer to store the clock frequency.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*get_clock_frequency)(const struct mcu* self, uint32_t* frequency);
};

/* ========================================================================== */

struct mcu
{
    /**
     * @brief Pointer to the MCU operations structure.
     */
    const struct mcu_ops* const ops;

    /* private: internal state - do not access directly */
    bool     was_initialized;
    uint32_t clock_frequency;
};

/* ========================================================================== */

#endif /* MCU_H */
