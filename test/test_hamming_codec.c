#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================================================================================== */

#include "support/unity.h"

/* ============================================================================================== */

#include "../hamming_codec/hamming_codec.h"
#include "../hamming_codec/hamming_codec.c"

/* ============================================================================================== */

void test_calc_n_par_bits(void)
{
    TEST_ASSERT_EQUAL(4, _calc_n_par_bits(4));
    TEST_ASSERT_EQUAL(5, _calc_n_par_bits(7));
    TEST_ASSERT_EQUAL(5, _calc_n_par_bits(7));
    TEST_ASSERT_EQUAL(5, _calc_n_par_bits(8));
    TEST_ASSERT_EQUAL(5, _calc_n_par_bits(11));
}

/* ============================================================================================== */

void test_check_global_even_parity(void)
{
    uint8_t odd_buffer[7]  = {1, 1, 1, 1, 1, 1, 1};
    uint8_t even_buffer[7] = {0, 1, 1, 1, 1, 1, 1};
    TEST_ASSERT_EQUAL(true, _check_global_even_parity(even_buffer, 7));
    TEST_ASSERT_EQUAL(false, _check_global_even_parity(odd_buffer, 7));
    TEST_ASSERT_EQUAL(false, _check_global_even_parity(odd_buffer, 7));
}

/* ============================================================================================== */

void test_get_bit(void)
{
    uint8_t buffer[] = {0, 1, 1, 0, 1};
    TEST_ASSERT_EQUAL(true, _get_bit_from_array(buffer, 1));
    TEST_ASSERT_EQUAL(true, _get_bit_from_array(buffer, 2));
    TEST_ASSERT_EQUAL(false, _get_bit_from_array(buffer, 3));
}

/* ============================================================================================== */

void test_find_par_bit_positions(void)
{
    uint8_t buffer[5];
    _find_par_bit_positions(_calc_n_par_bits(16), buffer);
    TEST_ASSERT_EQUAL(0, buffer[0]);
    TEST_ASSERT_EQUAL(1, buffer[1]);
    TEST_ASSERT_EQUAL(2, buffer[2]);
    TEST_ASSERT_EQUAL(4, buffer[3]);
    TEST_ASSERT_EQUAL(8, buffer[4]);
}

/* ============================================================================================== */

void test_round_to_mult_of_8(void)
{
    TEST_ASSERT_EQUAL(16, _round_to_mult_of_8(13));
    TEST_ASSERT_EQUAL(16, _round_to_mult_of_8(15));
    TEST_ASSERT_EQUAL(16, _round_to_mult_of_8(12));
    TEST_ASSERT_EQUAL(8, _round_to_mult_of_8(8));
    TEST_ASSERT_EQUAL(16, _round_to_mult_of_8(9));
    TEST_ASSERT_EQUAL(16, _round_to_mult_of_8(16));
    TEST_ASSERT_EQUAL(24, _round_to_mult_of_8(17));
}

/* ============================================================================================== */

#define PAR_BITS_POS_ARRAY_SIZE 16

typedef struct
{
    uint8_t data_size;
    uint8_t data_size_bits;
    uint8_t n_par_bits;
    uint8_t bit_array_size;
    uint8_t par_bits_pos_array[PAR_BITS_POS_ARRAY_SIZE];
} config_params_t;

/* Auxiliary */
void calc_config(uint8_t* data, uint8_t data_size, config_params_t* config)
{
    config->data_size      = data_size;
    config->data_size_bits = data_size * 8;
    config->n_par_bits     = _calc_n_par_bits(config->data_size_bits);
    config->bit_array_size = _round_to_mult_of_8(config->data_size_bits + config->n_par_bits);
    _find_par_bit_positions(config->n_par_bits, config->par_bits_pos_array);
    printf("\nData size: %d", data_size);
    printf("\nData: ");
    for (int i = 0; i < data_size; i++)
    {
        printf("%#x ", data[i]);
    }
    printf("\nN par. bits: %d", config->n_par_bits);
    printf("\nBit array size: %d", config->bit_array_size);
    printf("\nPar. bits positions:");
    for (int i = 0; i < config->n_par_bits; i++)
    {
        printf("%d ", config->par_bits_pos_array[i]);
    }
}

/* ============================================================================================== */

void test_parse_data_to_array_0(void)
{
    printf("\ntest_parse_data_to_array_0()");
    uint8_t         data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
}

/* ============================================================================================== */

void test_parse_data_to_array_1(void)
{
    printf("\ntest_parse_data_to_array_1()");
    uint8_t         data[] = {0x5F};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    TEST_ASSERT_EQUAL(0, bit_array_g[0]);
    TEST_ASSERT_EQUAL(0, bit_array_g[1]);
    TEST_ASSERT_EQUAL(0, bit_array_g[2]);
    TEST_ASSERT_EQUAL(0, bit_array_g[3]);
    TEST_ASSERT_EQUAL(0, bit_array_g[4]);
    TEST_ASSERT_EQUAL(0, bit_array_g[5]);
    TEST_ASSERT_EQUAL(0, bit_array_g[6]);
    TEST_ASSERT_EQUAL(0, bit_array_g[7]);
    TEST_ASSERT_EQUAL(0, bit_array_g[8]);
    TEST_ASSERT_EQUAL(1, bit_array_g[9]);
    TEST_ASSERT_EQUAL(0, bit_array_g[10]);
    TEST_ASSERT_EQUAL(1, bit_array_g[11]);
    TEST_ASSERT_EQUAL(1, bit_array_g[12]);
    TEST_ASSERT_EQUAL(1, bit_array_g[13]);
    TEST_ASSERT_EQUAL(1, bit_array_g[14]);
    TEST_ASSERT_EQUAL(1, bit_array_g[15]);
}

/* ============================================================================================== */

void test_parse_data_to_array_2(void)
{
    printf("\ntest_parse_data_to_array_2()");
    uint8_t         data[] = {0xE7};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    TEST_ASSERT_EQUAL(0, bit_array_g[0]);
    TEST_ASSERT_EQUAL(0, bit_array_g[1]);
    TEST_ASSERT_EQUAL(0, bit_array_g[2]);
    TEST_ASSERT_EQUAL(0, bit_array_g[3]);
    TEST_ASSERT_EQUAL(0, bit_array_g[4]);
    TEST_ASSERT_EQUAL(0, bit_array_g[5]);
    TEST_ASSERT_EQUAL(0, bit_array_g[6]);
    TEST_ASSERT_EQUAL(1, bit_array_g[7]);
    TEST_ASSERT_EQUAL(0, bit_array_g[8]);
    TEST_ASSERT_EQUAL(1, bit_array_g[9]);
    TEST_ASSERT_EQUAL(1, bit_array_g[10]);
    TEST_ASSERT_EQUAL(0, bit_array_g[11]);
    TEST_ASSERT_EQUAL(0, bit_array_g[12]);
    TEST_ASSERT_EQUAL(1, bit_array_g[13]);
    TEST_ASSERT_EQUAL(1, bit_array_g[14]);
    TEST_ASSERT_EQUAL(1, bit_array_g[15]);
}

/* ============================================================================================== */

void test_parse_encoded_word_to_bit_array_0(void)
{
    printf("\ntest_parse_encoded_word_to_bit_array_0()");
    uint8_t encoded_word[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t bit_array_size = sizeof(encoded_word) * 8;
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_encoded_word_to_bit_array(encoded_word, sizeof(encoded_word), bit_array_g,
                                     bit_array_size);
    printf("\nBit array: ");
    for (int i = 0; i < bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
}

/* ============================================================================================== */

void test_parse_decoded_bit_array_to_data(void)
{
    printf("\ntest_parse_decoded_bit_array_to_data()");
    uint8_t         data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    config_params_t config;
    calc_config(data, sizeof(data), &config);

    uint8_t n_data_bits  = _round_to_mult_of_8(config.bit_array_size - config.n_par_bits);
    uint8_t n_data_bytes = n_data_bits / 8;
    printf("\nN data bits: %d", n_data_bits);
    printf("\nN data bytes: %d", n_data_bytes);

    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }

    uint8_t decoded_data[sizeof(data) + 8];
    _clear_array(decoded_data, sizeof(decoded_data));
    uint8_t dec_data_size = _parse_decoded_bit_array_to_data(
        bit_array_g, config.bit_array_size, decoded_data, sizeof(data), config.n_par_bits,
        config.par_bits_pos_array);
    printf("\nDecoded word size: %d", dec_data_size);
    printf("\nDecoded word: ");
    for (int i = 0; i < dec_data_size; i++)
    {
        printf("%#x ", decoded_data[i]);
    }
}

/* ============================================================================================== */

void test_parse_encoded_bit_array_to_word(void)
{
    printf("\ntest_parse_encoded_bit_array_to_word()");
    uint8_t         data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    _hamming_encode_bit_array(sizeof(data), bit_array_g, config.bit_array_size, encoded_bit_array_g,
                              config.bit_array_size, config.n_par_bits, config.par_bits_pos_array);
    printf("\nEncoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", encoded_bit_array_g[i]);
    }
    uint8_t encoded_word[16];
    _clear_array(encoded_word, 16);
    uint8_t enc_word_size = _parse_encoded_bit_array_to_word(
        encoded_bit_array_g, config.bit_array_size, encoded_word, sizeof(encoded_word));
    printf("\nEncoded word: ");
    for (int i = 0; i < enc_word_size; i++)
    {
        printf("%#x ", encoded_word[i]);
    }
}

/* ============================================================================================== */

void test_hamming_encode_array_0(void)
{
    printf("\ntest_hamming_encode_array_0()");
    uint8_t         data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    _hamming_encode_bit_array(sizeof(data), bit_array_g, config.bit_array_size, encoded_bit_array_g,
                              config.bit_array_size, config.n_par_bits, config.par_bits_pos_array);
    printf("\nEncoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", encoded_bit_array_g[i]);
    }
}

/* ============================================================================================== */

void test_hamming_encode_array_1(void)
{
    printf("\ntest_hamming_encode_array_1()");
    uint8_t         data[] = {0x2F};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    _hamming_encode_bit_array(sizeof(data), bit_array_g, config.bit_array_size, encoded_bit_array_g,
                              config.bit_array_size, config.n_par_bits, config.par_bits_pos_array);
    printf("\nEncoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", encoded_bit_array_g[i]);
    }
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[0]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[1]);
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[2]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[3]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[4]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[5]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[6]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[7]);
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[8]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[9]);
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[10]);
    TEST_ASSERT_EQUAL(0, encoded_bit_array_g[11]);
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[12]);
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[13]);
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[14]);
    TEST_ASSERT_EQUAL(1, encoded_bit_array_g[15]);
}

/* ============================================================================================== */

void test_hamming_decode_array_no_error(void)
{
    printf("\ntest_hamming_decode_array_no_error()");
    uint8_t         data[] = {0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _clear_array(encoded_bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _clear_array(decoded_bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array:     ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    _hamming_encode_bit_array(sizeof(data), bit_array_g, config.bit_array_size, encoded_bit_array_g,
                              config.bit_array_size, config.n_par_bits, config.par_bits_pos_array);
    printf("\nEncoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", encoded_bit_array_g[i]);
    }
    /* No errors */
    TEST_ASSERT_EQUAL(true, _hamming_decode_bit_array(encoded_bit_array_g, config.bit_array_size,
                                                      decoded_bit_array_g, config.bit_array_size));
    printf("\nDecoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", decoded_bit_array_g[i]);
    }
    uint8_t decoded_data[32];
    uint8_t dec_data_size = _parse_decoded_bit_array_to_data(
        decoded_bit_array_g, config.bit_array_size, decoded_data, sizeof(data), config.n_par_bits,
        config.par_bits_pos_array);
    printf("\nDecoded data: ");
    for (int i = 0; i < dec_data_size; i++)
    {
        printf("%#x ", decoded_data[i]);
    }
}

/* ============================================================================================== */

void test_hamming_decode_array_one_error(void)
{
    printf("\ntest_hamming_decode_array_one_error()");
    uint8_t         data[] = {0x2F};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _clear_array(encoded_bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _clear_array(decoded_bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    _hamming_encode_bit_array(sizeof(data), bit_array_g, config.bit_array_size, encoded_bit_array_g,
                              config.bit_array_size, config.n_par_bits, config.par_bits_pos_array);

    /* Introduce one error */
    encoded_bit_array_g[3] ^= 1;
    printf("\nEncoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", encoded_bit_array_g[i]);
    }

    /* Expect true (fixes error) */
    TEST_ASSERT_EQUAL(true, _hamming_decode_bit_array(encoded_bit_array_g, config.bit_array_size,
                                                      decoded_bit_array_g, config.bit_array_size));
    printf("\nDecoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", decoded_bit_array_g[i]);
    }

    uint8_t decoded_data[32];
    uint8_t dec_data_size = _parse_decoded_bit_array_to_data(
        decoded_bit_array_g, config.bit_array_size, decoded_data, sizeof(data), config.n_par_bits,
        config.par_bits_pos_array);
    printf("\nDecoded data: ");
    for (int i = 0; i < dec_data_size; i++)
    {
        printf("%#x ", decoded_data[i]);
    }
}

/* ============================================================================================== */

void test_hamming_decode_array_two_errors(void)
{
    printf("\ntest_hamming_decode_array_two_errors()");
    uint8_t         data[] = {0x2F, 0x2E, 0xFF, 0x44};
    config_params_t config;
    calc_config(data, sizeof(data), &config);
    _clear_array(bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _clear_array(encoded_bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _clear_array(decoded_bit_array_g, GLOBAL_BIT_ARRAY_SIZE);
    _parse_data_to_bit_array(data, sizeof(data), bit_array_g, config.bit_array_size,
                             config.n_par_bits, config.par_bits_pos_array);
    printf("\nBit array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", bit_array_g[i]);
    }
    _hamming_encode_bit_array(sizeof(data), bit_array_g, config.bit_array_size, encoded_bit_array_g,
                              config.bit_array_size, config.n_par_bits, config.par_bits_pos_array);

    /* Introduce two errors */
    encoded_bit_array_g[3] ^= 1;
    encoded_bit_array_g[4] ^= 1;

    printf("\nEncoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", encoded_bit_array_g[i]);
    }

    /* Expected false (cannot fix two errors, only detect) */
    TEST_ASSERT_EQUAL(false, _hamming_decode_bit_array(encoded_bit_array_g, config.bit_array_size,
                                                       decoded_bit_array_g, config.bit_array_size));
    printf("\nDecoded array: ");
    for (int i = 0; i < config.bit_array_size; i++)
    {
        printf("%d ", decoded_bit_array_g[i]);
    }

    uint8_t decoded_data[32];
    uint8_t dec_data_size = _parse_decoded_bit_array_to_data(
        decoded_bit_array_g, config.bit_array_size, decoded_data, sizeof(data), config.n_par_bits,
        config.par_bits_pos_array);
    printf("\nDecoded data: ");
    for (int i = 0; i < dec_data_size; i++)
    {
        printf("%#x ", decoded_data[i]);
    }
}

/* ============================================================================================== */

void test_hamming_encode_0(void)
{
    printf("\ntest_hamming_encode_0()");
    uint8_t data[] = {0x07, 0x22};
    uint8_t encoded_data[2 * sizeof(data)];
    uint8_t enc_data_size
        = hamming_encode_bytestream(data, sizeof(data), encoded_data, sizeof(encoded_data));
    printf("\nEncoded data: ");
    for (int i = 0; i < enc_data_size; i++)
    {
        printf("%#x ", encoded_data[i]);
    }
}

/* ============================================================================================== */

void test_hamming_encode_decode_0(void)
{
    printf("\ntest_hamming_encode_decode_0()");
    uint8_t data[] = {0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xAA, 0xBB, 0xCC, 0xDD};
    printf("\nData: ");
    for (int i = 0; i < sizeof(data); i++)
    {
        printf("%#x ", data[i]);
    }
    uint8_t encoded_data[32];
    uint8_t enc_data_size
        = hamming_encode_bytestream(data, sizeof(data), encoded_data, sizeof(encoded_data));
    printf("\nEncoded data: ");
    for (int i = 0; i < enc_data_size; i++)
    {
        printf("%#x ", encoded_data[i]);
    }

    uint8_t decoded_data[32];
    uint8_t dec_data_size = hamming_decode_bytestream(encoded_data, enc_data_size, decoded_data,
                                                      sizeof(decoded_data));
    printf("\nDecoded data: ");
    for (int i = 0; i < dec_data_size; i++)
    {
        printf("%#x ", decoded_data[i]);
    }
}

/* ============================================================================================== */

void test_hamming_encode_decode_1(void)
{
    printf("\ntest_hamming_encode_decode_1()");
    uint8_t data[25];
    for (uint16_t i = 0; i < sizeof(data); ++i)
    {
        data[i] = (uint8_t)i;
    }
    printf("\nData: ");
    for (int i = 0; i < sizeof(data); i++)
    {
        printf("%#x ", data[i]);
    }
    uint8_t encoded_data[32];
    uint8_t enc_data_size
        = hamming_encode_bytestream(data, sizeof(data), encoded_data, sizeof(encoded_data));
    printf("\nEncoded data: ");
    for (int i = 0; i < enc_data_size; i++)
    {
        printf("%#x ", encoded_data[i]);
    }

    uint8_t decoded_data[32];
    uint8_t dec_data_size = hamming_decode_bytestream(encoded_data, enc_data_size, decoded_data,
                                                      sizeof(decoded_data));
    printf("\nDecoded data: ");
    for (int i = 0; i < dec_data_size; i++)
    {
        printf("%#x ", decoded_data[i]);
    }
}

/* ============================================================================================== */

void test_hamming_encode_word(void)
{
    printf("\ntest_hamming_encode_word()");
    printf("\nEncoded: %#x", hamming_encode_word(0x2E));
}

/* ============================================================================================== */

void test_hamming_decode_word(void)
{
    printf("\ntest_hamming_decode_word()");
    printf("\nDecoded: %#x", hamming_decode_word(0x155E));
}

/* ============================================================================================== */

void test_calc_enc_data_size(void)
{
    printf("\ntest_calc_enc_data_size()");
    printf("\nData size: %d", calc_enc_data_size(1));
}

/* ============================================================================================== */

void test_hamming_encode_decode_2(void)
{
    printf("\ntest_hamming_encode_decode_2()");
    hamming_codec_t codec;
    hamming_codec_create(&codec);

    uint8_t data[] = {0xF0, 0xff, 0xfe};
    printf("\nData: ");
    for (int i = 0; i < sizeof(data); i++)
    {
        printf("%#x ", data[i]);
    }
    uint8_t encoded_data[32];
    uint8_t enc_data_size
        = codec.encode_bytestream(data, sizeof(data), encoded_data, sizeof(encoded_data));
    printf("\nEncoded data: ");
    for (int i = 0; i < enc_data_size; i++)
    {
        printf("%#x ", encoded_data[i]);
    }

    uint8_t decoded_data[32];
    uint8_t dec_data_size
        = codec.decode_bytestream(encoded_data, enc_data_size, decoded_data, sizeof(decoded_data));
    printf("\nDecoded data: ");
    for (int i = 0; i < dec_data_size; i++)
    {
        printf("%#x ", decoded_data[i]);
    }
}

/* ============================================================================================== */
