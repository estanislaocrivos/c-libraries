#include "../inc/gpio_driver.h"

/* ============================================================================================== */

int8_t gpio_set_state(const struct gpio* self, bool state)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    switch (self->id)
    {
        case GPIO_B7:
            if (state)
            {
                PORTB |= (1 << PB7);
            }
            else
            {
                PORTB &= ~(1 << PB7);
            }
            break;

        case GPIO_B6:
            if (state)
            {
                PORTB |= (1 << PB6);
            }
            else
            {
                PORTB &= ~(1 << PB6);
            }
            break;

        case GPIO_B5:
            if (state)
            {
                PORTB |= (1 << PB5);
            }
            else
            {
                PORTB &= ~(1 << PB5);
            }
            break;

        case GPIO_B4:
            if (state)
            {
                PORTB |= (1 << PB4);
            }
            else
            {
                PORTB &= ~(1 << PB4);
            }
            break;

        case GPIO_H6:
            if (state)
            {
                PORTH |= (1 << PH6);
            }
            else
            {
                PORTH &= ~(1 << PH6);
            }
            break;

        case GPIO_H5:
            if (state)
            {
                PORTH |= (1 << PH5);
            }
            else
            {
                PORTH &= ~(1 << PH5);
            }
            break;

        default:
            break;
    }
    return 0;
}

/* ============================================================================================== */

int8_t gpio_get_state(const struct gpio* self, bool* state)
{
    *state = false;
    return 0;
}

/* ============================================================================================== */

int8_t gpio_toggle(const struct gpio* self)
{
    return 0;
}

/* ============================================================================================== */
