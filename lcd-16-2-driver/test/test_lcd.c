#include "../../test/support/unity.h"

/* ============================================================================================== */

#include <string.h>

#include "../inc/lcd.h"
#include "../src/lcd.c"

/* Mocked interfaces */
#include "mock_mcu_driver.h"

/* ============================================================================================== */

void helper_lcd_init(lcd_bus_interface_t* lcd_bus,
                     delay_interface_t*   lcd_delay,
                     lcd_interface_t*     lcd_iface)
{
    memset(lcd_bus, 0, sizeof(lcd_bus_interface_t));
    memset(lcd_delay, 0, sizeof(delay_interface_t));
    memset(lcd_iface, 0, sizeof(lcd_interface_t));
    lcd_bus->rs.set_state = gpio0_set_state;
    lcd_bus->rs.get_state = gpio0_get_state;
    lcd_bus->en.set_state = gpio1_set_state;
    lcd_bus->en.get_state = gpio1_get_state;
    lcd_bus->d0.set_state = gpio6_set_state;
    lcd_bus->d0.get_state = gpio6_get_state;
    lcd_bus->d1.set_state = gpio7_set_state;
    lcd_bus->d1.get_state = gpio7_get_state;
    lcd_bus->d2.set_state = gpio8_set_state;
    lcd_bus->d2.get_state = gpio8_get_state;
    lcd_bus->d3.set_state = gpio9_set_state;
    lcd_bus->d3.get_state = gpio9_get_state;
    lcd_bus->d4.set_state = gpio2_set_state;
    lcd_bus->d4.get_state = gpio2_get_state;
    lcd_bus->d5.set_state = gpio3_set_state;
    lcd_bus->d5.get_state = gpio3_get_state;
    lcd_bus->d6.set_state = gpio4_set_state;
    lcd_bus->d6.get_state = gpio4_get_state;
    lcd_bus->d7.set_state = gpio5_set_state;
    lcd_bus->d7.get_state = gpio5_get_state;
    lcd_delay->set_ms     = delay_ms;
    lcd_delay->set_us     = delay_us;

    lcd_iface->bus   = lcd_bus;
    lcd_iface->delay = lcd_delay;
}

/* ============================================================================================== */

void test_lcd_creation(void)
{
    lcd_bus_interface_t lcd_bus;
    delay_interface_t   lcd_delay;
    lcd_interface_t     lcd_iface;
    helper_lcd_init(&lcd_bus, &lcd_delay, &lcd_iface);

    lcd_t lcd;
    lcd_create(&lcd, &lcd_iface, false);

    TEST_ASSERT_EQUAL_PTR(gpio0_set_state, lcd.iface->bus->rs.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio1_set_state, lcd.iface->bus->en.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio2_set_state, lcd.iface->bus->d4.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio3_set_state, lcd.iface->bus->d5.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio4_set_state, lcd.iface->bus->d6.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio5_set_state, lcd.iface->bus->d7.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio6_set_state, lcd.iface->bus->d0.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio7_set_state, lcd.iface->bus->d1.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio8_set_state, lcd.iface->bus->d2.set_state);
    TEST_ASSERT_EQUAL_PTR(gpio9_set_state, lcd.iface->bus->d3.set_state);

    TEST_ASSERT_EQUAL_PTR(gpio0_get_state, lcd.iface->bus->rs.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio1_get_state, lcd.iface->bus->en.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio2_get_state, lcd.iface->bus->d4.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio3_get_state, lcd.iface->bus->d5.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio4_get_state, lcd.iface->bus->d6.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio5_get_state, lcd.iface->bus->d7.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio6_get_state, lcd.iface->bus->d0.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio7_get_state, lcd.iface->bus->d1.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio8_get_state, lcd.iface->bus->d2.get_state);
    TEST_ASSERT_EQUAL_PTR(gpio9_get_state, lcd.iface->bus->d3.get_state);
    TEST_ASSERT_EQUAL_PTR(delay_ms, lcd.iface->delay->set_ms);
    TEST_ASSERT_EQUAL_PTR(delay_us, lcd.iface->delay->set_us);
}

/* ============================================================================================== */

void test_lcd_command_assert_seq(void)
{
    lcd_bus_interface_t lcd_bus;
    delay_interface_t   lcd_delay;
    lcd_interface_t     lcd_iface;
    helper_lcd_init(&lcd_bus, &lcd_delay, &lcd_iface);

    lcd_t lcd;
    lcd_create(&lcd, &lcd_iface, false);

    gpio0_set_state_Expect(false);
    gpio1_set_state_Expect(true);
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_state_Expect(false);
    _lcd_command_assert_seq(&lcd);
}

/* ============================================================================================== */

void test_lcd_data_assert_seq(void)
{
    lcd_bus_interface_t lcd_bus;
    delay_interface_t   lcd_delay;
    lcd_interface_t     lcd_iface;
    helper_lcd_init(&lcd_bus, &lcd_delay, &lcd_iface);

    lcd_t lcd;
    lcd_create(&lcd, &lcd_iface, false);

    gpio0_set_state_Expect(true);
    gpio1_set_state_Expect(true);
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_state_Expect(false);
    _lcd_data_assert_seq(&lcd);
}

/* ============================================================================================== */

void test_lcd_send_nibble(void)
{
    lcd_bus_interface_t lcd_bus;
    delay_interface_t   lcd_delay;
    lcd_interface_t     lcd_iface;
    helper_lcd_init(&lcd_bus, &lcd_delay, &lcd_iface);

    lcd_t lcd;
    lcd_create(&lcd, &lcd_iface, false);

    gpio2_set_state_Expect(1);
    gpio3_set_state_Expect(0);
    gpio4_set_state_Expect(1);
    gpio5_set_state_Expect(0);
    _lcd_send_nibble(&lcd, 0x05);
}

/* ============================================================================================== */

void test_lcd_send_command(void)
{
    lcd_bus_interface_t lcd_bus;
    delay_interface_t   lcd_delay;
    lcd_interface_t     lcd_iface;
    helper_lcd_init(&lcd_bus, &lcd_delay, &lcd_iface);

    lcd_t lcd;
    lcd_create(&lcd, &lcd_iface, false);

    gpio2_set_state_Expect(1);
    gpio3_set_state_Expect(0);
    gpio4_set_state_Expect(1);
    gpio5_set_state_Expect(0);
    gpio0_set_state_Expect(false);
    gpio1_set_state_Expect(true);
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_state_Expect(false);

    gpio2_set_state_Expect(1);
    gpio3_set_state_Expect(1);
    gpio4_set_state_Expect(1);
    gpio5_set_state_Expect(1);
    gpio0_set_state_Expect(false);
    gpio1_set_state_Expect(true);
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_state_Expect(false);

    _lcd_send_command(&lcd, 0x5F);
}

/* ============================================================================================== */

void test_lcd_send_char(void)
{
    lcd_bus_interface_t lcd_bus;
    delay_interface_t   lcd_delay;
    lcd_interface_t     lcd_iface;
    helper_lcd_init(&lcd_bus, &lcd_delay, &lcd_iface);

    lcd_t lcd;
    lcd_create(&lcd, &lcd_iface, false);

    gpio2_set_state_Expect(0);
    gpio3_set_state_Expect(0);
    gpio4_set_state_Expect(1);
    gpio5_set_state_Expect(0);
    gpio0_set_state_Expect(true);
    gpio1_set_state_Expect(true);
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_state_Expect(false);

    gpio2_set_state_Expect(1);
    gpio3_set_state_Expect(0);
    gpio4_set_state_Expect(0);
    gpio5_set_state_Expect(0);
    gpio0_set_state_Expect(true);
    gpio1_set_state_Expect(true);
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_state_Expect(false);

    _lcd_send_char(&lcd, 'A');
}

/* ============================================================================================== */
