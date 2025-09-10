#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/mcu_manager.h"
#include "../src/mcu_manager.c"

/* Mocked interfaces */
#include "mock_mcu_driver.h"

/* ============================================================================================== */

static interrupt_ctrl_interface_t fake_interrupt_ctrl;
static timer_interface_t          fake_timer0;
static led_interface_t            fake_led_0;
static gpio_interface_t           fake_gpio_0;

void helper_mcu_mngr_init(mcu_manager_interface_t* mcu_mngr)
{
    mcu_mngr->interrupt_ctrl = &fake_interrupt_ctrl;
    mcu_mngr->timer0         = &fake_timer0;
    mcu_mngr->led_0          = &fake_led_0;
    mcu_mngr->gpio_0         = &fake_gpio_0;

    mcu_mngr->interrupt_ctrl->disable    = disable_interrupts;
    mcu_mngr->interrupt_ctrl->enable     = enable_interrupts;
    mcu_mngr->timer0->set_timeout        = tmr0_set_timeout_ms;
    mcu_mngr->timer0->reset_timeout      = tmr0_reset_timeout;
    mcu_mngr->timer0->deactivate_timeout = tmr0_deactivate_timeout;
    mcu_mngr->timer0->set_callback       = tmr0_set_timeout_callback;
    mcu_mngr->led_0->on                  = led_turn_on;
    mcu_mngr->led_0->off                 = led_turn_off;
    mcu_mngr->led_0->toggle              = led_toggle;
    mcu_mngr->gpio_0->set_state          = gpio0_set_pin_state;
    mcu_mngr->gpio_0->get_state          = gpio0_get_pin_state;
}

/* ============================================================================================== */

void test_mcu_mngr_creation(void)
{
    mcu_manager_interface_t mcu_mngr = {0};
    helper_mcu_mngr_init(&mcu_mngr);
    TEST_ASSERT_EQUAL_PTR(disable_interrupts, mcu_mngr.interrupt_ctrl->disable);
    TEST_ASSERT_EQUAL_PTR(enable_interrupts, mcu_mngr.interrupt_ctrl->enable);
    TEST_ASSERT_EQUAL_PTR(tmr0_set_timeout_ms, mcu_mngr.timer0->set_timeout);
    TEST_ASSERT_EQUAL_PTR(tmr0_reset_timeout, mcu_mngr.timer0->reset_timeout);
    TEST_ASSERT_EQUAL_PTR(tmr0_deactivate_timeout, mcu_mngr.timer0->deactivate_timeout);
    TEST_ASSERT_EQUAL_PTR(tmr0_set_timeout_callback, mcu_mngr.timer0->set_callback);
    TEST_ASSERT_EQUAL_PTR(led_turn_on, mcu_mngr.led_0->on);
    TEST_ASSERT_EQUAL_PTR(led_turn_off, mcu_mngr.led_0->off);
    TEST_ASSERT_EQUAL_PTR(led_toggle, mcu_mngr.led_0->toggle);
    TEST_ASSERT_EQUAL_PTR(gpio0_set_pin_state, mcu_mngr.gpio_0->set_state);
    TEST_ASSERT_EQUAL_PTR(gpio0_get_pin_state, mcu_mngr.gpio_0->get_state);
}

/* ============================================================================================== */
