/**
 * @file delay.h
 * @brief Compile-time delay macros for HAL.
 *
 * Provides blocking delay macros that resolve at compile-time
 * (no function pointer overhead).
 *
 * Usage:
 *   #define DELAY_HEADER "pic18_delay.h"
 *   #include "delay.h"
 *
 *   void foo(void) {
 *       DELAY_MS(100);
 *       DELAY_US(50);
 *   }
 */

#ifndef DELAY_H
#define DELAY_H

/* Include platform-specific implementation */
#ifndef DELAY_HEADER
#error "DELAY_HEADER must be defined before including delay.h"
#endif

#include DELAY_HEADER

/* Verify required macros are defined */
#ifndef DELAY_MS
#error "Platform must define DELAY_MS(ms)"
#endif

#ifndef DELAY_US
#error "Platform must define DELAY_US(us)"
#endif

#endif /* DELAY_H */
