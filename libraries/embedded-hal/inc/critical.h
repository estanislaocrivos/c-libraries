/**
 * @file critical.h
 * @brief Compile-time critical section macros for HAL.
 *
 * Provides high-performance macros for interrupt enable/disable
 * that resolve at compile-time (no function pointer overhead).
 *
 * Usage:
 *   #include "critical.h"
 *
 *   void foo(void) {
 *       CRITICAL_SECTION_STATE_T state;
 *       CRITICAL_SECTION_ENTER(state);
 *       // ... protected code ...
 *       CRITICAL_SECTION_EXIT(state);
 *   }
 *
 * Or using the block macro:
 *   CRITICAL_SECTION({
 *       // ... protected code ...
 *   });
 */

#ifndef CRITICAL_H
#define CRITICAL_H

/* Include platform-specific implementation */
#ifndef CRITICAL_HEADER
#error "CRITICAL_HEADER must be defined before including critical.h"
#endif

#include CRITICAL_HEADER

/* Verify required macros are defined */
#ifndef CRITICAL_SECTION_STATE_T
#error "Platform must define CRITICAL_SECTION_STATE_T"
#endif

#ifndef CRITICAL_SECTION_ENTER
#error "Platform must define CRITICAL_SECTION_ENTER(state)"
#endif

#ifndef CRITICAL_SECTION_EXIT
#error "Platform must define CRITICAL_SECTION_EXIT(state)"
#endif

#ifndef CLEAR_WDT
#error "Platform must define CLEAR_WDT()"
#endif

/* Optional convenience macro for scoped critical sections */
#ifndef CRITICAL_SECTION
#define CRITICAL_SECTION(code)                 \
    do                                         \
    {                                          \
        CRITICAL_SECTION_STATE_T _cs_state;    \
        CRITICAL_SECTION_ENTER(_cs_state);     \
        code CRITICAL_SECTION_EXIT(_cs_state); \
    } while (0)
#endif

#endif /* CRITICAL_H */
