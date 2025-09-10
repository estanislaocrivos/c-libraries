#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/mcu_manager.h"
#include "../src/mcu_manager.c"

/* Mocked interfaces */
#include "mock_mcu_driver.h"

/* ============================================================================================== */

void helper_mcu_mngr_init(mcu_manager_interface_t* mcu_mngr)
{
    mcu_mngr->interrupt_ctrl->disable    = mcu_mngr_disable_interrupts;
    mcu_mngr->interrupt_ctrl->enable     = mcu_mngr_enable_interrupts;
    mcu_mngr->timer0->set_timeout        = mcu_mngr_tmr0_set_timeout_ms;
    mcu_mngr->timer0->reset_timeout      = mcu_mngr_tmr0_reset_timeout;
    mcu_mngr->timer0->deactivate_timeout = mcu_mngr_tmr0_deactivate_timeout;
    mcu_mngr->timer0->set_callback       = mcu_mngr_tmr0_set_timeout_callback;
    mcu_mngr->led_0->on                  = mcu_mngr_led_turn_on;
    mcu_mngr->led_0->off                 = mcu_mngr_led_turn_off;
    mcu_mngr->led_0->toggle              = mcu_mngr_led_toggle;
    mcu_mngr->gpio_0->set_state          = mcu_mngr_set_pin_state;
    mcu_mngr->gpio_0->get_state          = mcu_mngr_get_pin_state;
}

/* ============================================================================================== */

void test_mcu_mngr_creation(void)
{
    mcu_manager_interface_t mcu_mngr = {0};
    helper_mcu_mngr_init(&mcu_mngr);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_disable_interrupts, mcu_mngr.interrupt_ctrl->disable);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_enable_interrupts, mcu_mngr.interrupt_ctrl->enable);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_tmr0_set_timeout_ms, mcu_mngr.timer0->set_timeout);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_tmr0_reset_timeout, mcu_mngr.timer0->reset_timeout);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_tmr0_deactivate_timeout, mcu_mngr.timer0->deactivate_timeout);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_tmr0_set_timeout_callback, mcu_mngr.timer0->set_callback);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_led_turn_on, mcu_mngr.led_0->on);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_led_turn_off, mcu_mngr.led_0->off);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_led_toggle, mcu_mngr.led_0->toggle);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_set_pin_state, mcu_mngr.gpio_0->set_state);
    TEST_ASSERT_EQUAL_PTR(mcu_mngr_get_pin_state, mcu_mngr.gpio_0->get_state);
}

/* ============================================================================================== */
