#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/crc.h"

TEST_SOURCE_FILE("../src/crc.c")

/* ============================================================================================== */

void test_crc8_autosar(void)
{
    struct crc crc8_autosar = {.crc8_final_xor_value = 0xFF,
                               .crc8_initial_value   = 0xFF,
                               .crc8_polynomial      = 0x2F,
                               .reflect_input        = false,
                               .reflect_output       = false};

    const uint8_t data_1[] = {0xC2};
    uint8_t       result   = 0;
    int8_t        status   = crc8_calculate(&crc8_autosar, data_1, sizeof(data_1), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0xE6, result);

    const uint8_t data_2[] = {0xAA, 0xBB, 0xCC};
    result                 = 0;
    status                 = crc8_calculate(&crc8_autosar, data_2, sizeof(data_2), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0xCF, result);

    const uint8_t data_3[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    result                 = 0;
    status                 = crc8_calculate(&crc8_autosar, data_3, sizeof(data_3), &result);
    TEST_ASSERT_EQUAL_INT8(0, status);
    TEST_ASSERT_EQUAL_UINT8(0x88, result);
}

/* ============================================================================================== */
