/**
 * @file
 * @brief PIC18-K42 ADCC (ADC with Computation) driver header file.
 */

/* ========================================================================== */

#ifndef PIC18_ADCC_H
#define PIC18_ADCC_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "adc.h"
#include "errno.h"
#include "pic18_k42.h"

/* ========================================================================== */

/**
 * @brief ADCC resolution in bits.
 */
#define PIC18_ADCC_RESOLUTION       12

/**
 * @brief Default acquisition time in ADC clock cycles.
 */
#define PIC18_ADCC_DEFAULT_ACQ_TIME 32

/* ========================================================================== */

/**
 * @brief ADCC computation modes (mapped 1:1 to ADCON2.ADMD).
 *
 * Use these values for the @c mode field of @c struct adc. Modes other than
 * @c BASIC compute a result over @c PIC18_ADCC_BURST_SAMPLES conversions; the
 * driver reads the filtered result register (@c ADFLTR) instead of @c ADRES.
 */
enum pic18_adcc_mode
{
    PIC18_ADCC_MODE_BASIC      = 0b000, /**< Raw single conversion in ADRES */
    PIC18_ADCC_MODE_ACCUMULATE = 0b001, /**< Sum of N samples in ADFLTR */
    PIC18_ADCC_MODE_AVERAGE    = 0b010, /**< Sum >> shift in ADFLTR */
    PIC18_ADCC_MODE_BURST_AVERAGE = 0b011, /**< N back-to-back per ADGO, avg in
                                              ADFLTR */
    PIC18_ADCC_MODE_LOW_PASS = 0b100,      /**< 1st-order IIR in ADFLTR */
};

/**
 * @brief Number of conversions per burst for averaging modes.
 *
 * Must be a power of two so the divide can be done with a right shift. If
 * changed, @c PIC18_ADCC_BURST_SHIFT must be updated to log2(N).
 */
#define PIC18_ADCC_BURST_SAMPLES 64

/**
 * @brief Accumulator right-shift = log2(@c PIC18_ADCC_BURST_SAMPLES).
 *
 * Loaded into ADCON2.ADCRS so ADFLTR = sum >> shift = average.
 */
#define PIC18_ADCC_BURST_SHIFT   6

/* ========================================================================== */

extern const struct adc_ops pic18_adcc_ops;

/* ========================================================================== */

/**
 * @brief Initializes the ADCC interface.
 * @param self Pointer to the ADC structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_adcc_initialize(struct adc* self);

/* ========================================================================== */

/**
 * @brief Reads a single ADC value from the specified channel (blocking).
 * @param self Pointer to the ADC structure.
 * @param channel ADC channel to read from (use ADPCH channel values).
 * @param value Pointer to store the converted value.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_adcc_read(struct adc* self, uint8_t channel, uint32_t* value);

/* ========================================================================== */

/**
 * @brief Starts ADC conversion on the specified channel.
 * @param self Pointer to the ADC structure.
 * @param channel ADC channel to convert.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_adcc_start_conversion(struct adc* self, uint8_t channel);

/* ========================================================================== */

/**
 * @brief Stops ongoing ADC conversion.
 * @param self Pointer to the ADC structure.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_adcc_stop_conversion(struct adc* self);

/* ========================================================================== */

/**
 * @brief Checks if ADC conversion is complete.
 * @param self Pointer to the ADC structure.
 * @param complete Pointer to store completion status.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_adcc_is_conversion_complete(struct adc* self, bool* complete);

/* ========================================================================== */

/**
 * @brief Gets the last converted ADC value without blocking.
 * @param self Pointer to the ADC structure.
 * @param value Pointer to store the converted value.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_adcc_get_value(struct adc* self, uint32_t* value);

/* ========================================================================== */

/**
 * @brief Sets the callback function for conversion complete events.
 * @param self Pointer to the ADC structure.
 * @param callback Pointer to the callback function.
 * @param callback_context Pointer to user-defined context data.
 * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
 */
int8_t pic18_adcc_set_conversion_callback(
    struct adc*               self,
    adc_conversion_callback_t callback,
    void*                     callback_context);

/* ========================================================================== */

/**
 * @brief Enables or disables ADC conversion complete interrupt.
 * @param self Pointer to the ADC structure.
 * @param enable true to enable, false to disable.
 */
void pic18_adcc_enable_conv_interrupt(struct adc* self, bool enable);

/* ========================================================================== */

#endif /* PIC18_ADCC_H */
