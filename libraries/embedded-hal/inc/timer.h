#ifndef TIMER_H
#define TIMER_H

/* ============================================================================================== */

#include <stdint.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Timer structure. This structure holds all information related to a timer instance.
 */
struct timer;

/* ============================================================================================== */

/**
 * @brief Prototype for the timer callback function.
 * @param context Pointer to user-defined context data (can be NULL).
 */
typedef void (*timer_callback_t)(void* context);

/* ============================================================================================== */

/**
 * @brief Timer operations structure. This structure holds function pointers for timer operations.
 */
struct timer_ops
{
    /**
     * @brief Configures a non-blocking delay in milliseconds.
     * @param self Pointer to the timer structure.
     * @param ms Delay in milliseconds.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*delay_ms)(const struct timer* self, uint16_t ms);

    /**
     * @brief Configures a non-blocking delay in microseconds.
     * @param self Pointer to the timer structure.
     * @param us Delay in microseconds.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*delay_us)(const struct timer* self, uint16_t us);

    /**
     * @brief Configures a timeout for the timer in milliseconds. When the timeout expires, the
     * callback function set by 'set_callback' will be called.
     * @param self Pointer to the timer structure.
     * @param ms Timeout in milliseconds.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_timeout_ms)(const struct timer* self, uint16_t ms);

    /**
     * @brief Resets the timer countdown.
     * @param self Pointer to the timer structure.
     */
    void (*reset_timeout)(const struct timer* self);

    /**
     * @brief Deactivates the timer.
     * @param self Pointer to the timer structure.
     */
    void (*deactivate_timeout)(const struct timer* self);

    /**
     * @brief Sets the callback function to be called when the timer expires.
     * @param self Pointer to the timer structure.
     * @param callback Pointer to the callback function.
     * @param context Pointer to user-defined context data (can be NULL).
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*set_timeout_callback)(const struct timer* self,
                                   timer_callback_t    callback,
                                   void*               context);
};

/* ============================================================================================== */

struct timer
{
    /**
     * @brief Timer identifier. Serves to identify different timer peripherals within the same
     * driver.
     */
    uint8_t id;

    /**
     * @brief Pointer to the user-defined callback_context data (can be NULL).
     */
    void* context;

    /**
     * @brief Pointer to the timer operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct timer_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct timer_ops* ops;
};

/* ============================================================================================== */

#endif
