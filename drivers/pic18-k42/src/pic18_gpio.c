#include "../inc/pic18_gpio.h"

/* ========================================================================== */

const struct gpio_ops pic18_gpio_ops = {
    .initialize    = pic18_gpio_initialize,
    .set_direction = pic18_gpio_set_direction,
    .set_state     = pic18_gpio_set_state,
    .get_state     = pic18_gpio_get_state,
    .toggle        = pic18_gpio_toggle,
};

/* ========================================================================== */

inline uint8_t pic18_get_pin_from_id(uint8_t id)
{
    return id & 0x07;
}

inline uint8_t pic18_get_port_from_id(uint8_t id)
{
    if (id < GPIO_A0_ID + 8)
    {
        return PORT_A;
    }
    else if (id >= GPIO_B0_ID && id < GPIO_B0_ID + 8)
    {
        return PORT_B;
    }
    else if (id >= GPIO_C0_ID && id < GPIO_C0_ID + 8)
    {
        return PORT_C;
    }
#if defined(PIC18F47K42) || defined(PIC18F57K42)
    else if (id >= GPIO_D0_ID && id < GPIO_D0_ID + 8)
    {
        return PORT_D;
    }
    else if (id >= GPIO_E0_ID && id < GPIO_E0_ID + 8)
    {
        return PORT_E;
    }
#endif
#if defined(PIC18F57K42)
    else if (id >= GPIO_F0_ID && id < GPIO_F0_ID + 8)
    {
        return PORT_F;
    }
#endif
    else
    {
        return PORT_INVALID;
    }
}

static inline uint8_t get_mask(uint8_t id)
{
    return (uint8_t)(1U << pic18_get_pin_from_id(id));
}

/* Local aliases for convenience */
#define get_port(id) pic18_get_port_from_id(id)

/* ========================================================================== */

int8_t pic18_gpio_initialize(struct gpio* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    uint8_t port = get_port(self->id);
    uint8_t mask = get_mask(self->id);

    if (self->type == GPIO_ANALOG)
    {
        /* Analog pin initialization can be added here if needed */
        switch (port)
        {
            case PORT_A:
                ANSELA |= mask;
                break;
            case PORT_B:
                ANSELB |= mask;
                break;
            case PORT_C:
                ANSELC |= mask;
                break;
#if defined(PIC18F47K42) || defined(PIC18F57K42)
            case PORT_D:
                ANSELD |= mask;
                break;
            case PORT_E:
                ANSELE |= mask;
                break;
#endif
#if defined(PIC18F57K42)
            case PORT_F:
                ANSELF |= mask;
                break;
#endif
            default:
                return -EINVAL;
        }
    }
    else if (self->type == GPIO_DIGITAL)
    {
        /* Disable analog, set direction */
        switch (port)
        {
            case PORT_A:
                ANSELA &= ~mask;
                if (self->direction == GPIO_OUTPUT)
                    TRISA &= ~mask;
                else
                    TRISA |= mask;
                break;
            case PORT_B:
                ANSELB &= ~mask;
                if (self->direction == GPIO_OUTPUT)
                    TRISB &= ~mask;
                else
                    TRISB |= mask;
                break;
            case PORT_C:
                ANSELC &= ~mask;
                if (self->direction == GPIO_OUTPUT)
                    TRISC &= ~mask;
                else
                    TRISC |= mask;
                break;
#if defined(PIC18F47K42) || defined(PIC18F57K42)
            case PORT_D:
                ANSELD &= ~mask;
                if (self->direction == GPIO_OUTPUT)
                    TRISD &= ~mask;
                else
                    TRISD |= mask;
                break;
            case PORT_E:
                ANSELE &= ~mask;
                if (self->direction == GPIO_OUTPUT)
                    TRISE &= ~mask;
                else
                    TRISE |= mask;
                break;
#endif
#if defined(PIC18F57K42)
            case PORT_F:
                ANSELF &= ~mask;
                if (self->direction == GPIO_OUTPUT)
                    TRISF &= ~mask;
                else
                    TRISF |= mask;
                break;
#endif
            default:
                return -EINVAL;
        }
    }
    else
    {
        return -EINVAL;
    }
    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t pic18_gpio_set_direction(struct gpio* self, enum gpio_direction direction)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    uint8_t port = get_port(self->id);
    uint8_t mask = get_mask(self->id);
    switch (port)
    {
        case PORT_A:
            if (direction == GPIO_OUTPUT)
                TRISA &= ~mask;
            else
                TRISA |= mask;
            break;
        case PORT_B:
            if (direction == GPIO_OUTPUT)
                TRISB &= ~mask;
            else
                TRISB |= mask;
            break;
        case PORT_C:
            if (direction == GPIO_OUTPUT)
                TRISC &= ~mask;
            else
                TRISC |= mask;
            break;
#if defined(PIC18F47K42) || defined(PIC18F57K42)
        case PORT_D:
            if (direction == GPIO_OUTPUT)
                TRISD &= ~mask;
            else
                TRISD |= mask;
            break;
        case PORT_E:
            if (direction == GPIO_OUTPUT)
                TRISE &= ~mask;
            else
                TRISE |= mask;
            break;
#endif
#if defined(PIC18F57K42)
        case PORT_F:
            if (direction == GPIO_OUTPUT)
                TRISF &= ~mask;
            else
                TRISF |= mask;
            break;
#endif
        default:
            return -EINVAL;
    }
    self->direction = direction;
    return 0;
}

/* ========================================================================== */

int8_t pic18_gpio_set_state(const struct gpio* self, bool state)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    uint8_t port = get_port(self->id);
    uint8_t mask = get_mask(self->id);
    switch (port)
    {
        case PORT_A:
            if (state)
                LATA |= mask;
            else
                LATA &= ~mask;
            break;
        case PORT_B:
            if (state)
                LATB |= mask;
            else
                LATB &= ~mask;
            break;
        case PORT_C:
            if (state)
                LATC |= mask;
            else
                LATC &= ~mask;
            break;
#if defined(PIC18F47K42) || defined(PIC18F57K42)
        case PORT_D:
            if (state)
                LATD |= mask;
            else
                LATD &= ~mask;
            break;
        case PORT_E:
            if (state)
                LATE |= mask;
            else
                LATE &= ~mask;
            break;
#endif
#if defined(PIC18F57K42)
        case PORT_F:
            if (state)
                LATF |= mask;
            else
                LATF &= ~mask;
            break;
#endif
        default:
            return -EINVAL;
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_gpio_get_state(const struct gpio* self, bool* state)
{
    if (self == NULL || state == NULL)
    {
        return -EFAULT;
    }
    uint8_t port = get_port(self->id);
    uint8_t mask = get_mask(self->id);
    switch (port)
    {
        case PORT_A:
            *state = (PORTA & mask) != 0;
            break;
        case PORT_B:
            *state = (PORTB & mask) != 0;
            break;
        case PORT_C:
            *state = (PORTC & mask) != 0;
            break;
#if defined(PIC18F47K42) || defined(PIC18F57K42)
        case PORT_D:
            *state = (PORTD & mask) != 0;
            break;
        case PORT_E:
            *state = (PORTE & mask) != 0;
            break;
#endif
#if defined(PIC18F57K42)
        case PORT_F:
            *state = (PORTF & mask) != 0;
            break;
#endif
        default:
            return -EINVAL;
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_gpio_toggle(const struct gpio* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    uint8_t port = get_port(self->id);
    uint8_t mask = get_mask(self->id);
    switch (port)
    {
        case PORT_A:
            LATA ^= mask;
            break;
        case PORT_B:
            LATB ^= mask;
            break;
        case PORT_C:
            LATC ^= mask;
            break;
#if defined(PIC18F47K42) || defined(PIC18F57K42)
        case PORT_D:
            LATD ^= mask;
            break;
        case PORT_E:
            LATE ^= mask;
            break;
#endif
#if defined(PIC18F57K42)
        case PORT_F:
            LATF ^= mask;
            break;
#endif
        default:
            return -EINVAL;
    }
    return 0;
}

/* ========================================================================== */
