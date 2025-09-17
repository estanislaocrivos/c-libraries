#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

struct gpio;

struct gpio_ops
{
    int8_t (*set_state)(struct gpio* gpio, bool state);
    int8_t (*get_state)(struct gpio* gpio, bool* state);
    int8_t (*toggle)(struct gpio* gpio);
};

struct gpio
{
    uint8_t                id;
    const struct gpio_ops* ops;
};

#endif
