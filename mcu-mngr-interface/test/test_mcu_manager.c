#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/mcu_manager.h"
#include "../src/mcu_manager.c"

/* Mocked interfaces */
#include "mock_mcu_driver.h"

/* ============================================================================================== */

void helper_sys_mngr_init(mcu_manager_interface_t* sys_mngr)
{
    sys_mngr->interrupt_ctrl->disable    = mcu_mngr_disable_interrupts;
    sys_mngr->interrupt_ctrl->enable     = mcu_mngr_enable_interrupts;
    sys_mngr->timer0->set_timeout        = mcu_mngr_tmr0_set_timeout_ms;
    sys_mngr->timer0->reset_timeout      = mcu_mngr_tmr0_reset_timeout;
    sys_mngr->timer0->deactivate_timeout = mcu_mngr_tmr0_deactivate_timeout;
    sys_mngr->timer0->set_callback       = mcu_mngr_tmr0_set_timeout_callback;
    sys_mngr->led_0->on                  = mcu_mngr_led_turn_on;
    sys_mngr->led_0->off                 = mcu_mngr_led_turn_off;
    sys_mngr->led_0->toggle              = mcu_mngr_led_toggle;
    sys_mngr->led_1->on                  = mcu_mngr_led_turn_on;
    sys_mngr->led_1->off                 = mcu_mngr_led_turn_off;
    sys_mngr->led_1->toggle              = mcu_mngr_led_toggle;
    sys_mngr->gpio_0->set_state          = mcu_mngr_set_pin_state;
    sys_mngr->gpio_0->get_state          = mcu_mngr_get_pin_state;

    TEST_ASSERT_EQUAL_PTR(sys_mngr->interrupt_ctrl->disable, mcu_mngr_disable_interrupts);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->interrupt_ctrl->enable, mcu_mngr_enable_interrupts);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->timer0->set_callback, mcu_mngr_tmr0_set_timeout_callback);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->timer0->reset_timeout, mcu_mngr_tmr0_deactivate_timeout);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->timer0->set_timeout, mcu_mngr_tmr0_set_timeout_ms);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->gpio_0->set_state, mcu_mngr_set_pin_state);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->gpio_0->get_state, mcu_mngr_get_pin_state);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->led_0->on, mcu_mngr_led_turn_on);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->led_0->off, mcu_mngr_led_turn_off);
    TEST_ASSERT_EQUAL_PTR(sys_mngr->led_0->toggle, mcu_mngr_led_toggle);
}

/* ============================================================================================== */
