/**
 * @file
 * @brief This file contains the unit tests for the spi module.
 */

/* ============================================================================================== */

#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/spi.h"
#include "../src/spi.c"

/* Mocked interfaces */
#include "mock_spi_driver.h"

/* ============================================================================================== */

#define TEST_SPI_WORD 0x1234

/* ============================================================================================== */

void helper_spi_create(spi_t* spi)
{
    static const spi_drivers_t spi_drivers = {.initialize      = spi1_initialize,
                                              .write_word      = spi1_write_word,
                                              .read_word       = spi1_read_word,
                                              .set_rx_callback = spi1_set_rx_callback,
                                              .clear_buffers   = spi1_clear_buffers};
    spi_create(spi, &spi_drivers);
}

void mock_spi_callback(uint16_t data)
{
    // Mock implementation of the SPI callback function
}

/* ============================================================================================== */

void test_spi_creation(void)
{
    static const spi_drivers_t spi_drivers = {.initialize      = spi1_initialize,
                                              .write_word      = spi1_write_word,
                                              .read_word       = spi1_read_word,
                                              .set_rx_callback = spi1_set_rx_callback,
                                              .clear_buffers   = spi1_clear_buffers};
    spi_t                      spi;
    TEST_ASSERT_EQUAL(0, spi_create(&spi, &spi_drivers));

    TEST_ASSERT_NOT_NULL(spi.initialize);
    TEST_ASSERT_NOT_NULL(spi.write_word);
    TEST_ASSERT_NOT_NULL(spi.read_word);
    TEST_ASSERT_NOT_NULL(spi.set_rx_callback);
    TEST_ASSERT_NOT_NULL(spi.clear_buffers);
    TEST_ASSERT_EQUAL_PTR(spi.initialize, spi1_initialize);
    TEST_ASSERT_EQUAL_PTR(spi.write_word, spi1_write_word);
    TEST_ASSERT_EQUAL_PTR(spi.read_word, spi1_read_word);
    TEST_ASSERT_EQUAL_PTR(spi.set_rx_callback, spi1_set_rx_callback);
    TEST_ASSERT_EQUAL_PTR(spi.clear_buffers, spi1_clear_buffers);
    TEST_ASSERT_EQUAL(-EFAULT, spi_create(NULL, &spi_drivers));
    TEST_ASSERT_EQUAL(-EFAULT, spi_create(&spi, NULL));
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

    spi1_write_word_ExpectAndReturn(TEST_SPI_WORD, 0);
    TEST_ASSERT_EQUAL(0, spi.write_word(TEST_SPI_WORD));

    uint16_t read_word = 0;
    spi1_read_word_ExpectAndReturn(&read_word, 0);
    TEST_ASSERT_EQUAL(0, spi.read_word(&read_word));
    TEST_ASSERT_EQUAL(0, read_word);

    spi1_set_rx_callback_ExpectAndReturn(mock_spi_callback, 0);
    TEST_ASSERT_EQUAL(0, spi.set_rx_callback(mock_spi_callback));

    spi1_clear_buffers_Expect();
    spi.clear_buffers();
}

/* ============================================================================================== */
