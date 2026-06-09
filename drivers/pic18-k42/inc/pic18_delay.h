/**
 * @file pic18_delay.h
 * @brief PIC18-K42 blocking delay macros.
 *
 * Wraps XC8 compiler intrinsics for portable delay interface.
 * Requires _XTAL_FREQ to be defined for correct timing.
 */

#ifndef PIC18_DELAY_H
#define PIC18_DELAY_H

/* ========================================================================== */

#include "pic18_k42.h"

/* ========================================================================== */

#ifndef _XTAL_FREQ
#error "_XTAL_FREQ must be defined for delay macros"
#endif

/* ========================================================================== */

/**
 * @brief Blocking delay in milliseconds.
 * @param ms Delay time in milliseconds (compile-time constant recommended).
 */
#define DELAY_MS(ms) __delay_ms(ms)

/**
 * @brief Blocking delay in microseconds.
 * @param us Delay time in microseconds (compile-time constant recommended).
 */
#define DELAY_US(us) __delay_us(us)

/* ========================================================================== */

#endif /* PIC18_DELAY_H */
