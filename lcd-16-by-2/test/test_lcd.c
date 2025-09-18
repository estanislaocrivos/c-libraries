#include "../../test/support/unity.h"

/* ============================================================================================== */

#include <string.h>

#include "../inc/lcd.h"
#include "../src/lcd.c"

/* Mocked interfaces */
#include "mock_mcu_driver.h"

/* ============================================================================================== */

void helper_lcd_init(const struct gpio_ops*    gpio_ops,
                     const struct timer_ops*   timer_ops,
                     struct lcd*               lcd,
                     struct lcd_bus_interface* lcd_bus,
                     struct timer*             lcd_delay)
{
    lcd_bus->rs.ops = gpio_ops;
    lcd_bus->en.ops = gpio_ops;
    lcd_bus->d0.ops = gpio_ops;
    lcd_bus->d1.ops = gpio_ops;
    lcd_bus->d2.ops = gpio_ops;
    lcd_bus->d3.ops = gpio_ops;
    lcd_bus->d4.ops = gpio_ops;
    lcd_bus->d5.ops = gpio_ops;
    lcd_bus->d6.ops = gpio_ops;
    lcd_bus->d7.ops = gpio_ops;
    lcd_delay->ops  = timer_ops;
    lcd->_bus       = lcd_bus;
    lcd->_delay     = lcd_delay;
}

/* ============================================================================================== */

void test_lcd_creation(void)
{
    const struct gpio_ops gpio_ops
        = {.get_state = gpio_get_state, .set_state = gpio_set_state, .toggle = gpio_toggle};
    const struct timer_ops   timer_ops = {.deactivate_timeout   = tmr_deactivate_timeout,
                                          .delay_ms             = tmr_delay_ms,
                                          .delay_us             = tmr_delay_us,
                                          .reset_timeout        = tmr_reset_timeout,
                                          .set_timeout_callback = tmr_set_timeout_callback,
                                          .set_timeout_ms       = tmr_set_timeout_ms};
    struct lcd_bus_interface lcd_bus;
    struct timer             lcd_delay;
    struct lcd               lcd;
    helper_lcd_init(&gpio_ops, &timer_ops, &lcd, &lcd_bus, &lcd_delay);

    lcd_create(&lcd);

    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->rs.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->en.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d4.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d5.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d6.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d7.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d0.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d1.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d2.ops);
    TEST_ASSERT_EQUAL_PTR(&gpio_ops, lcd._bus->d3.ops);
    TEST_ASSERT_EQUAL_PTR(&timer_ops, lcd._delay->ops);
}

/* ============================================================================================== */

void test_lcd_command_assert_seq(void)
{
    const struct gpio_ops gpio_ops
        = {.get_state = gpio_get_state, .set_state = gpio_set_state, .toggle = gpio_toggle};
    const struct timer_ops   timer_ops = {.deactivate_timeout   = tmr_deactivate_timeout,
                                          .delay_ms             = tmr_delay_ms,
                                          .delay_us             = tmr_delay_us,
                                          .reset_timeout        = tmr_reset_timeout,
                                          .set_timeout_callback = tmr_set_timeout_callback,
                                          .set_timeout_ms       = tmr_set_timeout_ms};
    struct lcd_bus_interface lcd_bus;
    struct timer             lcd_delay;
    struct lcd               lcd;
    helper_lcd_init(&gpio_ops, &timer_ops, &lcd, &lcd_bus, &lcd_delay);

    lcd_create(&lcd);

    gpio_set_state_ExpectAndReturn(&lcd._bus->rs, false, 0);
    gpio_set_state_ExpectAndReturn(&lcd_bus.en, true, 0);
    tmr_delay_ms_ExpectAndReturn(&lcd_delay, 10, 0);
    gpio_set_state_ExpectAndReturn(&lcd_bus.en, false, 0);
    _lcd_command_assert_seq(&lcd);
}

/* ============================================================================================== */

void test_lcd_data_assert_seq(void)
{
    const struct gpio_ops gpio_ops
        = {.get_state = gpio_get_state, .set_state = gpio_set_state, .toggle = gpio_toggle};
    const struct timer_ops   timer_ops = {.deactivate_timeout   = tmr_deactivate_timeout,
                                          .delay_ms             = tmr_delay_ms,
                                          .delay_us             = tmr_delay_us,
                                          .reset_timeout        = tmr_reset_timeout,
                                          .set_timeout_callback = tmr_set_timeout_callback,
                                          .set_timeout_ms       = tmr_set_timeout_ms};
    struct lcd_bus_interface lcd_bus;
    struct timer             lcd_delay;
    struct lcd               lcd;
    helper_lcd_init(&gpio_ops, &timer_ops, &lcd, &lcd_bus, &lcd_delay);

    lcd_create(&lcd);

    gpio_set_state_ExpectAndReturn(&lcd._bus->rs, true, 0);
    gpio_set_state_ExpectAndReturn(&lcd_bus.en, true, 0);
    tmr_delay_ms_ExpectAndReturn(&lcd_delay, 10, 0);
    gpio_set_state_ExpectAndReturn(&lcd_bus.en, false, 0);
    _lcd_data_assert_seq(&lcd);
}

/* ============================================================================================== */

void test_lcd_send_nibble(void)
{
    const struct gpio_ops gpio_ops
        = {.get_state = gpio_get_state, .set_state = gpio_set_state, .toggle = gpio_toggle};
    const struct timer_ops   timer_ops = {.deactivate_timeout   = tmr_deactivate_timeout,
                                          .delay_ms             = tmr_delay_ms,
                                          .delay_us             = tmr_delay_us,
                                          .reset_timeout        = tmr_reset_timeout,
                                          .set_timeout_callback = tmr_set_timeout_callback,
                                          .set_timeout_ms       = tmr_set_timeout_ms};
    struct lcd_bus_interface lcd_bus;
    struct timer             lcd_delay;
    struct lcd               lcd;
    helper_lcd_init(&gpio_ops, &timer_ops, &lcd, &lcd_bus, &lcd_delay);

    lcd_create(&lcd);

    gpio_set_state_ExpectAndReturn(&lcd._bus->d4, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d5, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d6, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d7, 0, 0);
    _lcd_send_nibble(&lcd, 0x05);
}

/* ============================================================================================== */

void test_lcd_send_command(void)
{
    const struct gpio_ops gpio_ops
        = {.get_state = gpio_get_state, .set_state = gpio_set_state, .toggle = gpio_toggle};
    const struct timer_ops   timer_ops = {.deactivate_timeout   = tmr_deactivate_timeout,
                                          .delay_ms             = tmr_delay_ms,
                                          .delay_us             = tmr_delay_us,
                                          .reset_timeout        = tmr_reset_timeout,
                                          .set_timeout_callback = tmr_set_timeout_callback,
                                          .set_timeout_ms       = tmr_set_timeout_ms};
    struct lcd_bus_interface lcd_bus;
    struct timer             lcd_delay;
    struct lcd               lcd;
    helper_lcd_init(&gpio_ops, &timer_ops, &lcd, &lcd_bus, &lcd_delay);

    lcd_create(&lcd);

    gpio_set_state_ExpectAndReturn(&lcd._bus->d4, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d5, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d6, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d7, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->rs, false, 0);
    gpio_set_state_ExpectAndReturn(&lcd_bus.en, true, 0);
    tmr_delay_ms_ExpectAndReturn(lcd._delay, 10, 0);
    gpio_set_state_ExpectAndReturn(&lcd_bus.en, false, 0);

    gpio_set_state_ExpectAndReturn(&lcd._bus->d4, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d5, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d6, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d7, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->rs, false, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->en, true, 0);
    tmr_delay_ms_ExpectAndReturn(lcd._delay, 10, 0);
    gpio_set_state_ExpectAndReturn(&lcd_bus.en, false, 0);

    _lcd_send_command(&lcd, 0x5F);
}

/* ============================================================================================== */

void test_lcd_send_char(void)
{
    const struct gpio_ops gpio_ops
        = {.get_state = gpio_get_state, .set_state = gpio_set_state, .toggle = gpio_toggle};
    const struct timer_ops   timer_ops = {.deactivate_timeout   = tmr_deactivate_timeout,
                                          .delay_ms             = tmr_delay_ms,
                                          .delay_us             = tmr_delay_us,
                                          .reset_timeout        = tmr_reset_timeout,
                                          .set_timeout_callback = tmr_set_timeout_callback,
                                          .set_timeout_ms       = tmr_set_timeout_ms};
    struct lcd_bus_interface lcd_bus;
    struct timer             lcd_delay;
    struct lcd               lcd;
    helper_lcd_init(&gpio_ops, &timer_ops, &lcd, &lcd_bus, &lcd_delay);

    gpio_set_state_ExpectAndReturn(&lcd._bus->d4, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d5, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d6, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d7, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->rs, true, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->en, true, 0);
    tmr_delay_ms_ExpectAndReturn(&lcd_delay, 10, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->en, false, 0);

    gpio_set_state_ExpectAndReturn(&lcd._bus->d4, 1, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d5, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d6, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->d7, 0, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->rs, true, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->en, true, 0);
    tmr_delay_ms_ExpectAndReturn(&lcd_delay, 10, 0);
    gpio_set_state_ExpectAndReturn(&lcd._bus->en, false, 0);

    _lcd_send_char(&lcd, 'A');
}

/* ============================================================================================== */
