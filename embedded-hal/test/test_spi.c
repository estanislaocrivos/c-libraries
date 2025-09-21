#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/spi.h"
#include "../src/spi.c"

/* Mocked interfaces */
#include "mock_spi_driver.h"

/* ============================================================================================== */

void helper_spi_create(spi_t* spi)
{
    spi->initialize      = spi1_initialize;
    spi->transfer        = spi1_transfer;
    spi->set_rx_callback = spi1_set_rx_callback;
    spi->clear_buffers   = spi1_clear_buffers;
}

void mock_spi_callback(const uint8_t* buffer, size_t length)
{
    // Mock implementation of the SPI callback function
}

/* ============================================================================================== */

void test_spi_creation(void)
{
    spi_t spi;
    helper_spi_create(&spi);

    TEST_ASSERT_NOT_NULL(spi.initialize);
    TEST_ASSERT_NOT_NULL(spi.transfer);
    TEST_ASSERT_NOT_NULL(spi.set_rx_callback);
    TEST_ASSERT_NOT_NULL(spi.clear_buffers);
    TEST_ASSERT_EQUAL_PTR(spi.initialize, spi1_initialize);
    TEST_ASSERT_EQUAL_PTR(spi.transfer, spi1_transfer);
    TEST_ASSERT_EQUAL_PTR(spi.set_rx_callback, spi1_set_rx_callback);
    TEST_ASSERT_EQUAL_PTR(spi.clear_buffers, spi1_clear_buffers);
}

/* ============================================================================================== */

void test_spi_callback(void)
{
    spi_t spi;
    helper_spi_create(&spi);

    spi1_set_rx_callback_ExpectAndReturn(mock_spi_callback, 0);
    TEST_ASSERT_EQUAL(0, spi.set_rx_callback(mock_spi_callback));
}

/* ============================================================================================== */

void test_spi_methods(void)
{
    spi_t spi;
    helper_spi_create(&spi);

    spi1_initialize_ExpectAndReturn(0);
    TEST_ASSERT_EQUAL(0, spi.initialize());

    uint8_t tx_buffer[2] = {0x12, 0x34};
    uint8_t rx_buffer[2] = {0};
    spi1_transfer_ExpectAndReturn(tx_buffer, rx_buffer, 2, 0);
    TEST_ASSERT_EQUAL(0, spi.transfer(tx_buffer, rx_buffer, 2));

    spi1_set_rx_callback_ExpectAndReturn(mock_spi_callback, 0);
    TEST_ASSERT_EQUAL(0, spi.set_rx_callback(mock_spi_callback));

    spi1_clear_buffers_Expect();
    spi.clear_buffers();
}

/* ============================================================================================== */

void test_spi_transfer(void)
{
    spi_t spi;
    helper_spi_create(&spi);

    uint8_t tx_buffer[2] = {0x12, 0x34};
    uint8_t rx_buffer[2] = {0};

    spi1_transfer_ExpectAndReturn(tx_buffer, rx_buffer, 2, 0);
    TEST_ASSERT_EQUAL(0, spi.transfer(tx_buffer, rx_buffer, 2));
}

/* ============================================================================================== */
