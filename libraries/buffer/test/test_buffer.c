#include "buffer.h"
#include "unity.h"

/* ========================================================================== */

void test_create_buffer(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = data, .size = sizeof(data)};
    TEST_ASSERT_NOT_NULL(buf.buffer);
    TEST_ASSERT_EQUAL(0, buffer_init(&buf));
    TEST_ASSERT_EQUAL(0, buf.index);
    TEST_ASSERT_EQUAL(sizeof(data), buf.size);
}

/* ========================================================================== */

void test_push_to_buffer(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = data, .size = sizeof(data)};
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        TEST_ASSERT_EQUAL(0, buffer_push(&buf, (uint8_t)i));
        TEST_ASSERT_EQUAL(i + 1, buf.index);
        TEST_ASSERT_EQUAL((uint8_t)i, buf.buffer[i]);
    }
    TEST_ASSERT_EQUAL(-ENOBUFS, buffer_push(&buf, 0xFF));
}

/* ========================================================================== */

void test_clear_buffer(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = data, .size = sizeof(data)};
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        buffer_push(&buf, (uint8_t)i);
    }
    TEST_ASSERT_EQUAL(buf.size, buf.index);
    TEST_ASSERT_EQUAL(0, buffer_reset(&buf));
    TEST_ASSERT_EQUAL(0, buf.index);
    for (size_t i = 0; i < buf.size; i++)
    {
        TEST_ASSERT_EQUAL(0, buf.buffer[i]);
    }
}

/* ========================================================================== */

void test_reset_buffer_index(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = data, .size = sizeof(data)};
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        buffer_push(&buf, (uint8_t)i);
    }
    TEST_ASSERT_EQUAL(buf.size, buf.index);
    TEST_ASSERT_EQUAL(0, buffer_reset_index(&buf));
    TEST_ASSERT_EQUAL(0, buf.index);
}

/* ========================================================================== */

void test_pop_from_buffer(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = data, .size = sizeof(data)};
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        buffer_push(&buf, (uint8_t)i);
    }
    TEST_ASSERT_EQUAL(buf.size, buf.index);
    uint8_t byte;
    for (size_t i = buf.size; i > 0; i--)
    {
        TEST_ASSERT_EQUAL(0, buffer_pop(&buf, &byte));
        TEST_ASSERT_EQUAL((uint8_t)(i - 1), byte);
        TEST_ASSERT_EQUAL(i - 1, buf.index);
    }
    TEST_ASSERT_EQUAL(-ENODATA, buffer_pop(&buf, &byte));
}

/* ========================================================================== */

void test_buffer_at(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = data, .size = sizeof(data)};
    buffer_init(&buf);
    for (size_t i = 0; i < buf.size; i++)
    {
        buffer_push(&buf, (uint8_t)(i * 2));
    }
    uint8_t byte;
    for (size_t i = 0; i < buf.size; i++)
    {
        TEST_ASSERT_EQUAL(0, buffer_at(&buf, i, &byte));
        TEST_ASSERT_EQUAL((uint8_t)(i * 2), byte);
    }
    TEST_ASSERT_EQUAL(buf.size, buf.index);
    TEST_ASSERT_EQUAL(-EINVAL, buffer_at(&buf, buf.size, &byte));
    TEST_ASSERT_EQUAL(-EINVAL, buffer_at(&buf, buf.size + 10, &byte));
}

/* ========================================================================== */

void test_buffer_null_pointer_errors(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = NULL, .size = sizeof(data)};
    TEST_ASSERT_EQUAL(-EFAULT, buffer_init(NULL));
    TEST_ASSERT_EQUAL(-EFAULT, buffer_init(&buf));
    TEST_ASSERT_EQUAL(-EFAULT, buffer_push(NULL, 0));
    uint8_t byte;
    TEST_ASSERT_EQUAL(-EFAULT, buffer_pop(NULL, &byte));
    TEST_ASSERT_EQUAL(-EFAULT, buffer_pop(&buf, NULL));
    TEST_ASSERT_EQUAL(-EFAULT, buffer_at(NULL, 0, &byte));
    TEST_ASSERT_EQUAL(-EFAULT, buffer_at(&buf, 0, NULL));
    TEST_ASSERT_EQUAL(-EFAULT, buffer_reset(NULL));
    TEST_ASSERT_EQUAL(-EFAULT, buffer_reset_index(NULL));
}

/* ========================================================================== */

void test_buffer_not_initialized_errors(void)
{
    uint8_t       data[10];
    struct buffer buf   = {.buffer = data, .size = sizeof(data)};
    buf.was_initialized = false;
    TEST_ASSERT_EQUAL(-EPERM, buffer_push(&buf, 0));
    uint8_t byte;
    TEST_ASSERT_EQUAL(-EPERM, buffer_pop(&buf, &byte));
    TEST_ASSERT_EQUAL(-EPERM, buffer_at(&buf, 0, &byte));
    TEST_ASSERT_EQUAL(-EPERM, buffer_reset(&buf));
    TEST_ASSERT_EQUAL(-EPERM, buffer_reset_index(&buf));
}

/* ========================================================================== */

void test_buffer_invalid_config(void)
{
    uint8_t       data[10];
    struct buffer buf = {.buffer = data, .size = 0};
    TEST_ASSERT_EQUAL(-EINVAL, buffer_init(&buf));
}

/* ========================================================================== */
