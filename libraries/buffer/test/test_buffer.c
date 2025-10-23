#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/buffer.h"
TEST_SOURCE_FILE("../src/buffer.c")

/* ============================================================================================== */

void test_create_buffer(void)
{
    struct buffer buf;
    uint8_t       buffer[10];
    buf.buffer = buffer;
    buf.size   = sizeof(buffer);
    TEST_ASSERT_NOT_NULL(buf.buffer);
    TEST_ASSERT_EQUAL(0, buffer_init(&buf));
    TEST_ASSERT_EQUAL(0, buf.index);
    TEST_ASSERT_EQUAL(sizeof(buffer), buf.size);
}

/* ============================================================================================== */

void test_push_to_buffer(void)
{
    struct buffer buf;
    uint8_t       buffer[5];
    buf.buffer = buffer;
    buf.size   = sizeof(buffer);
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        TEST_ASSERT_EQUAL(0, buffer_push(&buf, (uint8_t)i));
        TEST_ASSERT_EQUAL(i + 1, buf.index);
        TEST_ASSERT_EQUAL((uint8_t)i, buf.buffer[i]);
    }
    TEST_ASSERT_EQUAL(-ENOBUFS, buffer_push(&buf, 0xFF));
}

/* ============================================================================================== */

void test_clear_buffer(void)
{
    struct buffer buf;
    uint8_t       buffer[5];
    buf.buffer = buffer;
    buf.size   = sizeof(buffer);
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        buffer_push(&buf, (uint8_t)i);
    }
    TEST_ASSERT_EQUAL(buf.size, buf.index);
    TEST_ASSERT_EQUAL(0, buffer_clear(&buf));
    TEST_ASSERT_EQUAL(0, buf.index);
    for (size_t i = 0; i < buf.size; i++)
    {
        TEST_ASSERT_EQUAL(0, buf.buffer[i]);
    }
}

/* ============================================================================================== */

void test_reset_buffer_index(void)
{
    struct buffer buf;
    uint8_t       buffer[5];
    buf.buffer = buffer;
    buf.size   = sizeof(buffer);
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        buffer_push(&buf, (uint8_t)i);
    }
    TEST_ASSERT_EQUAL(buf.size, buf.index);
    TEST_ASSERT_EQUAL(0, buffer_reset_index(&buf));
    TEST_ASSERT_EQUAL(0, buf.index);
}

/* ============================================================================================== */
