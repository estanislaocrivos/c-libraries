#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/mcu_manager.h"
#include "../src/mcu_manager.c"

/* Mocked interfaces */
#include "mock_mcu_driver.h"

/* ============================================================================================== */

void helper_sys_mngr_init(sys_mngr_class_t* sys_mngr)
{
    sys_mngr->initialize                = mcu_mngr_initialize;
    sys_mngr->disable_interrupts        = mcu_mngr_disable_interrupts;
    sys_mngr->enable_interrupts         = mcu_mngr_enable_interrupts;
    sys_mngr->tmr0_set_timeout          = mcu_mngr_tmr0_set_timeout_ms;
    sys_mngr->tmr0_deact_timeout        = mcu_mngr_tmr0_deactivate_timeout;
    sys_mngr->tmr0_set_timeout_callback = mcu_mngr_tmr0_set_timeout_callback;
    sys_mngr->tmr2_set_timeout          = mcu_mngr_tmr2_set_timeout_ms;
    sys_mngr->tmr2_deact_timeout        = mcu_mngr_tmr2_deactivate_timeout;
    sys_mngr->tmr2_set_timeout_callback = mcu_mngr_tmr2_set_timeout_callback;
    sys_mngr->tmr4_set_timeout          = mcu_mngr_tmr4_set_timeout_ms;
    sys_mngr->tmr4_deact_timeout        = mcu_mngr_tmr4_deactivate_timeout;
    sys_mngr->tmr4_set_timeout_callback = mcu_mngr_tmr4_set_timeout_callback;
    sys_mngr->tmr6_set_timeout          = mcu_mngr_tmr6_set_timeout_ms;
    sys_mngr->tmr6_reset_timeout        = mcu_mngr_tmr6_reset_timeout;
    sys_mngr->tmr6_set_timeout_callback = mcu_mngr_tmr6_set_timeout_callback;
    sys_mngr->set_pin_state             = mcu_mngr_set_pin_state;
    sys_mngr->get_pin_state             = mcu_mngr_get_pin_state;
    sys_mngr->comm_led_high             = mcu_mngr_comms_led_turn_on;
    sys_mngr->comm_led_low              = mcu_mngr_comms_led_turn_off;
    sys_mngr->stat_led_toggle           = mcu_mngr_stat_led_toggle;
    sys_mngr->stat_led_high             = mcu_mngr_stat_led_turn_on;
    sys_mngr->stat_led_low              = mcu_mngr_stat_led_turn_off;
}

/* ============================================================================================== */

void test_system_manager_init(void)
{
    sys_mngr_class_t sys_mngr;
    helper_sys_mngr_init(&sys_mngr);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.initialize, mcu_mngr_initialize);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.disable_interrupts, mcu_mngr_disable_interrupts);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.enable_interrupts, mcu_mngr_enable_interrupts);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr0_set_timeout_callback, mcu_mngr_tmr0_set_timeout_callback);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr0_deact_timeout, mcu_mngr_tmr0_deactivate_timeout);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr0_set_timeout, mcu_mngr_tmr0_set_timeout_ms);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr2_set_timeout_callback, mcu_mngr_tmr2_set_timeout_callback);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr2_deact_timeout, mcu_mngr_tmr2_deactivate_timeout);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr2_set_timeout, mcu_mngr_tmr2_set_timeout_ms);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr4_set_timeout_callback, mcu_mngr_tmr4_set_timeout_callback);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr4_deact_timeout, mcu_mngr_tmr4_deactivate_timeout);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.tmr4_set_timeout, mcu_mngr_tmr4_set_timeout_ms);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.set_pin_state, mcu_mngr_set_pin_state);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.get_pin_state, mcu_mngr_get_pin_state);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.comm_led_high, mcu_mngr_comms_led_turn_on);
    TEST_ASSERT_EQUAL_PTR(sys_mngr.stat_led_toggle, mcu_mngr_stat_led_toggle);
}

/* ============================================================================================== */

void test_tmr_callback(void)
{
}

void test_system_manager_calls(void)
{
    sys_mngr_class_t sys_mngr;
    helper_sys_mngr_init(&sys_mngr);

    mcu_mngr_initialize_Expect();
    sys_mngr.initialize();

    mcu_mngr_disable_interrupts_Expect();
    sys_mngr.disable_interrupts();

    mcu_mngr_enable_interrupts_Expect();
    sys_mngr.enable_interrupts();

    mcu_mngr_tmr0_set_timeout_ms_ExpectAndReturn(1000, 0);
    sys_mngr.tmr0_set_timeout(1000);

    mcu_mngr_tmr0_deactivate_timeout_Expect();
    sys_mngr.tmr0_deact_timeout();

    mcu_mngr_tmr0_set_timeout_callback_ExpectAndReturn(test_tmr_callback, 0);
    sys_mngr.tmr0_set_timeout_callback(test_tmr_callback);

    mcu_mngr_tmr2_set_timeout_ms_ExpectAndReturn(2000, 0);
    sys_mngr.tmr2_set_timeout(2000);

    mcu_mngr_tmr2_deactivate_timeout_Expect();
    sys_mngr.tmr2_deact_timeout();

    mcu_mngr_tmr2_set_timeout_callback_ExpectAndReturn(test_tmr_callback, 0);
    sys_mngr.tmr2_set_timeout_callback(test_tmr_callback);

    mcu_mngr_tmr4_set_timeout_ms_ExpectAndReturn(4000, 0);
    sys_mngr.tmr4_set_timeout(4000);

    mcu_mngr_tmr4_deactivate_timeout_Expect();
    sys_mngr.tmr4_deact_timeout();

    mcu_mngr_tmr4_set_timeout_callback_ExpectAndReturn(test_tmr_callback, 0);
    sys_mngr.tmr4_set_timeout_callback(test_tmr_callback);

    mcu_mngr_set_pin_state_ExpectAndReturn(1, true, 0);
    sys_mngr.set_pin_state(1, true);

    bool pin;
    mcu_mngr_get_pin_state_ExpectAndReturn(1, &pin, 0);
    sys_mngr.get_pin_state(1, &pin);

    mcu_mngr_comms_led_turn_on_Expect();
    sys_mngr.comm_led_high();

    mcu_mngr_stat_led_toggle_Expect();
    sys_mngr.stat_led_toggle();
}

/* ============================================================================================== */
