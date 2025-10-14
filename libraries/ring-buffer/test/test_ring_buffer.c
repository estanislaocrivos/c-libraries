#include "../../test/support/unity.h"

/* ============================================================================================== */

#include "../inc/ring_buffer.h"

TEST_SOURCE_FILE("../src/ring_buffer.c")

/* ============================================================================================== */

#define RING_BUFFER_SIZE      20
#define TEST_DATA_BUFFER_SIZE 13

static const uint8_t test_data_buffer[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

void test_initialize(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));
}

/* ============================================================================================== */

void test_use_no_initialized(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(-EPERM, push(&rb, test_data_buffer, sizeof(test_data_buffer)));

    uint8_t pop_buffer[TEST_DATA_BUFFER_SIZE] = {0};
    TEST_ASSERT_EQUAL(-EPERM, pop(&rb, pop_buffer, sizeof(pop_buffer)));

    bool empty = false;
    TEST_ASSERT_EQUAL(-EPERM, is_empty(&rb, &empty));

    bool full = false;
    TEST_ASSERT_EQUAL(-EPERM, is_full(&rb, &full));

    size_t available_space = 0;
    TEST_ASSERT_EQUAL(-EPERM, available(&rb, &available_space));
}

/* ============================================================================================== */

void test_push_data(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    TEST_ASSERT_EQUAL(0, push(&rb, test_data_buffer, sizeof(test_data_buffer)));
}

/* ============================================================================================== */

void test_pop_data(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    TEST_ASSERT_EQUAL(0, push(&rb, test_data_buffer, sizeof(test_data_buffer)));

    uint8_t pop_buffer[TEST_DATA_BUFFER_SIZE] = {0};
    TEST_ASSERT_EQUAL(0, pop(&rb, pop_buffer, sizeof(pop_buffer)));

    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_data_buffer, pop_buffer, sizeof(test_data_buffer));
}

/* ============================================================================================== */

void test_is_empty(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    bool empty = false;
    is_empty(&rb, &empty);
    TEST_ASSERT_TRUE(empty);

    TEST_ASSERT_EQUAL(0, push(&rb, test_data_buffer, sizeof(test_data_buffer)));

    is_empty(&rb, &empty);
    TEST_ASSERT_FALSE(empty);
}

/* ============================================================================================== */

void test_is_full(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    bool full = false;
    is_full(&rb, &full);
    TEST_ASSERT_FALSE(full);

    static const uint8_t buffer_to_full_occupancy[RING_BUFFER_SIZE - 1] = {1};
    TEST_ASSERT_EQUAL(0, push(&rb, buffer_to_full_occupancy, sizeof(buffer_to_full_occupancy)));

    is_full(&rb, &full);
    TEST_ASSERT_TRUE(full);
}

/* ============================================================================================== */

void test_overwrite_data(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    static const uint8_t buffer_to_full_occupancy[RING_BUFFER_SIZE - 1] = {1};
    TEST_ASSERT_EQUAL(0, push(&rb, buffer_to_full_occupancy, sizeof(buffer_to_full_occupancy)));

    TEST_ASSERT_EQUAL(
        0, push(&rb, (const uint8_t[]){0xFF}, 1));  // This should overwrite the first value (1)

    uint8_t pop_buffer[RING_BUFFER_SIZE] = {0};
    TEST_ASSERT_EQUAL(0, pop(&rb, pop_buffer, RING_BUFFER_SIZE - 1));
    TEST_ASSERT_EQUAL_UINT8(0xFF, pop_buffer[RING_BUFFER_SIZE - 2]);
}

/* ============================================================================================== */

void test_not_overwrite_data(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = false,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    static const uint8_t buffer_to_full_occupancy[RING_BUFFER_SIZE - 1] = {1};
    TEST_ASSERT_EQUAL(0, push(&rb, buffer_to_full_occupancy, sizeof(buffer_to_full_occupancy)));

    TEST_ASSERT_EQUAL(-ENOSPC, push(&rb, (const uint8_t[]){0xAA}, 1));
}

/* ============================================================================================== */

void test_available(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    static const uint8_t test_data[] = {1, 2, 3, 4, 5, 6};
    TEST_ASSERT_EQUAL(0, push(&rb, test_data, sizeof(test_data)));

    size_t available_space = 0;
    available(&rb, &available_space);
    TEST_ASSERT_EQUAL(RING_BUFFER_SIZE - sizeof(test_data), available_space);
}

/* ============================================================================================== */

void test_pop_partial(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    static const uint8_t test_data[] = {1, 2, 3, 4, 5};
    TEST_ASSERT_EQUAL(0, push(&rb, test_data, sizeof(test_data)));

    uint8_t pop_buffer[3] = {0};
    TEST_ASSERT_EQUAL(0, pop(&rb, pop_buffer, 3));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_data, pop_buffer, 3);
}

/* ============================================================================================== */

void test_push_pop_alternated(void)
{
    uint8_t                   buffer[RING_BUFFER_SIZE];
    struct ring_buffer        rb     = {0};
    struct ring_buffer_config config = {
        .buffer    = buffer,
        .size      = RING_BUFFER_SIZE,
        .overwrite = true,
    };
    TEST_ASSERT_EQUAL(0, initialize(&rb, &config));

    static const uint8_t data1[] = {10, 20, 30, 40, 50};
    TEST_ASSERT_EQUAL(0, push(&rb, data1, 5));

    uint8_t pop1[3] = {0};
    TEST_ASSERT_EQUAL(0, pop(&rb, pop1, 3));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data1, pop1, 3);

    static const uint8_t data2[] = {60, 70};
    TEST_ASSERT_EQUAL(0, push(&rb, data2, 2));

    uint8_t pop2[4] = {0};
    TEST_ASSERT_EQUAL(0, pop(&rb, pop2, 4));

    static const uint8_t expected[] = {40, 50, 60, 70};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, pop2, 4);
}

/* ============================================================================================== */
