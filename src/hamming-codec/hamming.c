#include "../inc/hamming-codec/hamming.h"

/* ============================================================================================== */

/* Array used for storing the par. bits positions. It has enough space to store the max. num. of
 * parity bits for the max. data size case (25 bytes, 9 par. bits). */
#define PAR_BITS_POS_ARRAY_SIZE 16
static uint8_t par_bits_positions_array_g[PAR_BITS_POS_ARRAY_SIZE];

/* ============================================================================================== */

static void _clear_array(uint8_t* array, uint8_t array_size)
{
    for (uint8_t k = 0; k < array_size; k += 1)
    {
        array[k] = 0;
    }
}

/* ============================================================================================== */

static void _copy_array(const uint8_t* source_array, uint8_t* dest_array, uint8_t array_size)
{
    for (uint8_t k = 0; k < array_size; k += 1)
    {
        dest_array[k] = source_array[k];
    }
}

/* ============================================================================================== */

static uint8_t _round_to_mult_of_8(uint8_t value)
{
    if (value % 8 == 0)
    {
        return value;
    }
    return (value + 7) & ~0x07;
}

/* ============================================================================================== */

static uint8_t _calc_n_par_bits(uint8_t data_size_bits)
{
    uint8_t r = 0;
    /* Calc. needed number of hamming parity bits */
    while ((1U << r) < (r + data_size_bits + 1))
    {
        r += 1;
    }
    return r + 1; /* Add one for global parity bit (on position 0) */
}

/* ============================================================================================== */

static bool _check_global_even_parity(const uint8_t* array, uint8_t array_size)
{
    uint8_t accumulator = 0;
    for (uint8_t k = 0; k < array_size; k += 1)
    {
        accumulator ^= array[k];
    }
    return accumulator == 0;
}

/* ============================================================================================== */

static bool _get_bit_from_array(const uint8_t* bit_array, uint8_t bit_position)
{
    return bit_array[bit_position] != 0;
}

/* ============================================================================================== */

static bool _get_bit_from_byte(uint8_t byte, uint8_t bit_index)
{
    return ((byte >> bit_index) & 0x01);
}

/* ============================================================================================== */

static void _set_bit_on_byte(uint8_t* byte, uint8_t bit_index, bool bit_state)
{
    if (bit_index > 7)
    {
        return;
    }
    if (bit_state)
        *byte |= (1U << bit_index);
    else
        *byte &= ~(1U << bit_index);
}

/* ============================================================================================== */

static uint8_t _find_par_bit_positions(uint8_t n_par_bits, uint8_t* par_bits_positions_array)
{
    for (uint8_t i = 0; i < n_par_bits; ++i)
    {
        par_bits_positions_array[i] = (i == 0) ? 0 : (1U << (i - 1));
    }
    return n_par_bits;
}

/* ============================================================================================== */

static bool _is_parity_bit_position(const uint8_t* par_bits_positions_array,
                                    uint8_t        n_par_bits,
                                    uint8_t        index)
{
    for (uint8_t j = 0; j < n_par_bits; ++j)
    {
        if (index == par_bits_positions_array[j])
        {
            return true;
        }
    }
    return false;
}

/* ============================================================================================== */

/**
 * @brief Converts a raw data byte array into a bit array, placing data bits in non-parity positions
 * according to Hamming code conventions. The resulting bit array is ready for encoding. This
 * function takes an input array of data bytes and fills a bit array, skipping positions reserved
 * for parity bits as specified by the Hamming code. Data bits are placed in the remaining
 * positions, starting from the least significant bit of the last data byte.
 * @param data                Pointer to the input data byte array.
 * @param data_size           Number of bytes in the input data array.
 * @param bit_array           Pointer to the output bit array.
 * @param bit_array_size      Size (in bits) of the output bit array.
 * @param n_par_bits          Number of parity bits (including global parity).
 * @param par_bits_pos_array  Array containing the positions of the parity bits.
 * @return uint8_t            Total number of data bits written to the bit array.
 */
static uint8_t _parse_data_to_bit_array(const uint8_t* data,
                                        uint8_t        data_size,
                                        uint8_t*       bit_array,
                                        uint8_t        bit_array_size,
                                        uint8_t        n_par_bits,
                                        const uint8_t* par_bits_pos_array)
{
    if (data_size * 8 > (bit_array_size + n_par_bits))
    {
        return 0;
    }
    uint8_t total_bits_written = 0;
    int16_t bit_array_idx      = bit_array_size - 1;
    for (int8_t byte = data_size - 1; byte >= 0; byte--)
    {
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            /* Search the next available no-parity index */
            while (bit_array_idx >= 0
                   && _is_parity_bit_position(par_bits_pos_array, n_par_bits, bit_array_idx))
            {
                bit_array_idx -= 1;
            }
            if (bit_array_idx < 0)
            {
                /* No more space on out buffer, return */
                break;
            }
            bit_array[bit_array_idx] = _get_bit_from_byte(data[byte], bit);
            bit_array_idx -= 1;
            total_bits_written += 1;
        }
    }
    return total_bits_written;
}

/* ============================================================================================== */

/**
 * @brief Converts an encoded byte array (Hamming codeword) into a bit array for decoding. This
 * function takes an array of bytes representing a Hamming-encoded word and parses it into a bit
 * array, where each element represents a single bit of the encoded word. The most significant bit
 * of the first byte is placed at bit_array[0], and the least significant bit of the last byte is
 * placed at bit_array[bit_array_size-1].
 * @param encoded_data     Pointer to the input encoded byte array.
 * @param enc_data_size    Size (in bytes) of the encoded_data array.
 * @param bit_array        Pointer to the output bit array.
 * @param bit_array_size   Size (in bits) of the output bit array.
 * @return uint8_t         Number of bits written to the bit array.
 */
static uint8_t _parse_encoded_word_to_bit_array(const uint8_t* encoded_word,
                                                uint8_t        enc_word_size,
                                                uint8_t*       bit_array,
                                                uint8_t        bit_array_size)
{
    uint8_t total_bits_written = 0;
    int16_t bit_array_idx      = bit_array_size - 1;
    for (int8_t byte = enc_word_size - 1; byte >= 0; byte--)
    {
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            bit_array[bit_array_idx] = _get_bit_from_byte(encoded_word[byte], bit);
            bit_array_idx -= 1;
            total_bits_written += 1;
            if (bit_array_idx < 0)
            {
                /* No more space on out buffer, return */
                return total_bits_written;
            }
        }
    }
    return total_bits_written;
}

/* ============================================================================================== */

/**
 * @brief Converts a decoded bit array (data + parity bits, already corrected if needed) into a byte
 * array. This function takes a bit array containing both data and parity bits (already decoded and
 * corrected if necessary) and parses it into an output byte array. The least significant bit of the
 * bit array (decoded_array[N-1]) is placed in the least significant bit of the last byte of the
 * output array, proceeding towards the most significant bit.
 * @param decoded_array Input bit array containing data and parity bits.
 * @param dec_bit_array_size Size of the input bit array.
 * @param data Output byte array to be written.
 * @param data_size Size of the output byte array.
 * @param n_par_bits Number of parity bits in the bit array.
 * @param par_bits_pos_array Array containing the positions of the parity bits.
 * @return uint8_t Number of bytes written to the output data array.
 */
static uint8_t _parse_decoded_bit_array_to_data(const uint8_t* dec_bit_array,
                                                uint8_t        dec_bit_array_size,
                                                uint8_t*       data,
                                                uint8_t        data_size,
                                                uint8_t        n_par_bits,
                                                const uint8_t* par_bits_pos_array)
{
    int8_t  data_byte_idx = data_size - 1;
    uint8_t data_bit_idx  = 0;
    for (int16_t bit = dec_bit_array_size - 1; bit >= 0; bit -= 1)
    {
        if (!_is_parity_bit_position(par_bits_pos_array, n_par_bits, bit))
        {
            _set_bit_on_byte(&data[data_byte_idx], data_bit_idx, dec_bit_array[bit]);
            data_bit_idx += 1;
            if (data_bit_idx >= 8)
            {
                data_bit_idx = 0;
                data_byte_idx -= 1;
            }
            if (data_byte_idx < 0)
            {
                break;
            }
        }
    }
    return data_size;
}

/* ============================================================================================== */

/**
 * @brief Parses an encoded bit array into a word (byte array).
 * @param encoded_array Pointer to the input encoded bit array.
 * @param enc_bit_array_size Size of the encoded bit array in bits.
 * @param word Pointer to the output word (byte array) where the parsed bits will be stored.
 * @param enc_word_size Size of the output word in bytes.
 * @return uint8_t Returns the size of the word in bytes, or 0 if the encoded array size exceeds the
 * maximum word size.
 */
static uint8_t _parse_encoded_bit_array_to_word(const uint8_t* encoded_array,
                                                uint8_t        enc_bit_array_size,
                                                uint8_t*       word,
                                                uint8_t        enc_word_size)
{
    int8_t  word_byte_idx     = enc_word_size - 1;
    uint8_t word_byte_bit_idx = 0; /* Index for each bit inside each byte of the word */
    for (int16_t bit = enc_bit_array_size - 1; bit >= 0; bit -= 1)
    {
        _set_bit_on_byte(&word[word_byte_idx], word_byte_bit_idx, encoded_array[bit]);
        word_byte_bit_idx += 1;
        if (word_byte_bit_idx >= 8)
        {
            word_byte_bit_idx = 0;
            word_byte_idx -= 1;
        }
        if (word_byte_idx < 0)
        {
            break;
        }
    }
    return enc_word_size; /* Size of word */
}

/* ============================================================================================== */

/**
 * @brief Encodes a bit array according to the Hamming code convention.
 * This function takes an input bit array representing the original data and encodes it using the
 * Hamming code algorithm. The encoded bit array will include the necessary parity bits as per the
 * selected Hamming convention.
 * @param data_size Size of the original data in bytes.
 * @param bit_array Pointer to the input bit array containing the data bits.
 * @param bit_array_size Size of the input bit array.
 * @param encoded_bit_array Pointer to the output bit array where the encoded bits will be stored.
 *        This array must be large enough to hold the data bits plus the required parity bits.
 * @param enc_bit_array_size Size of the output encoded bit array (data bits + parity bits).
 */
static void _hamming_encode_bit_array(uint8_t        data_size,
                                      const uint8_t* bit_array,
                                      uint8_t        bit_array_size,
                                      uint8_t*       encoded_bit_array,
                                      uint8_t        enc_bit_array_size,
                                      uint8_t        n_par_bits,
                                      const uint8_t* par_bits_positions_array)
{
    if (bit_array_size != enc_bit_array_size)
    {
        return;
    }
    uint8_t accumulator = 0;
    _clear_array(encoded_bit_array, enc_bit_array_size);
    _copy_array(bit_array, encoded_bit_array, bit_array_size);
    for (uint8_t par_bit_index = 1; par_bit_index < n_par_bits;
         par_bit_index += 1) /* Start from 1 to avoid global par. bit for now */
    {
        for (uint8_t word_bit_index = 0; word_bit_index < enc_bit_array_size; word_bit_index += 1)
        {
            if (word_bit_index == par_bits_positions_array[par_bit_index])
            {
                continue;
            }
            if ((word_bit_index & par_bits_positions_array[par_bit_index])
                && _get_bit_from_array(bit_array, word_bit_index))
            {
                accumulator ^= 1;
            }
        }
        encoded_bit_array[par_bits_positions_array[par_bit_index]] = accumulator;
        accumulator                                                = 0;
    }
    if (!_check_global_even_parity(encoded_bit_array, enc_bit_array_size))
    {
        encoded_bit_array[0] = 1; /* Add global parity */
    }
}

/* ============================================================================================== */

/**
 * @brief Decodes a Hamming encoded bit array into its original data bits. This function takes an
 * array of bits that have been encoded using the Hamming code, decodes it, and stores the resulting
 * data bits in the provided output array. It can also detect and correct single-bit errors in the
 * encoded data.
 * @param encoded_bit_array Pointer to the input array containing the Hamming encoded bits.
 * @param enc_bit_array_size The size (number of bits) of the encoded_bit_array.
 * @param decoded_bit_array Pointer to the output array where the decoded data bits will be stored.
 * @return true if decoding was successful and any single-bit errors were corrected;
 *         false if an uncorrectable error was detected.
 */
static bool _hamming_decode_bit_array(const uint8_t* encoded_bit_array,
                                      uint8_t        enc_bit_array_size,
                                      uint8_t*       decoded_bit_array,
                                      uint8_t        dec_bit_array_size)
{
    uint8_t accumulator = 0;
    if (dec_bit_array_size != enc_bit_array_size)
    {
        return 0;
    }
    for (uint8_t bit_index = 0; bit_index < enc_bit_array_size; bit_index += 1)
    {
        decoded_bit_array[bit_index] = encoded_bit_array[bit_index];
        if (encoded_bit_array[bit_index])
        {
            accumulator ^= bit_index;
        }
    }
    if (accumulator != 0 && accumulator < enc_bit_array_size)
    {
        decoded_bit_array[accumulator] ^= 1;
    }
    /* Check global parity */
    if (_check_global_even_parity(decoded_bit_array, dec_bit_array_size))
    {
        return true; /* 0 errors or 1 error detected & fixed */
    }
    return false; /* 2 or more errors detected */
}

/* ============================================================================================== */

/* The max. data size is limited by buffers size and uint8_t size used for implementation. For 25
 * bytes of data there are 9 parity bits, thus 30*8 + 9 = 241 bits, can be rounded to 248 bits or 31
 * bytes. Anyway, Hamming is not recommended on large data blocks due to its incapacity to fix more
 * than one bit error (more probable as data size increases), so 25 bytes is more than enough. */
static const uint8_t MAX_DATA_SIZE = 25;

/* Global arrays for bit handling */
#define GLOBAL_BIT_ARRAY_SIZE 255
static uint8_t bit_array_g[GLOBAL_BIT_ARRAY_SIZE];
static uint8_t encoded_bit_array_g[GLOBAL_BIT_ARRAY_SIZE];
static uint8_t decoded_bit_array_g[GLOBAL_BIT_ARRAY_SIZE];

/* Data size. Used for calculating the size of the bit array during encoding/decoding. */
static uint8_t data_size_g = 0;

/* ============================================================================================== */

/**
 * @brief Encodes input data using the Hamming code for error detection and correction. This
 * function takes a buffer of input data and encodes it using the Hamming code algorithm, storing
 * the result in the provided encoded_data buffer. The function ensures that the encoded data does
 * not exceed the specified maximum size.
 * @param data Pointer to the input data buffer to be encoded.
 * @param data_size Size of the input data buffer in bytes.
 * @param encoded_data Pointer to the buffer where the encoded data will be stored.
 * @param enc_data_max_size Maximum size of the encoded_data buffer in bytes.
 * @return uint8_t Returns the number of bytes written to the encoded_data buffer. Returns 0 in case
 * any of the params. are out of range.
 */
static uint8_t hamming_encode_bytestream(const uint8_t* data,
                                         uint8_t        data_size,
                                         uint8_t*       encoded_data,
                                         uint8_t        enc_data_max_size)
{
    if (data_size > MAX_DATA_SIZE)
    {
        data_size = MAX_DATA_SIZE; /* Limit data size to max. allowed */
    }
    data_size_g
        = data_size; /* Store for Rx and Tx to know data size and thus N total bits & N par. bits */
    uint8_t data_size_bits = data_size_g * 8;
    uint8_t n_par_bits     = _calc_n_par_bits(data_size_bits);
    uint8_t bit_array_size = n_par_bits + data_size_bits;
    if (bit_array_size > GLOBAL_BIT_ARRAY_SIZE)
    {
        return 0;
    }
    uint8_t enc_data_size = _round_to_mult_of_8(bit_array_size) / 8;
    if (enc_data_size > enc_data_max_size)
    {
        return 0;
    }
    _clear_array(bit_array_g, bit_array_size);
    _clear_array(encoded_bit_array_g, bit_array_size);
    _clear_array(encoded_data, enc_data_max_size);
    _clear_array(par_bits_positions_array_g, n_par_bits);
    _find_par_bit_positions(n_par_bits, par_bits_positions_array_g);
    _parse_data_to_bit_array(data, data_size, bit_array_g, bit_array_size, n_par_bits,
                             par_bits_positions_array_g);
    _hamming_encode_bit_array(data_size, bit_array_g, bit_array_size, encoded_bit_array_g,
                              bit_array_size, n_par_bits, par_bits_positions_array_g);
    uint8_t size = _parse_encoded_bit_array_to_word(encoded_bit_array_g, bit_array_size,
                                                    encoded_data, enc_data_size);
    return size;
}

/* ============================================================================================== */

/**
 * @brief Decodes data encoded with the Hamming code. This function takes an array of encoded data,
 * applies Hamming code error correction, and writes the decoded data to the provided output buffer.
 * It returns the number of bytes successfully decoded.
 * @param encoded_data Pointer to the input buffer containing Hamming-encoded data.
 * @param enc_data_size Size (in bytes) of the encoded data buffer.
 * @param decoded_data Pointer to the output buffer where the decoded data will be stored.
 * @param dec_data_max_size Maximum size (in bytes) of the decoded data buffer.
 * @return uint8_t Number of bytes written to the decoded_data buffer.
 */
static uint8_t hamming_decode_bytestream(const uint8_t* encoded_data,
                                         uint8_t        enc_data_size,
                                         uint8_t*       decoded_data,
                                         uint8_t        dec_data_max_size)
{
    uint8_t data_size_bits = data_size_g * 8;
    uint8_t n_par_bits     = _calc_n_par_bits(data_size_bits);
    uint8_t bit_array_size = n_par_bits + data_size_bits;
    uint8_t dec_data_size  = data_size_g;
    if (bit_array_size > GLOBAL_BIT_ARRAY_SIZE)
    {
        return 0;
    }
    if (dec_data_size > dec_data_max_size)
    {
        return 0;
    }
    _clear_array(bit_array_g, bit_array_size);
    _clear_array(decoded_bit_array_g, bit_array_size);
    _clear_array(decoded_data, dec_data_max_size);
    _clear_array(par_bits_positions_array_g, n_par_bits);
    _parse_encoded_word_to_bit_array(encoded_data, enc_data_size, bit_array_g, bit_array_size);
    _hamming_decode_bit_array(bit_array_g, bit_array_size, decoded_bit_array_g, bit_array_size);
    _find_par_bit_positions(n_par_bits, par_bits_positions_array_g);
    uint8_t size
        = _parse_decoded_bit_array_to_data(decoded_bit_array_g, bit_array_size, decoded_data,
                                           dec_data_size, n_par_bits, par_bits_positions_array_g);
    return size;
}

/* ============================================================================================== */

static uint16_t hamming_encode_word(uint8_t word)
{
    uint8_t data[] = {word};
    uint8_t encoded_data[2];
    hamming_encode_bytestream(data, sizeof(data), encoded_data, sizeof(encoded_data));
    return ((uint16_t)encoded_data[0] << 8) | ((uint16_t)encoded_data[1]);
}

/* ============================================================================================== */

static uint8_t hamming_decode_word(uint16_t word)
{
    uint8_t encoded_data[]                     = {(uint8_t)(word >> 8), (uint8_t)word};
    uint8_t decoded_data[sizeof(encoded_data)] = {0};
    hamming_decode_bytestream(encoded_data, sizeof(encoded_data), decoded_data,
                              sizeof(decoded_data));
    return (uint8_t)decoded_data[0];
}

/* ============================================================================================== */

static uint8_t calc_enc_data_size(uint8_t data_size)
{
    if (data_size > MAX_DATA_SIZE)
    {
        return 0;
    }
    uint8_t data_bits  = data_size * 8;
    uint8_t n_par_bits = _calc_n_par_bits(data_bits);
    return _round_to_mult_of_8(data_bits + n_par_bits) / 8;
}

/* ============================================================================================== */

hamming_codec_t hamming_create(void)
{
    hamming_codec_t encoder;
    encoder.encode_bytestream  = hamming_encode_bytestream;
    encoder.decode_bytestream  = hamming_decode_bytestream;
    encoder.encode_word        = hamming_encode_word;
    encoder.decode_word        = hamming_decode_word;
    encoder.calc_enc_data_size = calc_enc_data_size;
    return encoder;
}

/* ============================================================================================== */
