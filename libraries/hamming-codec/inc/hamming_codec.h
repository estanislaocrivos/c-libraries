#ifndef HAMMING_H
#define HAMMING_H

/* ============================================================================================== */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

typedef uint8_t (*hamming_encode_bytestream_t)(const uint8_t*, uint8_t, uint8_t*, uint8_t);
typedef uint8_t (*hamming_decode_bytestream_t)(const uint8_t*, uint8_t, uint8_t*, uint8_t);
typedef uint16_t (*hamming_encode_word_t)(uint8_t);
typedef uint8_t (*hamming_decode_word_t)(uint16_t);
typedef uint8_t (*calc_enc_data_size_t)(uint8_t);

/* ============================================================================================== */

typedef struct
{
    hamming_encode_bytestream_t
        encode_bytestream; /* Function to encode a byte stream using Hamming code */
    hamming_decode_bytestream_t decode_bytestream;
    hamming_encode_word_t       encode_word;
    hamming_decode_word_t       decode_word;
    calc_enc_data_size_t        calc_enc_data_size;
} hamming_codec_t;

/* ============================================================================================== */

int8_t hamming_codec_create(hamming_codec_t* self);

/* ============================================================================================== */

#endif /* HAMMING_H */
