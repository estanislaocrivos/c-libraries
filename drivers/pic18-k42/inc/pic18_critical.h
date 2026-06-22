/**
 * @file
 * @brief PIC18-K42 critical section macros. High-performance, inline macros for
 * interrupt control. Uses save/restore pattern - each ENTER/EXIT pair is
 * independent.
 */

#ifndef PIC18_CRITICAL_H
#define PIC18_CRITICAL_H

/* ========================================================================== */

#include "pic18_k42.h"

/* ========================================================================== */

/* State type: stores the GIE bit value */
#define CRITICAL_SECTION_STATE_T uint8_t

/**
 * @brief Enter critical section: save GIE state and disable interrupts.
 * @param state Variable to store the previous interrupt state.
 */
#define CRITICAL_SECTION_ENTER(state)      \
    do                                     \
    {                                      \
        (state)         = INTCON0bits.GIE; \
        INTCON0bits.GIE = 0;               \
    } while (0)

/**
 * @brief Exit critical section: restore previous interrupt state.
 * @param state Variable containing the saved interrupt state.
 */
#define CRITICAL_SECTION_EXIT(state) \
    do                               \
    {                                \
        INTCON0bits.GIE = (state);   \
    } while (0)

/* State type: stores the GIEL bit value */
#define LOW_PRIOR_CRITICAL_SECTION_STATE_T uint8_t

/**
 * @brief Enter low-priority critical section: save GIEL state and disable
 * low-priority interrupts only. High-priority ISRs remain enabled.
 * @param state Variable to store the previous interrupt state.
 */
#define LOW_PRIOR_CRITICAL_SECTION_ENTER(state) \
    do                                          \
    {                                           \
        (state)          = INTCON0bits.GIEL;    \
        INTCON0bits.GIEL = 0;                   \
    } while (0)

/**
 * @brief Exit low-priority critical section: restore previous GIEL state.
 * @param state Variable containing the saved interrupt state.
 */
#define LOW_PRIOR_CRITICAL_SECTION_EXIT(state) \
    do                                         \
    {                                          \
        INTCON0bits.GIEL = (state);            \
    } while (0)

/**
 * @brief Simple disable/enable (no nesting support).
 * Use only when you're certain there's no nesting.
 */
#define INTERRUPTS_DISABLE() (INTCON0bits.GIE = 0)
#define INTERRUPTS_ENABLE()  (INTCON0bits.GIE = 1)

/* ========================================================================== */

/**
 * @brief Clear the watchdog timer.
 */
#define CLEAR_WDT()          CLRWDT()

/* ========================================================================== */

#endif /* PIC18_CRITICAL_H */
