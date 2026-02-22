#ifndef ADC_H
#define ADC_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

/**
 * @brief Macro to define and initialize an ADC instance.
 * @param name Variable name for the ADC instance.
 * @param ops_ptr Pointer to the ADC operations structure.
 * @param port ADC port identifier.
 * @param res ADC resolution in bits.
 * @param sampling Sampling time in ADC clock cycles.
 * @param volt_reference Voltage reference selection.
 */
#define ADC_DEFINE(name, ops_ptr, port, res, sampling, volt_ref) \
    struct adc name = {                                          \
        .ops              = (ops_ptr),                           \
        .port_id          = (port),                              \
        .resolution       = (res),                               \
        .sampling_time    = (sampling),                          \
        .volt_reference   = (volt_ref),                          \
        .was_initialized  = false,                               \
        .callback         = NULL,                                \
        .callback_context = NULL,                                \
    }

/* ========================================================================== */

/**
 * @brief ADC voltage reference options.
 */
enum adc_vref
{
    ADC_VREF_VDD      = 0, /**< VDD as positive reference */
    ADC_VREF_EXTERNAL = 1, /**< External voltage reference */
    ADC_VREF_FVR_1024 = 2, /**< Fixed Voltage Reference 1.024V */
    ADC_VREF_FVR_2048 = 3, /**< Fixed Voltage Reference 2.048V */
    ADC_VREF_FVR_4096 = 4, /**< Fixed Voltage Reference 4.096V */
};

/* ========================================================================== */

/**
 * @brief Prototype for the ADC conversion callback function.
 * @param callback_context Pointer to user-defined callback_context data (can be
 * NULL).
 * @param data The converted ADC data.
 */
typedef void (*adc_conversion_callback_t)(void* callback_context, uint32_t data);

/* ========================================================================== */

/**
 * @brief ADC interface structure.
 */
struct adc;

/**
 * @brief ADC operations structure. This structure holds function pointers for
 * ADC operations.
 */
struct adc_ops
{
    /**
     * @brief Initializes the ADC interface.
     * @param self Pointer to the ADC port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct adc* self);

    /**
     * @brief Reads a single ADC value from the specified channel (blocking).
     * @param self Pointer to the ADC port structure.
     * @param channel ADC channel to read from.
     * @param value Pointer to store the converted value.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*read)(struct adc* self, uint8_t channel, uint32_t* value);

    /**
     * @brief Starts ADC conversion on the specified channel.
     * @param self Pointer to the ADC port structure.
     * @param channel ADC channel to convert.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*start_conversion)(struct adc* self, uint8_t channel);

    /**
     * @brief Stops ongoing ADC conversion.
     * @param self Pointer to the ADC port structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*stop_conversion)(struct adc* self);

    /**
     * @brief Checks if ADC conversion is complete.
     * @param self Pointer to the ADC port structure.
     * @param complete Pointer to store completion status.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*is_conversion_complete)(struct adc* self, bool* complete);

    /**
     * @brief Gets the last converted ADC value without blocking.
     * @param self Pointer to the ADC port structure.
     * @param value Pointer to store the converted value.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*get_value)(struct adc* self, uint32_t* value);

    /**
     * @brief Sets the callback function for conversion complete events.
     * @param self Pointer to the ADC port structure.
     * @param callback Pointer to the callback function.
     * @param callback_context Pointer to user-defined context data.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*set_conversion_callback)(
        struct adc*               self,
        adc_conversion_callback_t callback,
        void*                     callback_context);

    /**
     * @brief Enables or disables ADC conversion complete interrupt.
     * @param self Pointer to the ADC port structure.
     * @param enable true to enable, false to disable.
     */
    void (*enable_conv_interrupt)(struct adc* self, bool enable);
};

/* ========================================================================== */

struct adc
{
    /**
     * @brief ADC port identifier (e.g., 0 for adc1, 1 for adc2, etc.).
     */
    uint8_t port_id;

    /**
     * @brief ADC resolution in bits (8, 10, 12, 16, or 32).
     */
    uint8_t resolution;

    /**
     * @brief Sampling time in ADC clock cycles (platform-specific).
     */
    uint16_t sampling_time;

    /**
     * @brief Voltage reference selection.
     */
    uint8_t volt_reference;

    /**
     * @brief Pointer to the ADC operations structure.
     */
    const struct adc_ops* const ops;

    /* private: internal state - do not access directly */
    bool                      was_initialized;
    adc_conversion_callback_t callback;
    void*                     callback_context;
};

/* ========================================================================== */

#endif  // ADC_H
