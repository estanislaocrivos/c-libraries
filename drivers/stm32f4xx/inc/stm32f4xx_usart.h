#ifndef STM32F4XX_USART_H
#define STM32F4XX_USART_H

/* ========================================================================== */

#include "../../../libraries/embedded-hal/inc/serial.h"

/* ========================================================================== */

/**
 * @brief APB1 peripheral clock in Hz. USART2 derives its baud rate from this.
 *        Override at build time if your RCC configuration differs.
 *        Default: 16 MHz (HSI, no PLL).
 */
#ifndef STM32F4XX_PCLK1_HZ
#define STM32F4XX_PCLK1_HZ 16000000UL
#endif

/**
 * @brief APB2 peripheral clock in Hz. USART1 and USART6 derive their baud
 *        rates from this. Override at build time if your RCC configuration
 *        differs. Default: 16 MHz (HSI, no PLL).
 */
#ifndef STM32F4XX_PCLK2_HZ
#define STM32F4XX_PCLK2_HZ 16000000UL
#endif

/* ========================================================================== */

/**
 * @brief serial_ops implementations for the three USART instances of the
 *        STM32F401. Pass &USARTn_OPS to SERIAL_DEFINE to bind a serial port
 *        to the desired peripheral.
 *
 * @note  GPIO alternate-function configuration is NOT performed by this
 *        driver. The caller (BSP) must configure TX/RX pins before calling
 *        the initialize op.
 *
 * @note  Requires CMSIS-Device (stm32f4xx.h) to be available on the include
 *        path of the consuming project. This driver does not vendor CMSIS.
 */
extern const struct serial_ops stm32f4xx_usart1_ops;
extern const struct serial_ops stm32f4xx_usart2_ops;
extern const struct serial_ops stm32f4xx_usart6_ops;

/* ========================================================================== */

#endif  // STM32F4XX_USART_H
