#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/ring_buffer.h"

TEST_SOURCE_FILE("../src/ring_buffer.c")

/* ============================================================================================== */

#define BUFFER_SIZE 10

void test_initialize(void)
{
    uint8_t            buffer[BUFFER_SIZE];
    struct ring_buffer rb;
    TEST_ASSERT_EQUAL(0, initialize(&rb, buffer, BUFFER_SIZE, true));
}

/* ============================================================================================== */

void test_push_data(void)
{
    uint8_t            buffer[BUFFER_SIZE];
    struct ring_buffer rb;
    TEST_ASSERT_EQUAL(0, initialize(&rb, buffer, BUFFER_SIZE, true));
    TEST_ASSERT_EQUAL(5, push(&rb, (uint8_t[]){1, 2, 3, 4, 5}, 5));
}

/* ============================================================================================== */

void test_pop_data(void)
{
    uint8_t            buffer[BUFFER_SIZE];
    struct ring_buffer rb;
    TEST_ASSERT_EQUAL(0, initialize(&rb, buffer, BUFFER_SIZE, true));
    TEST_ASSERT_EQUAL(5, push(&rb, (uint8_t[]){1, 2, 3, 4, 5}, 5));

    uint8_t pop_buffer[5];
    TEST_ASSERT_EQUAL(5, pop(&rb, pop_buffer, 5));

    uint8_t expected[] = {1, 2, 3, 4, 5};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, pop_buffer, 5);
}

/* ============================================================================================== */

void test_is_empty(void)
{
    uint8_t            buffer[BUFFER_SIZE];
    struct ring_buffer rb;
    TEST_ASSERT_EQUAL(0, initialize(&rb, buffer, BUFFER_SIZE, true));
    TEST_ASSERT_TRUE(is_empty(&rb));
    TEST_ASSERT_EQUAL(1, push(&rb, (uint8_t[]){0xFF}, 1));
    TEST_ASSERT_FALSE(is_empty(&rb));
}

/* ============================================================================================== */

void test_overwrite_data(void)
{
    uint8_t            buffer[BUFFER_SIZE];
    struct ring_buffer rb;
    TEST_ASSERT_EQUAL(0, initialize(&rb, buffer, BUFFER_SIZE, true));

    uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    TEST_ASSERT_EQUAL(sizeof(data), push(&rb, data, sizeof(data)));
    TEST_ASSERT_TRUE(is_full(&rb));
}

/* ============================================================================================== */

void test_not_overwrite_data(void)
{
    uint8_t            buffer[BUFFER_SIZE];
    struct ring_buffer rb;
    TEST_ASSERT_EQUAL(0, initialize(&rb, buffer, BUFFER_SIZE, false));

    uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 1,
                      push(&rb, data, sizeof(data)));  // Writes only up to capacity - 1
    TEST_ASSERT_TRUE(is_full(&rb));
}

/* ============================================================================================== */

void test_available(void)
{
    uint8_t            buffer[BUFFER_SIZE];
    struct ring_buffer rb;
    TEST_ASSERT_EQUAL(0, initialize(&rb, buffer, BUFFER_SIZE, true));

    uint8_t data[] = {1, 2, 3, 4, 5};
    TEST_ASSERT_EQUAL(sizeof(data), push(&rb, data, sizeof(data)));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(data), available(&rb));
}

/* ============================================================================================== */
