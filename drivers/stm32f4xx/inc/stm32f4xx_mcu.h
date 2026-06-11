#ifndef STM32F4XX_MCU_H
#define STM32F4XX_MCU_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../../libraries/embedded-hal/inc/mcu.h"

/* ========================================================================== */

extern const struct mcu_ops stm32f4xx_mcu_ops;

/* ========================================================================== */

int8_t stm32f4xx_mcu_initialize(struct mcu* self);
int8_t stm32f4xx_mcu_reset(struct mcu* self);
int8_t stm32f4xx_mcu_sleep(struct mcu* self);
int8_t stm32f4xx_mcu_get_clock_frequency(
    const struct mcu* self, uint32_t* frequency);

/* ========================================================================== */

#endif /* STM32F4XX_MCU_H */
