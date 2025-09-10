#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/lcd.h"
#include "../src/lcd.c"

/* Mocked interfaces */
#include "mock_mcu_driver.h"

/* ============================================================================================== */

static delay_interface_t fake_delay;
static gpio_interface_t  fake_gpio_0;
static gpio_interface_t  fake_gpio_1;
static gpio_interface_t  fake_gpio_2;
static gpio_interface_t  fake_gpio_3;
static gpio_interface_t  fake_gpio_4;
static gpio_interface_t  fake_gpio_5;

void helper_mcu_mngr_init(mcu_manager_interface_t* mcu_mngr)
{
    mcu_mngr->delay  = &fake_delay;
    mcu_mngr->gpio_0 = &fake_gpio_0;
    mcu_mngr->gpio_1 = &fake_gpio_1;
    mcu_mngr->gpio_2 = &fake_gpio_2;
    mcu_mngr->gpio_3 = &fake_gpio_3;
    mcu_mngr->gpio_4 = &fake_gpio_4;
    mcu_mngr->gpio_5 = &fake_gpio_5;

    mcu_mngr->gpio_0->set_high  = gpio0_set_high;
    mcu_mngr->gpio_0->set_low   = gpio0_set_low;
    mcu_mngr->gpio_0->set_state = gpio0_set_pin_state;
    mcu_mngr->gpio_0->get_state = gpio0_get_pin_state;

    mcu_mngr->gpio_1->set_high  = gpio1_set_high;
    mcu_mngr->gpio_1->set_low   = gpio1_set_low;
    mcu_mngr->gpio_1->set_state = gpio1_set_pin_state;
    mcu_mngr->gpio_1->get_state = gpio1_get_pin_state;

    mcu_mngr->gpio_2->set_high  = gpio2_set_high;
    mcu_mngr->gpio_2->set_low   = gpio2_set_low;
    mcu_mngr->gpio_2->set_state = gpio2_set_pin_state;
    mcu_mngr->gpio_2->get_state = gpio2_get_pin_state;

    mcu_mngr->gpio_3->set_high  = gpio3_set_high;
    mcu_mngr->gpio_3->set_low   = gpio3_set_low;
    mcu_mngr->gpio_3->set_state = gpio3_set_pin_state;
    mcu_mngr->gpio_3->get_state = gpio3_get_pin_state;

    mcu_mngr->gpio_4->set_high  = gpio4_set_high;
    mcu_mngr->gpio_4->set_low   = gpio4_set_low;
    mcu_mngr->gpio_4->set_state = gpio4_set_pin_state;
    mcu_mngr->gpio_4->get_state = gpio4_get_pin_state;

    mcu_mngr->gpio_5->set_high  = gpio5_set_high;
    mcu_mngr->gpio_5->set_low   = gpio5_set_low;
    mcu_mngr->gpio_5->set_state = gpio5_set_pin_state;
    mcu_mngr->gpio_5->get_state = gpio5_get_pin_state;

    mcu_mngr->delay->set_ms = delay_ms;
    mcu_mngr->delay->set_us = delay_us;
}

/* ============================================================================================== */

void test_lcd_creation(void)
{
    mcu_manager_interface_t mcu_mngr = {0};
    helper_mcu_mngr_init(&mcu_mngr);

    lcd_t lcd;
    TEST_ASSERT_EQUAL(0, lcd_create(&lcd, &mcu_mngr));

    TEST_ASSERT_EQUAL_PTR(gpio0_set_high, lcd.rs->set_high);
    TEST_ASSERT_EQUAL_PTR(gpio1_set_high, lcd.en->set_high);
    TEST_ASSERT_EQUAL_PTR(gpio2_set_high, lcd.d4->set_high);
    TEST_ASSERT_EQUAL_PTR(gpio3_set_high, lcd.d5->set_high);
    TEST_ASSERT_EQUAL_PTR(gpio4_set_high, lcd.d6->set_high);
    TEST_ASSERT_EQUAL_PTR(gpio5_set_high, lcd.d7->set_high);

    TEST_ASSERT_EQUAL_PTR(gpio0_set_low, lcd.rs->set_low);
    TEST_ASSERT_EQUAL_PTR(gpio1_set_low, lcd.en->set_low);
    TEST_ASSERT_EQUAL_PTR(gpio2_set_low, lcd.d4->set_low);
    TEST_ASSERT_EQUAL_PTR(gpio3_set_low, lcd.d5->set_low);
    TEST_ASSERT_EQUAL_PTR(gpio4_set_low, lcd.d6->set_low);
    TEST_ASSERT_EQUAL_PTR(gpio5_set_low, lcd.d7->set_low);

    TEST_ASSERT_EQUAL_PTR(gpio0_set_pin_state, lcd.rs->set_state);
    TEST_ASSERT_EQUAL_PTR(gpio1_set_pin_state, lcd.en->set_state);
    TEST_ASSERT_EQUAL_PTR(gpio2_set_pin_state, lcd.d4->set_state);
    TEST_ASSERT_EQUAL_PTR(gpio3_set_pin_state, lcd.d5->set_state);
    TEST_ASSERT_EQUAL_PTR(gpio4_set_pin_state, lcd.d6->set_state);
    TEST_ASSERT_EQUAL_PTR(gpio5_set_pin_state, lcd.d7->set_state);

    TEST_ASSERT_EQUAL_PTR(gpio0_get_pin_state, lcd.rs->get_state);
    TEST_ASSERT_EQUAL_PTR(gpio1_get_pin_state, lcd.en->get_state);
    TEST_ASSERT_EQUAL_PTR(gpio2_get_pin_state, lcd.d4->get_state);
    TEST_ASSERT_EQUAL_PTR(gpio3_get_pin_state, lcd.d5->get_state);
    TEST_ASSERT_EQUAL_PTR(gpio4_get_pin_state, lcd.d6->get_state);
    TEST_ASSERT_EQUAL_PTR(gpio5_get_pin_state, lcd.d7->get_state);

    TEST_ASSERT_EQUAL_PTR(delay_ms, lcd.delay->set_ms);
    TEST_ASSERT_EQUAL_PTR(delay_us, lcd.delay->set_us);
}

/* ============================================================================================== */

void test_lcd_command_assert_seq(void)
{
    mcu_manager_interface_t mcu_mngr = {0};
    helper_mcu_mngr_init(&mcu_mngr);

    lcd_t lcd;
    TEST_ASSERT_EQUAL(0, lcd_create(&lcd, &mcu_mngr));

    gpio0_set_low_Expect();
    gpio1_set_high_Expect();
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_low_Expect();
    _lcd_command_assert_seq(&lcd);
}

/* ============================================================================================== */

void test_lcd_data_assert_seq(void)
{
    mcu_manager_interface_t mcu_mngr = {0};
    helper_mcu_mngr_init(&mcu_mngr);

    lcd_t lcd;
    TEST_ASSERT_EQUAL(0, lcd_create(&lcd, &mcu_mngr));

    gpio0_set_high_Expect();
    gpio1_set_high_Expect();
    delay_ms_ExpectAndReturn(10, 0);
    gpio1_set_low_Expect();
    _lcd_data_assert_seq(&lcd);
}

/* ============================================================================================== */
