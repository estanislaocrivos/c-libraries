#include "../../test/support/unity.h"

/* ============================================================================================== */

#include <stdio.h>

#include "../inc/crc.h"
TEST_SOURCE_FILE("../src/crc.c")

// Used https://crccalc.com/?crc=123456789&method=&datatype=ascii&outtype=hex to generate expected
// test results

/* ============================================================================================== */

void test_crc8_autosar(void)
{
    const struct crc crc8 = {.crc8_final_xor_value = 0xFF,
                             .crc8_initial_value   = 0xFF,
                             .crc8_polynomial      = 0x2F,
                             .reflect_input        = false,
                             .reflect_output       = false};

    const uint8_t data_1[] = {0xC2};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8, data_1, sizeof(data_1), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0xE6, result);

    const uint8_t data_2[] = {0xAA, 0xBB, 0xCC};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_2, sizeof(data_2), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0xCF, result);

    const uint8_t data_3[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_3, sizeof(data_3), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x88, result);
}

/* ============================================================================================== */

void test_crc8_bluetooth(void)
{
    const struct crc crc8 = {.crc8_final_xor_value = 0x00,
                             .crc8_initial_value   = 0x00,
                             .crc8_polynomial      = 0xA7,
                             .reflect_input        = true,
                             .reflect_output       = true};

    const uint8_t data_1[] = {0xC2};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8, data_1, sizeof(data_1), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0xA4, result);

    const uint8_t data_2[] = {0xAA, 0xBB, 0xCC};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_2, sizeof(data_2), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x5A, result);

    const uint8_t data_3[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_3, sizeof(data_3), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x09, result);
}

/* ============================================================================================== */

void test_crc8_cdma2000(void)
{
    const struct crc crc8 = {.crc8_final_xor_value = 0x00,
                             .crc8_initial_value   = 0xFF,
                             .crc8_polynomial      = 0x9B,
                             .reflect_input        = false,
                             .reflect_output       = false};

    const uint8_t data_1[] = {0xC2};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8, data_1, sizeof(data_1), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x15, result);

    const uint8_t data_2[] = {0xAA, 0xBB, 0xCC};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_2, sizeof(data_2), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x6A, result);

    const uint8_t data_3[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_3, sizeof(data_3), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0xF0, result);
}

/* ============================================================================================== */

void test_crc8_darc(void)
{
    const struct crc crc8 = {.crc8_final_xor_value = 0x00,
                             .crc8_initial_value   = 0x00,
                             .crc8_polynomial      = 0x39,
                             .reflect_input        = true,
                             .reflect_output       = true};

    const uint8_t data_1[] = {0xC2};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8, data_1, sizeof(data_1), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x36, result);

    const uint8_t data_2[] = {0xAA, 0xBB, 0xCC};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_2, sizeof(data_2), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x86, result);

    const uint8_t data_3[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_3, sizeof(data_3), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x4E, result);
}

/* ============================================================================================== */

void test_crc8_lte(void)
{
    const struct crc crc8 = {.crc8_final_xor_value = 0x00,
                             .crc8_initial_value   = 0x00,
                             .crc8_polynomial      = 0x9B,
                             .reflect_input        = false,
                             .reflect_output       = false};

    const uint8_t data_1[] = {0xC2};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8, data_1, sizeof(data_1), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x6E, result);

    const uint8_t data_2[] = {0xAA, 0xBB, 0xCC};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_2, sizeof(data_2), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0xAC, result);

    const uint8_t data_3[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    result                 = 0;
    status                 = crc8_calculate(&crc8, data_3, sizeof(data_3), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x24, result);
}

/* ============================================================================================== */

void test_crc8_propietary(void)
{
    const struct crc crc8 = {.crc8_final_xor_value = 0x00,
                             .crc8_initial_value   = 0x00,
                             .crc8_polynomial      = 0x97,
                             .reflect_input        = false,
                             .reflect_output       = false};

    const uint8_t data_1[] = {0xAA, 0x04, 0x01, 0x02, 0x03, 0x04, 0x55};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8, data_1, sizeof(data_1), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    printf("CRC8 Propietary Test Result: 0x%02X\n", result);
}

/* ============================================================================================== */
