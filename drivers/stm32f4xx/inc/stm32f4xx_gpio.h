#ifndef STM32F4XX_GPIO_H
#define STM32F4XX_GPIO_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../../libraries/embedded-hal/inc/gpio.h"

/* ========================================================================== */

extern const struct gpio_ops stm32f4xx_gpio_ops;

/* ========================================================================== */

/**
 * @brief GPIO pin IDs. Pass these to GPIO_DEFINE as the pin_id argument.
 */
#define GPIO_A0_ID   0
#define GPIO_A1_ID   1
#define GPIO_A2_ID   2
#define GPIO_A3_ID   3
#define GPIO_A4_ID   4
#define GPIO_A5_ID   5
#define GPIO_A6_ID   6
#define GPIO_A7_ID   7
#define GPIO_A8_ID   8
#define GPIO_A9_ID   9
#define GPIO_A10_ID  10
#define GPIO_A11_ID  11
#define GPIO_A12_ID  12
#define GPIO_A13_ID  13
#define GPIO_A14_ID  14
#define GPIO_A15_ID  15
#define GPIO_B0_ID   16
#define GPIO_B1_ID   17
#define GPIO_B2_ID   18
#define GPIO_B3_ID   19
#define GPIO_B4_ID   20
#define GPIO_B5_ID   21
#define GPIO_B6_ID   22
#define GPIO_B7_ID   23
#define GPIO_B8_ID   24
#define GPIO_B9_ID   25
#define GPIO_B10_ID  26
#define GPIO_B11_ID  27
#define GPIO_B12_ID  28
#define GPIO_B13_ID  29
#define GPIO_B14_ID  30
#define GPIO_B15_ID  31
#define GPIO_C0_ID   32
#define GPIO_C1_ID   33
#define GPIO_C2_ID   34
#define GPIO_C3_ID   35
#define GPIO_C4_ID   36
#define GPIO_C5_ID   37
#define GPIO_C6_ID   38
#define GPIO_C7_ID   39
#define GPIO_C8_ID   40
#define GPIO_C9_ID   41
#define GPIO_C10_ID  42
#define GPIO_C11_ID  43
#define GPIO_C12_ID  44
#define GPIO_C13_ID  45
#define GPIO_C14_ID  46
#define GPIO_C15_ID  47
#define GPIO_D2_ID   48
#define GPIO_H0_ID   49
#define GPIO_H1_ID   50

/* ========================================================================== */

#define PORT_A       0
#define PORT_B       1
#define PORT_C       2
#define PORT_D       3
#define PORT_H       4
#define PORT_INVALID 0xFF

/* ========================================================================== */

/**
 * @brief Pin mode definitions. Use these in your board's pinout_config.h.
 *        Modes can be combined with | (e.g. PIN_DI | PIN_PU).
 */
#define PIN_NC       0x00 /* Not connected (defaults to input, no pull) */
#define PIN_DI       0x01 /* Digital input */
#define PIN_DO       0x02 /* Digital output */
#define PIN_AI       0x04 /* Analog input */
#define PIN_AF       0x08 /* Alternate function */
#define PIN_OD       0x10 /* Open-drain modifier (use with PIN_DO or PIN_AF) */
#define PIN_PU       0x20 /* Pull-up modifier */
#define PIN_PD       0x40 /* Pull-down modifier */

/* ========================================================================== */

/**
 * @brief Alternate function numbers for STM32F401 peripherals.
 *        Source: DS9716 Table 9 "Alternate function mapping".
 *        Use these as GPIO_Xx_AF values in your pinout_config.h.
 */

#define USART1_TX_AF 7
#define USART1_RX_AF 7
#define USART2_TX_AF 7
#define USART2_RX_AF 7
#define USART6_TX_AF 8
#define USART6_RX_AF 8
#define SPI1_SCK_AF  5
#define SPI1_MISO_AF 5
#define SPI1_MOSI_AF 5
#define SPI1_NSS_AF  5
#define SPI2_SCK_AF  5
#define SPI2_MISO_AF 5
#define SPI2_MOSI_AF 5
#define SPI3_SCK_AF  6
#define SPI3_MISO_AF 6
#define SPI3_MOSI_AF 6
#define I2C1_SCL_AF  4
#define I2C1_SDA_AF  4
#define I2C2_SCL_AF  4
#define I2C2_SDA_AF  4
#define I2C3_SCL_AF  4
#define I2C3_SDA_AF  4
#define TIM1_CH1_AF  1
#define TIM1_CH2_AF  1
#define TIM1_CH3_AF  1
#define TIM1_CH4_AF  1
#define TIM2_CH1_AF  1
#define TIM2_CH2_AF  1
#define TIM2_CH3_AF  1
#define TIM2_CH4_AF  1

/* ========================================================================== */

/* ========================================================================== */

int8_t gpio_initialize(struct gpio* self);
int8_t gpio_set_direction(struct gpio* self, enum gpio_direction direction);
int8_t gpio_set_state(const struct gpio* self, bool state);
int8_t gpio_get_state(const struct gpio* self, bool* state);
int8_t gpio_toggle(const struct gpio* self);

/* ========================================================================== */

#endif /* STM32F4XX_GPIO_H */
