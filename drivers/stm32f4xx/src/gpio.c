#include "../inc/gpio.h"

#include "stm32f4xx.h"

/* ========================================================================== */

const struct gpio_ops gpio_ops = {
    .initialize    = gpio_initialize,
    .set_direction = gpio_set_direction,
    .set_state     = gpio_set_state,
    .get_state     = gpio_get_state,
    .toggle        = gpio_toggle,
};

/* ========================================================================== */

static GPIO_TypeDef* const port_regs[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOH};

/* ========================================================================== */

uint8_t get_pin_from_id(uint8_t id)
{
    if (id == GPIO_D2_ID) return 2;
    if (id == GPIO_H0_ID) return 0;
    if (id == GPIO_H1_ID) return 1;
    return id % 16;
}

uint8_t get_port_from_id(uint8_t id)
{
    if (id <= GPIO_A15_ID) return PORT_A;
    if (id <= GPIO_B15_ID) return PORT_B;
    if (id <= GPIO_C15_ID) return PORT_C;
    if (id == GPIO_D2_ID)  return PORT_D;
    if (id == GPIO_H0_ID || id == GPIO_H1_ID) return PORT_H;
    return PORT_INVALID;
}

/* ========================================================================== */

static void enable_port_clock(uint8_t port)
{
    switch (port)
    {
        case PORT_A: RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; break;
        case PORT_B: RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; break;
        case PORT_C: RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; break;
        case PORT_D: RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; break;
        case PORT_H: RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; break;
        default: break;
    }
}

/* ========================================================================== */

int8_t gpio_initialize(struct gpio* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    uint8_t port = get_port_from_id(self->id);
    uint8_t pin  = get_pin_from_id(self->id);

    if (port == PORT_INVALID)
    {
        return -EINVAL;
    }

    enable_port_clock(port);

    GPIO_TypeDef* p = port_regs[port];

    uint32_t moder_val;
    if (self->type == GPIO_ANALOG)
    {
        moder_val = 3U;
    }
    else if (self->direction == GPIO_OUTPUT)
    {
        moder_val = 1U;
    }
    else
    {
        moder_val = 0U;
    }

    p->MODER &= ~(3U << (pin * 2));
    p->MODER |=  (moder_val << (pin * 2));

    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t gpio_set_direction(struct gpio* self, enum gpio_direction direction)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    uint8_t port = get_port_from_id(self->id);
    uint8_t pin  = get_pin_from_id(self->id);

    if (port == PORT_INVALID)
    {
        return -EINVAL;
    }

    GPIO_TypeDef* p = port_regs[port];

    p->MODER &= ~(3U << (pin * 2));
    if (direction == GPIO_OUTPUT)
    {
        p->MODER |= (1U << (pin * 2));
    }

    self->direction = direction;
    return 0;
}

/* ========================================================================== */

int8_t gpio_set_state(const struct gpio* self, bool state)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    uint8_t port = get_port_from_id(self->id);
    uint8_t pin  = get_pin_from_id(self->id);

    if (port == PORT_INVALID)
    {
        return -EINVAL;
    }

    if (state)
    {
        port_regs[port]->BSRR = (1U << pin);
    }
    else
    {
        port_regs[port]->BSRR = (1U << (pin + 16));
    }

    return 0;
}

/* ========================================================================== */

int8_t gpio_get_state(const struct gpio* self, bool* state)
{
    if (self == NULL || state == NULL)
    {
        return -EFAULT;
    }

    uint8_t port = get_port_from_id(self->id);
    uint8_t pin  = get_pin_from_id(self->id);

    if (port == PORT_INVALID)
    {
        return -EINVAL;
    }

    *state = (port_regs[port]->IDR & (1U << pin)) != 0;
    return 0;
}

/* ========================================================================== */

int8_t gpio_toggle(const struct gpio* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    uint8_t port = get_port_from_id(self->id);
    uint8_t pin  = get_pin_from_id(self->id);

    if (port == PORT_INVALID)
    {
        return -EINVAL;
    }

    port_regs[port]->ODR ^= (1U << pin);
    return 0;
}

/* ========================================================================== */
