#include "../inc/stm32f4xx_mcu.h"

#include "../inc/stm32f4xx_gpio.h"
#include "stm32f4xx.h"

#ifdef PINOUT_CONFIG_HEADER
#include PINOUT_CONFIG_HEADER
#endif

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
 * @brief Per-port active-pin checks. Non-zero if any pin on the port is not NC.
 *        Requires GPIO_Ax, GPIO_Bx, etc. to be defined (via pinout_config.h).
 */
#define ANY_GPIO_A_IS_NOT_NC                                                   \
    (GPIO_A0 | GPIO_A1 | GPIO_A2 | GPIO_A3 | GPIO_A4 | GPIO_A5 | GPIO_A6       \
     | GPIO_A7 | GPIO_A8 | GPIO_A9 | GPIO_A10 | GPIO_A11 | GPIO_A12 | GPIO_A13 \
     | GPIO_A14 | GPIO_A15)

#define ANY_GPIO_B_IS_NOT_NC                                                   \
    (GPIO_B0 | GPIO_B1 | GPIO_B2 | GPIO_B3 | GPIO_B4 | GPIO_B5 | GPIO_B6       \
     | GPIO_B7 | GPIO_B8 | GPIO_B9 | GPIO_B10 | GPIO_B11 | GPIO_B12 | GPIO_B13 \
     | GPIO_B14 | GPIO_B15)

#define ANY_GPIO_C_IS_NOT_NC                                                   \
    (GPIO_C0 | GPIO_C1 | GPIO_C2 | GPIO_C3 | GPIO_C4 | GPIO_C5 | GPIO_C6       \
     | GPIO_C7 | GPIO_C8 | GPIO_C9 | GPIO_C10 | GPIO_C11 | GPIO_C12 | GPIO_C13 \
     | GPIO_C14 | GPIO_C15)

#define ANY_GPIO_D_IS_NOT_NC (GPIO_D2)

#define ANY_GPIO_H_IS_NOT_NC (GPIO_H0 | GPIO_H1)

/* ========================================================================== */

const struct mcu_ops stm32f4xx_mcu_ops = {
    .initialize          = stm32f4xx_mcu_initialize,
    .reset               = stm32f4xx_mcu_reset,
    .sleep               = stm32f4xx_mcu_sleep,
    .get_clock_frequency = stm32f4xx_mcu_get_clock_frequency,
};

/* ========================================================================== */

static void mcu_init_pinout(void)
{
    if (ANY_GPIO_A_IS_NOT_NC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER  = GPIOA_MODER_CONFIG;
        GPIOA->OTYPER = GPIOA_OTYPER_CONFIG;
        GPIOA->PUPDR  = GPIOA_PUPDR_CONFIG;
        GPIOA->AFR[0] = GPIOA_AFRL_CONFIG;
        GPIOA->AFR[1] = GPIOA_AFRH_CONFIG;
    }
    if (ANY_GPIO_B_IS_NOT_NC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        GPIOB->MODER  = GPIOB_MODER_CONFIG;
        GPIOB->OTYPER = GPIOB_OTYPER_CONFIG;
        GPIOB->PUPDR  = GPIOB_PUPDR_CONFIG;
        GPIOB->AFR[0] = GPIOB_AFRL_CONFIG;
        GPIOB->AFR[1] = GPIOB_AFRH_CONFIG;
    }
    if (ANY_GPIO_C_IS_NOT_NC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIOC->MODER  = GPIOC_MODER_CONFIG;
        GPIOC->OTYPER = GPIOC_OTYPER_CONFIG;
        GPIOC->PUPDR  = GPIOC_PUPDR_CONFIG;
        GPIOC->AFR[0] = GPIOC_AFRL_CONFIG;
        GPIOC->AFR[1] = GPIOC_AFRH_CONFIG;
    }
    if (ANY_GPIO_D_IS_NOT_NC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        GPIOD->MODER  = GPIOD_MODER_CONFIG;
        GPIOD->OTYPER = GPIOD_OTYPER_CONFIG;
        GPIOD->PUPDR  = GPIOD_PUPDR_CONFIG;
        GPIOD->AFR[0] = GPIOD_AFRL_CONFIG;
    }
    if (ANY_GPIO_H_IS_NOT_NC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
        GPIOH->MODER  = GPIOH_MODER_CONFIG;
        GPIOH->OTYPER = GPIOH_OTYPER_CONFIG;
        GPIOH->PUPDR  = GPIOH_PUPDR_CONFIG;
        GPIOH->AFR[0] = GPIOH_AFRL_CONFIG;
    }
}

/* ========================================================================== */

int8_t stm32f4xx_mcu_initialize(struct mcu* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    mcu_init_pinout();

    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t stm32f4xx_mcu_reset(struct mcu* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    NVIC_SystemReset();

    return 0;
}

/* ========================================================================== */

int8_t stm32f4xx_mcu_sleep(struct mcu* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    __WFI();

    return 0;
}

/* ========================================================================== */

int8_t stm32f4xx_mcu_get_clock_frequency(
    const struct mcu* self, uint32_t* frequency)
{
    if (self == NULL || frequency == NULL)
    {
        return -EFAULT;
    }

    *frequency = self->clock_frequency;
    return 0;
}

/* ========================================================================== */
