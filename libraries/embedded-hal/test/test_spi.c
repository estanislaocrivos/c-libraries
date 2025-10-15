#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/spi.h"

/* Mocked interfaces */
#include "mock_spi_driver.h"

/* ============================================================================================== */

void helper_spi_create(struct spi_ops* ops, struct spi_port* spi)
{
    ops->initialize          = spi1_initialize;
    ops->transmit            = spi1_transmit;
    ops->receive             = spi1_receive;
    ops->enable_rx_interrupt = spi1_enable_rx_interrupt;
    ops->set_rx_callback     = spi1_set_rx_callback;
    ops->flush_tx            = spi1_flush_tx;
    ops->flush_rx            = spi1_flush_rx;
    spi->ops                 = ops;
    spi->port_id             = 0;
    spi->frequency           = 115200;
    spi->_callback_context   = NULL;
}

void mock_spi_callback(void* context, const uint8_t byte)
{
    // Mock implementation of the spi callback function
    (void)context;  // Unused
    (void)byte;     // Unused
}

/* ============================================================================================== */

void test_spi_creation(void)
{
    struct spi_port spi = {0};
    struct spi_ops  ops = {0};
    helper_spi_create(&ops, &spi);

    TEST_ASSERT_NOT_NULL(spi.ops->initialize);
    TEST_ASSERT_NOT_NULL(spi.ops->transmit);
    TEST_ASSERT_NOT_NULL(spi.ops->receive);
    TEST_ASSERT_NOT_NULL(spi.ops->set_rx_callback);
    TEST_ASSERT_NOT_NULL(spi.ops->flush_tx);
    TEST_ASSERT_NOT_NULL(spi.ops->flush_rx);
    TEST_ASSERT_EQUAL_PTR(spi.ops->initialize, spi1_initialize);
    TEST_ASSERT_EQUAL_PTR(spi.ops->transmit, spi1_transmit);
    TEST_ASSERT_EQUAL_PTR(spi.ops->receive, spi1_receive);
    TEST_ASSERT_EQUAL_PTR(spi.ops->set_rx_callback, spi1_set_rx_callback);
    TEST_ASSERT_EQUAL_PTR(spi.ops->flush_tx, spi1_flush_tx);
    TEST_ASSERT_EQUAL_PTR(spi.ops->flush_rx, spi1_flush_rx);
}

/* ============================================================================================== */

void test_spi_callback(void)
{
    struct spi_port spi = {0};
    struct spi_ops  ops = {0};
    helper_spi_create(&ops, &spi);

    spi1_set_rx_callback_ExpectAndReturn(&spi, mock_spi_callback, NULL, 0);
    TEST_ASSERT_EQUAL(0, spi.ops->set_rx_callback(&spi, mock_spi_callback, NULL));
}

/* ============================================================================================== */

void test_spi_methods(void)
{
    struct spi_port spi = {0};
    struct spi_ops  ops = {0};
    helper_spi_create(&ops, &spi);

    spi1_initialize_ExpectAndReturn(&spi, 0);
    TEST_ASSERT_EQUAL(0, spi.ops->initialize(&spi));

    uint8_t tx_buffer[2] = {0x12, 0x34};
    spi1_transmit_ExpectAndReturn(&spi, tx_buffer, 2, 0);
    TEST_ASSERT_EQUAL(0, spi.ops->transmit(&spi, tx_buffer, 2));

    uint8_t rx_buffer;
    spi1_receive_ExpectAndReturn(&spi, &rx_buffer, 0);
    TEST_ASSERT_EQUAL(0, spi.ops->receive(&spi, &rx_buffer));

    spi1_set_rx_callback_ExpectAndReturn(&spi, mock_spi_callback, NULL, 0);
    TEST_ASSERT_EQUAL(0, spi.ops->set_rx_callback(&spi, mock_spi_callback, NULL));

    spi1_flush_tx_ExpectAndReturn(&spi, 0);
    TEST_ASSERT_EQUAL(0, spi.ops->flush_tx(&spi));

    spi1_flush_rx_ExpectAndReturn(&spi, 0);
    TEST_ASSERT_EQUAL(0, spi.ops->flush_rx(&spi));
}

/* ============================================================================================== */
