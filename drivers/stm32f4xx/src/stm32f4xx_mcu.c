#include "../inc/mcu.h"

#include "stm32f4xx.h"

#ifdef PINOUT_CONFIG_HEADER
#include PINOUT_CONFIG_HEADER
#endif

#include "../inc/gpio.h"

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
