#include "logging.h"
#include "serial.h"
#include "string.h"
#include "unity.h"

#include <stdio.h>

/* ========================================================================== */

/* Test doubles for serial interface */
static uint8_t tx_buffer[256];
static size_t  tx_index;

static int8_t mock_transmit(
    const struct serial* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    for (size_t i = 0; i < size && tx_index < sizeof(tx_buffer); i++)
    {
        tx_buffer[tx_index++] = buffer[i];
    }
    printf("\nmock_transmit sent: ");
    for (size_t i = 0; i < size; i++)
    {
        printf("%c", buffer[i]);
    }
    return 0;
}

static const struct serial_ops mock_ops = {
    .transmit = mock_transmit,
};

static struct serial test_serial = {
    .ops             = &mock_ops,
    .baud_rate       = 9600,
    .data_bits       = 8,
    .parity          = 'N',
    .stop_bits       = 1,
    .was_initialized = true,
};

/* ========================================================================== */

void setUp(void)
{
    tx_index = 0;
    memset(tx_buffer, 0, sizeof(tx_buffer));
}

void tearDown(void)
{
}

/* ========================================================================== */

void test_example(void)
{
    struct log_config log_configuration = {
        .serial_output = &test_serial,
        .min_level     = LOG_LEVEL_INFO,
        .show_level    = true,
    };
    TEST_ASSERT_EQUAL(0, log_init(&log_configuration));
    TEST_ASSERT_NOT_NULL(log_configuration.serial_output);
    TEST_ASSERT_EQUAL(LOG_LEVEL_INFO, log_configuration.min_level);
    TEST_ASSERT_EQUAL(true, log_configuration.show_level);
    TEST_ASSERT_EQUAL(0, log_set_level(LOG_LEVEL_DEBUG));

    enum log_level current_level;
    TEST_ASSERT_EQUAL(0, log_get_level(&current_level));
    TEST_ASSERT_EQUAL(LOG_LEVEL_DEBUG, current_level);

    log_write(LOG_LEVEL_DEBUG, "(LOG_LEVEL_DEBUG) This is a debug message.");
    log_write(LOG_LEVEL_INFO, "(LOG_LEVEL_DEBUG) This is an info message.");
    log_write(LOG_LEVEL_WARN, "(LOG_LEVEL_DEBUG) This is a warning message.");
    log_write(LOG_LEVEL_ERROR, "(LOG_LEVEL_DEBUG) This is an error message.");
    log_write(LOG_LEVEL_FATAL, "(LOG_LEVEL_DEBUG) This is a fatal message.");

    log_debug("(LOG_LEVEL_DEBUG) This is a debug message.");
    log_info("(LOG_LEVEL_DEBUG) This is an info message.");
    log_warn("(LOG_LEVEL_DEBUG) This is a warning message.");
    log_error("(LOG_LEVEL_DEBUG) This is an error message.");
    log_fatal("(LOG_LEVEL_DEBUG) This is a fatal message.");

    TEST_ASSERT_EQUAL(0, log_set_level(LOG_LEVEL_INFO));
    TEST_ASSERT_EQUAL(0, log_get_level(&current_level));
    TEST_ASSERT_EQUAL(LOG_LEVEL_INFO, current_level);

    log_debug(
        "(LOG_LEVEL_INFO) This is a debug message. This should be filtered "
        "out.");
    log_info("(LOG_LEVEL_INFO) This is an info message.");
    log_warn("(LOG_LEVEL_INFO) This is a warning message.");
    log_error("(LOG_LEVEL_INFO) This is an error message.");
    log_fatal("(LOG_LEVEL_INFO) This is a fatal message.");

    TEST_ASSERT_EQUAL(0, log_set_level(LOG_LEVEL_WARN));
    TEST_ASSERT_EQUAL(0, log_get_level(&current_level));
    TEST_ASSERT_EQUAL(LOG_LEVEL_WARN, current_level);

    log_debug(
        "(LOG_LEVEL_WARN) This is a debug message. This should be filtered "
        "out.");
    log_info(
        "(LOG_LEVEL_WARN) This is an info message. This should be filtered "
        "out.");
    log_warn("(LOG_LEVEL_WARN) This is a warning message.");
    log_error("(LOG_LEVEL_WARN) This is an error message.");
    log_fatal("(LOG_LEVEL_WARN) This is a fatal message.");

    TEST_ASSERT_EQUAL(0, log_set_level(LOG_LEVEL_ERROR));
    TEST_ASSERT_EQUAL(0, log_get_level(&current_level));
    TEST_ASSERT_EQUAL(LOG_LEVEL_ERROR, current_level);

    log_debug(
        "(LOG_LEVEL_ERROR) This is a debug message. This should be filtered "
        "out.");
    log_info(
        "(LOG_LEVEL_ERROR) This is an info message. This should be filtered "
        "out.");
    log_warn(
        "(LOG_LEVEL_ERROR) This is a warning message. This should be filtered "
        "out.");
    log_error("(LOG_LEVEL_ERROR) This is an error message.");
    log_fatal("(LOG_LEVEL_ERROR) This is a fatal message.");

    TEST_ASSERT_EQUAL(0, log_set_level(LOG_LEVEL_FATAL));
    TEST_ASSERT_EQUAL(0, log_get_level(&current_level));
    TEST_ASSERT_EQUAL(LOG_LEVEL_FATAL, current_level);

    log_debug(
        "(LOG_LEVEL_FATAL) This is a debug message. This should be filtered "
        "out.");
    log_info(
        "(LOG_LEVEL_FATAL) This is an info message. This should be filtered "
        "out.");
    log_warn(
        "(LOG_LEVEL_FATAL) This is a warning message. This should be filtered "
        "out.");
    log_error(
        "(LOG_LEVEL_FATAL) This is an error message. This should be filtered "
        "out.");
    log_fatal("(LOG_LEVEL_FATAL) This is a fatal message.");

    TEST_ASSERT_EQUAL(0, log_set_level(LOG_LEVEL_NONE));
    TEST_ASSERT_EQUAL(0, log_get_level(&current_level));
    TEST_ASSERT_EQUAL(LOG_LEVEL_NONE, current_level);

    log_debug(
        "(LOG_LEVEL_NONE) This is a debug message. This should be filtered "
        "out.");
    log_info(
        "(LOG_LEVEL_NONE) This is an info message. This should be filtered "
        "out.");
    log_warn(
        "(LOG_LEVEL_NONE) This is a warning message. This should be filtered "
        "out.");
    log_error(
        "(LOG_LEVEL_NONE) This is an error message. This should be filtered "
        "out.");
    log_fatal(
        "(LOG_LEVEL_NONE) This is a fatal message. This should be filtered "
        "out.");

    TEST_ASSERT_EQUAL(0, log_set_level(LOG_LEVEL_DEBUG));
    TEST_ASSERT_EQUAL(0, log_get_level(&current_level));
    TEST_ASSERT_EQUAL(LOG_LEVEL_DEBUG, current_level);

    int32_t test_variable = 42;
    log_value(
        "(LOG_LEVEL_DEBUG) Value of test_variable: %d",
        &test_variable,
        LOG_VALUE_INT32);
}

/* ========================================================================== */
