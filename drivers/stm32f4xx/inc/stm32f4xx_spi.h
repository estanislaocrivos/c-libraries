#ifndef STM32F4XX_SPI_H
#define STM32F4XX_SPI_H

/* ========================================================================== */

#include "../../../libraries/embedded-hal/inc/spi.h"

/* ========================================================================== */

/**
 * @brief APB1 peripheral clock in Hz. SPI2 and SPI3 derive their bit rate
 *        from this. Override at build time if your RCC configuration differs.
 *        Default: 16 MHz (HSI, no PLL).
 */
#ifndef STM32F4XX_PCLK1_HZ
#define STM32F4XX_PCLK1_HZ 16000000UL
#endif

/**
 * @brief APB2 peripheral clock in Hz. SPI1 and SPI4 derive their bit rate
 *        from this. Override at build time if your RCC configuration differs.
 *        Default: 16 MHz (HSI, no PLL).
 */
#ifndef STM32F4XX_PCLK2_HZ
#define STM32F4XX_PCLK2_HZ 16000000UL
#endif

/* ========================================================================== */

/**
 * @brief spi_ops implementations for the SPI instances of the STM32F401.
 *        Pass &SPInX_OPS to your spi struct to bind a port to the desired
 *        peripheral.
 *
 * @note  Only master mode is supported. struct spi::master must be true.
 *
 * @note  Chip Select is software-managed by the caller via GPIO. The set_cs
 *        op returns -ENOTSUP because the driver has no knowledge of which
 *        pin acts as CS.
 *
 * @note  GPIO alternate-function configuration is NOT performed by this
 *        driver. The caller (BSP) must configure SCK/MISO/MOSI pins before
 *        calling the initialize op.
 *
 * @note  Requires CMSIS-Device (stm32f4xx.h) to be available on the include
 *        path of the consuming project. This driver does not vendor CMSIS.
 */
extern const struct spi_ops stm32f4xx_spi1_ops;
extern const struct spi_ops stm32f4xx_spi2_ops;
extern const struct spi_ops stm32f4xx_spi3_ops;
extern const struct spi_ops stm32f4xx_spi4_ops;

/* ========================================================================== */

#endif  // STM32F4XX_SPI_H
