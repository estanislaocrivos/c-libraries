#ifndef STM32F4XX_I2C_H
#define STM32F4XX_I2C_H

/* ========================================================================== */

#include "../../../libraries/embedded-hal/inc/i2c.h"

/* ========================================================================== */

/**
 * @brief APB1 peripheral clock in Hz. All I2C instances on the STM32F4xx
 *        derive their SCL frequency from this. Override at build time if your
 *        RCC configuration differs.
 *        Default: 16 MHz (HSI, no PLL).
 */
#ifndef STM32F4XX_PCLK1_HZ
#define STM32F4XX_PCLK1_HZ 16000000UL
#endif

/* ========================================================================== */

/**
 * @brief i2c_ops implementations for the I2C instances of the STM32F4xx.
 *        Pass &stm32f4xx_i2cX_ops to your i2c struct to bind it to the
 *        desired peripheral.
 *
 * @note  Only master mode and 7-bit addressing are supported.
 *
 * @note  Only standard mode (up to 100 kHz) and fast mode (up to 400 kHz)
 *        are supported.
 *
 * @note  GPIO alternate-function configuration is NOT performed by this
 *        driver. The caller (BSP) must configure SCL/SDA pins with open-drain
 *        output and the correct AF before calling the initialize op.
 *
 * @note  All ops use polling (no interrupts or DMA). The CPU busy-waits on
 *        each byte transfer.
 *
 * @note  read and read_raw with exactly 2 bytes have a known race condition
 *        (RM0368 Fig 162). Use them only for N=1 or N>=3 payload sizes.
 *
 * @note  Requires CMSIS-Device (stm32f4xx.h) to be available on the include
 *        path of the consuming project. This driver does not vendor CMSIS.
 */
extern const struct i2c_ops stm32f4xx_i2c1_ops;
extern const struct i2c_ops stm32f4xx_i2c2_ops;
extern const struct i2c_ops stm32f4xx_i2c3_ops;

/* ========================================================================== */

#endif /* STM32F4XX_I2C_H */
