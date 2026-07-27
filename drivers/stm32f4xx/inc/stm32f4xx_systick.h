#ifndef STM32F4XX_SYSTICK_H
#define STM32F4XX_SYSTICK_H

/* ========================================================================== */

#include <stdint.h>

/* ========================================================================== */

/**
 * @brief Configure the SysTick timer to tick every 1ms and reset the
 *        millisecond counter to 0. Must be called once at boot, after the
 *        core clock has been configured. Owns SysTick_Handler: no other
 *        translation unit in the final firmware may define it.
 * @param core_clock_hz Core clock frequency in Hz (e.g. 84000000UL).
 * @return int8_t Returns 0 on success, -EINVAL if core_clock_hz does not fit
 *         in the 24-bit SysTick reload register for a 1ms period.
 */
int8_t stm32f4xx_systick_init(uint32_t core_clock_hz);

/**
 * @brief Get the number of milliseconds elapsed since
 *        stm32f4xx_systick_init(). Wraps around every ~49.7 days.
 * @return uint32_t Milliseconds elapsed.
 */
uint32_t stm32f4xx_get_tick_ms(void);

/* ========================================================================== */

#endif /* STM32F4XX_SYSTICK_H */
