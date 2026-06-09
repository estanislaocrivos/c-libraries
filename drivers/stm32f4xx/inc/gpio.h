#ifndef GPIO_H
#define GPIO_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../../libraries/embedded-hal/inc/gpio.h"
#include "../../../libraries/inc/errno.h"

/* ========================================================================== */

extern const struct gpio_ops gpio_ops;

/* ========================================================================== */

/**
 * @brief GPIO pin IDs. Pass these to GPIO_DEFINE as the pin_id argument.
 */
#define GPIO_A0_ID               0
#define GPIO_A1_ID               1
#define GPIO_A2_ID               2
#define GPIO_A3_ID               3
#define GPIO_A4_ID               4
#define GPIO_A5_ID               5
#define GPIO_A6_ID               6
#define GPIO_A7_ID               7
#define GPIO_A8_ID               8
#define GPIO_A9_ID               9
#define GPIO_A10_ID              10
#define GPIO_A11_ID              11
#define GPIO_A12_ID              12
#define GPIO_A13_ID              13
#define GPIO_A14_ID              14
#define GPIO_A15_ID              15
#define GPIO_B0_ID               16
#define GPIO_B1_ID               17
#define GPIO_B2_ID               18
#define GPIO_B3_ID               19
#define GPIO_B4_ID               20
#define GPIO_B5_ID               21
#define GPIO_B6_ID               22
#define GPIO_B7_ID               23
#define GPIO_B8_ID               24
#define GPIO_B9_ID               25
#define GPIO_B10_ID              26
#define GPIO_B11_ID              27
#define GPIO_B12_ID              28
#define GPIO_B13_ID              29
#define GPIO_B14_ID              30
#define GPIO_B15_ID              31
#define GPIO_C0_ID               32
#define GPIO_C1_ID               33
#define GPIO_C2_ID               34
#define GPIO_C3_ID               35
#define GPIO_C4_ID               36
#define GPIO_C5_ID               37
#define GPIO_C6_ID               38
#define GPIO_C7_ID               39
#define GPIO_C8_ID               40
#define GPIO_C9_ID               41
#define GPIO_C10_ID              42
#define GPIO_C11_ID              43
#define GPIO_C12_ID              44
#define GPIO_C13_ID              45
#define GPIO_C14_ID              46
#define GPIO_C15_ID              47
#define GPIO_D2_ID               48
#define GPIO_H0_ID               49
#define GPIO_H1_ID               50

/* ========================================================================== */

#define PORT_A                   0
#define PORT_B                   1
#define PORT_C                   2
#define PORT_D                   3
#define PORT_H                   4
#define PORT_INVALID             0xFF

/* ========================================================================== */

/**
 * @brief Pin mode definitions. Use these in your board's pinout_config.h.
 *        Modes can be combined with | (e.g. PIN_DI | PIN_PU).
 */
#define PIN_NC                   0x00 /* Not connected (defaults to input, no pull) */
#define PIN_DI                   0x01 /* Digital input */
#define PIN_DO                   0x02 /* Digital output */
#define PIN_AI                   0x04 /* Analog input */
#define PIN_AF                   0x08 /* Alternate function */
#define PIN_OD                   0x10 /* Open-drain modifier (use with PIN_DO or PIN_AF) */
#define PIN_PU                   0x20 /* Pull-up modifier */
#define PIN_PD                   0x40 /* Pull-down modifier */

/* ========================================================================== */

/**
 * @brief Alternate function numbers for STM32F401 peripherals.
 *        Source: DS9716 Table 9 "Alternate function mapping".
 *        Use these as GPIO_Xx_AF values in your pinout_config.h.
 */

#define USART1_TX_AF             7
#define USART1_RX_AF             7
#define USART2_TX_AF             7
#define USART2_RX_AF             7
#define USART6_TX_AF             8
#define USART6_RX_AF             8
#define SPI1_SCK_AF              5
#define SPI1_MISO_AF             5
#define SPI1_MOSI_AF             5
#define SPI1_NSS_AF              5
#define SPI2_SCK_AF              5
#define SPI2_MISO_AF             5
#define SPI2_MOSI_AF             5
#define SPI3_SCK_AF              6
#define SPI3_MISO_AF             6
#define SPI3_MOSI_AF             6
#define I2C1_SCL_AF              4
#define I2C1_SDA_AF              4
#define I2C2_SCL_AF              4
#define I2C2_SDA_AF              4
#define I2C3_SCL_AF              4
#define I2C3_SDA_AF              4
#define TIM1_CH1_AF              1
#define TIM1_CH2_AF              1
#define TIM1_CH3_AF              1
#define TIM1_CH4_AF              1
#define TIM2_CH1_AF              1
#define TIM2_CH2_AF              1
#define TIM2_CH3_AF              1
#define TIM2_CH4_AF              1

/* ========================================================================== */

/**
 * @brief Helper macros to check pin mode bits.
 */
#define GPIO_IS_INPUT(mode)      ((!((mode) & PIN_DO) && !((mode) & PIN_AF)) ? 1 : 0)
#define GPIO_IS_OUTPUT(mode)     ((mode) & PIN_DO ? 1 : 0)
#define GPIO_IS_ANALOG(mode)     ((mode) == PIN_AI ? 1 : 0)
#define GPIO_IS_AF(mode)         ((mode) & PIN_AF ? 1 : 0)
#define GPIO_IS_OPEN_DRAIN(mode) ((mode) & PIN_OD ? 1 : 0)
#define GPIO_IS_PULL_UP(mode)    ((mode) & PIN_PU ? 1 : 0)
#define GPIO_IS_PULL_DOWN(mode)  ((mode) & PIN_PD ? 1 : 0)

/* ========================================================================== */

/**
 * @brief Compute the 2-bit MODER field value for a given pin mode.
 *        MODER: 00=input, 01=output, 10=AF, 11=analog.
 */
#define MODER_VAL(mode)          \
    (GPIO_IS_INPUT(mode)  ? 0U : \
     GPIO_IS_OUTPUT(mode) ? 1U : \
     GPIO_IS_AF(mode)     ? 2U : \
     GPIO_IS_ANALOG(mode) ? 3U : \
                            0U)

/**
 * @brief Compute the 2-bit PUPDR field value for a given pin mode.
 *        PUPDR: 00=none, 01=pull-up, 10=pull-down.
 */
#define PUPDR_VAL(mode) \
    (GPIO_IS_PULL_UP(mode) ? 1U : GPIO_IS_PULL_DOWN(mode) ? 2U : 0U)

/* ========================================================================== */

/**
 * @brief Per-pin register field macros. These shift the field value into the
 *        correct bit position for a given pin number within the register.
 */
#define GPIO_MODER_BIT(mode, pin)  ((uint32_t)MODER_VAL(mode) << ((pin) * 2))
#define GPIO_OTYPER_BIT(mode, pin) ((uint32_t)GPIO_IS_OPEN_DRAIN(mode) << (pin))
#define GPIO_PUPDR_BIT(mode, pin)  ((uint32_t)PUPDR_VAL(mode) << ((pin) * 2))
#define GPIO_AFRL_BIT(mode, af, pin) \
    ((uint32_t)(GPIO_IS_AF(mode) ? (af) : 0U) << ((pin) * 4))
#define GPIO_AFRH_BIT(mode, af, pin) \
    ((uint32_t)(GPIO_IS_AF(mode) ? (af) : 0U) << (((pin) - 8) * 4))

/* ========================================================================== */

/**
 * @brief Macros to compute full register values from per-pin definitions.
 *        Requires GPIO_Ax, GPIO_Bx, etc. and GPIO_Ax_AF, etc. to be defined
 *        in your board's pinout_config.h before including this header.
 */

#define GPIOA_MODER_CONFIG                                         \
    (GPIO_MODER_BIT(GPIO_A0, 0) | GPIO_MODER_BIT(GPIO_A1, 1)       \
     | GPIO_MODER_BIT(GPIO_A2, 2) | GPIO_MODER_BIT(GPIO_A3, 3)     \
     | GPIO_MODER_BIT(GPIO_A4, 4) | GPIO_MODER_BIT(GPIO_A5, 5)     \
     | GPIO_MODER_BIT(GPIO_A6, 6) | GPIO_MODER_BIT(GPIO_A7, 7)     \
     | GPIO_MODER_BIT(GPIO_A8, 8) | GPIO_MODER_BIT(GPIO_A9, 9)     \
     | GPIO_MODER_BIT(GPIO_A10, 10) | GPIO_MODER_BIT(GPIO_A11, 11) \
     | GPIO_MODER_BIT(GPIO_A12, 12) | GPIO_MODER_BIT(GPIO_A13, 13) \
     | GPIO_MODER_BIT(GPIO_A14, 14) | GPIO_MODER_BIT(GPIO_A15, 15))

#define GPIOA_OTYPER_CONFIG                                          \
    (GPIO_OTYPER_BIT(GPIO_A0, 0) | GPIO_OTYPER_BIT(GPIO_A1, 1)       \
     | GPIO_OTYPER_BIT(GPIO_A2, 2) | GPIO_OTYPER_BIT(GPIO_A3, 3)     \
     | GPIO_OTYPER_BIT(GPIO_A4, 4) | GPIO_OTYPER_BIT(GPIO_A5, 5)     \
     | GPIO_OTYPER_BIT(GPIO_A6, 6) | GPIO_OTYPER_BIT(GPIO_A7, 7)     \
     | GPIO_OTYPER_BIT(GPIO_A8, 8) | GPIO_OTYPER_BIT(GPIO_A9, 9)     \
     | GPIO_OTYPER_BIT(GPIO_A10, 10) | GPIO_OTYPER_BIT(GPIO_A11, 11) \
     | GPIO_OTYPER_BIT(GPIO_A12, 12) | GPIO_OTYPER_BIT(GPIO_A13, 13) \
     | GPIO_OTYPER_BIT(GPIO_A14, 14) | GPIO_OTYPER_BIT(GPIO_A15, 15))

#define GPIOA_PUPDR_CONFIG                                         \
    (GPIO_PUPDR_BIT(GPIO_A0, 0) | GPIO_PUPDR_BIT(GPIO_A1, 1)       \
     | GPIO_PUPDR_BIT(GPIO_A2, 2) | GPIO_PUPDR_BIT(GPIO_A3, 3)     \
     | GPIO_PUPDR_BIT(GPIO_A4, 4) | GPIO_PUPDR_BIT(GPIO_A5, 5)     \
     | GPIO_PUPDR_BIT(GPIO_A6, 6) | GPIO_PUPDR_BIT(GPIO_A7, 7)     \
     | GPIO_PUPDR_BIT(GPIO_A8, 8) | GPIO_PUPDR_BIT(GPIO_A9, 9)     \
     | GPIO_PUPDR_BIT(GPIO_A10, 10) | GPIO_PUPDR_BIT(GPIO_A11, 11) \
     | GPIO_PUPDR_BIT(GPIO_A12, 12) | GPIO_PUPDR_BIT(GPIO_A13, 13) \
     | GPIO_PUPDR_BIT(GPIO_A14, 14) | GPIO_PUPDR_BIT(GPIO_A15, 15))

#define GPIOA_AFRL_CONFIG                    \
    (GPIO_AFRL_BIT(GPIO_A0, GPIO_A0_AF, 0)   \
     | GPIO_AFRL_BIT(GPIO_A1, GPIO_A1_AF, 1) \
     | GPIO_AFRL_BIT(GPIO_A2, GPIO_A2_AF, 2) \
     | GPIO_AFRL_BIT(GPIO_A3, GPIO_A3_AF, 3) \
     | GPIO_AFRL_BIT(GPIO_A4, GPIO_A4_AF, 4) \
     | GPIO_AFRL_BIT(GPIO_A5, GPIO_A5_AF, 5) \
     | GPIO_AFRL_BIT(GPIO_A6, GPIO_A6_AF, 6) \
     | GPIO_AFRL_BIT(GPIO_A7, GPIO_A7_AF, 7))

#define GPIOA_AFRH_CONFIG                       \
    (GPIO_AFRH_BIT(GPIO_A8, GPIO_A8_AF, 8)      \
     | GPIO_AFRH_BIT(GPIO_A9, GPIO_A9_AF, 9)    \
     | GPIO_AFRH_BIT(GPIO_A10, GPIO_A10_AF, 10) \
     | GPIO_AFRH_BIT(GPIO_A11, GPIO_A11_AF, 11) \
     | GPIO_AFRH_BIT(GPIO_A12, GPIO_A12_AF, 12) \
     | GPIO_AFRH_BIT(GPIO_A13, GPIO_A13_AF, 13) \
     | GPIO_AFRH_BIT(GPIO_A14, GPIO_A14_AF, 14) \
     | GPIO_AFRH_BIT(GPIO_A15, GPIO_A15_AF, 15))

/* ========================================================================== */

#define GPIOB_MODER_CONFIG                                         \
    (GPIO_MODER_BIT(GPIO_B0, 0) | GPIO_MODER_BIT(GPIO_B1, 1)       \
     | GPIO_MODER_BIT(GPIO_B2, 2) | GPIO_MODER_BIT(GPIO_B3, 3)     \
     | GPIO_MODER_BIT(GPIO_B4, 4) | GPIO_MODER_BIT(GPIO_B5, 5)     \
     | GPIO_MODER_BIT(GPIO_B6, 6) | GPIO_MODER_BIT(GPIO_B7, 7)     \
     | GPIO_MODER_BIT(GPIO_B8, 8) | GPIO_MODER_BIT(GPIO_B9, 9)     \
     | GPIO_MODER_BIT(GPIO_B10, 10) | GPIO_MODER_BIT(GPIO_B11, 11) \
     | GPIO_MODER_BIT(GPIO_B12, 12) | GPIO_MODER_BIT(GPIO_B13, 13) \
     | GPIO_MODER_BIT(GPIO_B14, 14) | GPIO_MODER_BIT(GPIO_B15, 15))

#define GPIOB_OTYPER_CONFIG                                          \
    (GPIO_OTYPER_BIT(GPIO_B0, 0) | GPIO_OTYPER_BIT(GPIO_B1, 1)       \
     | GPIO_OTYPER_BIT(GPIO_B2, 2) | GPIO_OTYPER_BIT(GPIO_B3, 3)     \
     | GPIO_OTYPER_BIT(GPIO_B4, 4) | GPIO_OTYPER_BIT(GPIO_B5, 5)     \
     | GPIO_OTYPER_BIT(GPIO_B6, 6) | GPIO_OTYPER_BIT(GPIO_B7, 7)     \
     | GPIO_OTYPER_BIT(GPIO_B8, 8) | GPIO_OTYPER_BIT(GPIO_B9, 9)     \
     | GPIO_OTYPER_BIT(GPIO_B10, 10) | GPIO_OTYPER_BIT(GPIO_B11, 11) \
     | GPIO_OTYPER_BIT(GPIO_B12, 12) | GPIO_OTYPER_BIT(GPIO_B13, 13) \
     | GPIO_OTYPER_BIT(GPIO_B14, 14) | GPIO_OTYPER_BIT(GPIO_B15, 15))

#define GPIOB_PUPDR_CONFIG                                         \
    (GPIO_PUPDR_BIT(GPIO_B0, 0) | GPIO_PUPDR_BIT(GPIO_B1, 1)       \
     | GPIO_PUPDR_BIT(GPIO_B2, 2) | GPIO_PUPDR_BIT(GPIO_B3, 3)     \
     | GPIO_PUPDR_BIT(GPIO_B4, 4) | GPIO_PUPDR_BIT(GPIO_B5, 5)     \
     | GPIO_PUPDR_BIT(GPIO_B6, 6) | GPIO_PUPDR_BIT(GPIO_B7, 7)     \
     | GPIO_PUPDR_BIT(GPIO_B8, 8) | GPIO_PUPDR_BIT(GPIO_B9, 9)     \
     | GPIO_PUPDR_BIT(GPIO_B10, 10) | GPIO_PUPDR_BIT(GPIO_B11, 11) \
     | GPIO_PUPDR_BIT(GPIO_B12, 12) | GPIO_PUPDR_BIT(GPIO_B13, 13) \
     | GPIO_PUPDR_BIT(GPIO_B14, 14) | GPIO_PUPDR_BIT(GPIO_B15, 15))

#define GPIOB_AFRL_CONFIG                    \
    (GPIO_AFRL_BIT(GPIO_B0, GPIO_B0_AF, 0)   \
     | GPIO_AFRL_BIT(GPIO_B1, GPIO_B1_AF, 1) \
     | GPIO_AFRL_BIT(GPIO_B2, GPIO_B2_AF, 2) \
     | GPIO_AFRL_BIT(GPIO_B3, GPIO_B3_AF, 3) \
     | GPIO_AFRL_BIT(GPIO_B4, GPIO_B4_AF, 4) \
     | GPIO_AFRL_BIT(GPIO_B5, GPIO_B5_AF, 5) \
     | GPIO_AFRL_BIT(GPIO_B6, GPIO_B6_AF, 6) \
     | GPIO_AFRL_BIT(GPIO_B7, GPIO_B7_AF, 7))

#define GPIOB_AFRH_CONFIG                       \
    (GPIO_AFRH_BIT(GPIO_B8, GPIO_B8_AF, 8)      \
     | GPIO_AFRH_BIT(GPIO_B9, GPIO_B9_AF, 9)    \
     | GPIO_AFRH_BIT(GPIO_B10, GPIO_B10_AF, 10) \
     | GPIO_AFRH_BIT(GPIO_B11, GPIO_B11_AF, 11) \
     | GPIO_AFRH_BIT(GPIO_B12, GPIO_B12_AF, 12) \
     | GPIO_AFRH_BIT(GPIO_B13, GPIO_B13_AF, 13) \
     | GPIO_AFRH_BIT(GPIO_B14, GPIO_B14_AF, 14) \
     | GPIO_AFRH_BIT(GPIO_B15, GPIO_B15_AF, 15))

/* ========================================================================== */

#define GPIOC_MODER_CONFIG                                         \
    (GPIO_MODER_BIT(GPIO_C0, 0) | GPIO_MODER_BIT(GPIO_C1, 1)       \
     | GPIO_MODER_BIT(GPIO_C2, 2) | GPIO_MODER_BIT(GPIO_C3, 3)     \
     | GPIO_MODER_BIT(GPIO_C4, 4) | GPIO_MODER_BIT(GPIO_C5, 5)     \
     | GPIO_MODER_BIT(GPIO_C6, 6) | GPIO_MODER_BIT(GPIO_C7, 7)     \
     | GPIO_MODER_BIT(GPIO_C8, 8) | GPIO_MODER_BIT(GPIO_C9, 9)     \
     | GPIO_MODER_BIT(GPIO_C10, 10) | GPIO_MODER_BIT(GPIO_C11, 11) \
     | GPIO_MODER_BIT(GPIO_C12, 12) | GPIO_MODER_BIT(GPIO_C13, 13) \
     | GPIO_MODER_BIT(GPIO_C14, 14) | GPIO_MODER_BIT(GPIO_C15, 15))

#define GPIOC_OTYPER_CONFIG                                          \
    (GPIO_OTYPER_BIT(GPIO_C0, 0) | GPIO_OTYPER_BIT(GPIO_C1, 1)       \
     | GPIO_OTYPER_BIT(GPIO_C2, 2) | GPIO_OTYPER_BIT(GPIO_C3, 3)     \
     | GPIO_OTYPER_BIT(GPIO_C4, 4) | GPIO_OTYPER_BIT(GPIO_C5, 5)     \
     | GPIO_OTYPER_BIT(GPIO_C6, 6) | GPIO_OTYPER_BIT(GPIO_C7, 7)     \
     | GPIO_OTYPER_BIT(GPIO_C8, 8) | GPIO_OTYPER_BIT(GPIO_C9, 9)     \
     | GPIO_OTYPER_BIT(GPIO_C10, 10) | GPIO_OTYPER_BIT(GPIO_C11, 11) \
     | GPIO_OTYPER_BIT(GPIO_C12, 12) | GPIO_OTYPER_BIT(GPIO_C13, 13) \
     | GPIO_OTYPER_BIT(GPIO_C14, 14) | GPIO_OTYPER_BIT(GPIO_C15, 15))

#define GPIOC_PUPDR_CONFIG                                         \
    (GPIO_PUPDR_BIT(GPIO_C0, 0) | GPIO_PUPDR_BIT(GPIO_C1, 1)       \
     | GPIO_PUPDR_BIT(GPIO_C2, 2) | GPIO_PUPDR_BIT(GPIO_C3, 3)     \
     | GPIO_PUPDR_BIT(GPIO_C4, 4) | GPIO_PUPDR_BIT(GPIO_C5, 5)     \
     | GPIO_PUPDR_BIT(GPIO_C6, 6) | GPIO_PUPDR_BIT(GPIO_C7, 7)     \
     | GPIO_PUPDR_BIT(GPIO_C8, 8) | GPIO_PUPDR_BIT(GPIO_C9, 9)     \
     | GPIO_PUPDR_BIT(GPIO_C10, 10) | GPIO_PUPDR_BIT(GPIO_C11, 11) \
     | GPIO_PUPDR_BIT(GPIO_C12, 12) | GPIO_PUPDR_BIT(GPIO_C13, 13) \
     | GPIO_PUPDR_BIT(GPIO_C14, 14) | GPIO_PUPDR_BIT(GPIO_C15, 15))

#define GPIOC_AFRL_CONFIG                    \
    (GPIO_AFRL_BIT(GPIO_C0, GPIO_C0_AF, 0)   \
     | GPIO_AFRL_BIT(GPIO_C1, GPIO_C1_AF, 1) \
     | GPIO_AFRL_BIT(GPIO_C2, GPIO_C2_AF, 2) \
     | GPIO_AFRL_BIT(GPIO_C3, GPIO_C3_AF, 3) \
     | GPIO_AFRL_BIT(GPIO_C4, GPIO_C4_AF, 4) \
     | GPIO_AFRL_BIT(GPIO_C5, GPIO_C5_AF, 5) \
     | GPIO_AFRL_BIT(GPIO_C6, GPIO_C6_AF, 6) \
     | GPIO_AFRL_BIT(GPIO_C7, GPIO_C7_AF, 7))

#define GPIOC_AFRH_CONFIG                       \
    (GPIO_AFRH_BIT(GPIO_C8, GPIO_C8_AF, 8)      \
     | GPIO_AFRH_BIT(GPIO_C9, GPIO_C9_AF, 9)    \
     | GPIO_AFRH_BIT(GPIO_C10, GPIO_C10_AF, 10) \
     | GPIO_AFRH_BIT(GPIO_C11, GPIO_C11_AF, 11) \
     | GPIO_AFRH_BIT(GPIO_C12, GPIO_C12_AF, 12) \
     | GPIO_AFRH_BIT(GPIO_C13, GPIO_C13_AF, 13) \
     | GPIO_AFRH_BIT(GPIO_C14, GPIO_C14_AF, 14) \
     | GPIO_AFRH_BIT(GPIO_C15, GPIO_C15_AF, 15))

/* ========================================================================== */

/* GPIOD: only PD2 is available on STM32F401 LQFP64 */

#define GPIOD_MODER_CONFIG  GPIO_MODER_BIT(GPIO_D2, 2)
#define GPIOD_OTYPER_CONFIG GPIO_OTYPER_BIT(GPIO_D2, 2)
#define GPIOD_PUPDR_CONFIG  GPIO_PUPDR_BIT(GPIO_D2, 2)
#define GPIOD_AFRL_CONFIG   GPIO_AFRL_BIT(GPIO_D2, GPIO_D2_AF, 2)

/* ========================================================================== */

/* GPIOH: only PH0 and PH1 (OSC_IN / OSC_OUT) */

#define GPIOH_MODER_CONFIG \
    (GPIO_MODER_BIT(GPIO_H0, 0) | GPIO_MODER_BIT(GPIO_H1, 1))
#define GPIOH_OTYPER_CONFIG \
    (GPIO_OTYPER_BIT(GPIO_H0, 0) | GPIO_OTYPER_BIT(GPIO_H1, 1))
#define GPIOH_PUPDR_CONFIG \
    (GPIO_PUPDR_BIT(GPIO_H0, 0) | GPIO_PUPDR_BIT(GPIO_H1, 1))
#define GPIOH_AFRL_CONFIG                  \
    (GPIO_AFRL_BIT(GPIO_H0, GPIO_H0_AF, 0) \
     | GPIO_AFRL_BIT(GPIO_H1, GPIO_H1_AF, 1))

/* ========================================================================== */

/**
 * @brief Retrieves the pin number (0-15) from the GPIO ID.
 */
uint8_t get_pin_from_id(uint8_t id);

/**
 * @brief Retrieves the port index (PORT_A..PORT_H) from the GPIO ID.
 */
uint8_t get_port_from_id(uint8_t id);

/* ========================================================================== */

int8_t gpio_initialize(struct gpio* self);
int8_t gpio_set_direction(struct gpio* self, enum gpio_direction direction);
int8_t gpio_set_state(const struct gpio* self, bool state);
int8_t gpio_get_state(const struct gpio* self, bool* state);
int8_t gpio_toggle(const struct gpio* self);

/* ========================================================================== */

#endif /* GPIO_H */
