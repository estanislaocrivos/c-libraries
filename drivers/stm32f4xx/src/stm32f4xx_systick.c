#include "../inc/stm32f4xx_systick.h"

#include "../../../libraries/inc/errno.h"
#include "stm32f4xx.h"

/* ========================================================================== */

#define SYSTICK_RELOAD_MAX 0x00FFFFFFUL /* 24-bit reload register */

/* ========================================================================== */

static volatile uint32_t g_tick_ms = 0;

/* ========================================================================== */

int8_t stm32f4xx_systick_init(uint32_t core_clock_hz)
{
    uint32_t reload = core_clock_hz / 1000U;
    if (reload == 0U || reload > SYSTICK_RELOAD_MAX)
    {
        return -EINVAL;
    }

    g_tick_ms = 0;
    SysTick_Config(reload);
    return 0;
}

/* ========================================================================== */

uint32_t stm32f4xx_get_tick_ms(void)
{
    return g_tick_ms;
}

/* ========================================================================== */

void SysTick_Handler(void)
{
    g_tick_ms++;
}
