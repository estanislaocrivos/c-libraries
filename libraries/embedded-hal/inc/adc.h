#ifndef ADC_H
#define ADC_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../inc/errno.h"

/* ============================================================================================== */

/**
 * @brief Prototype for the ADC conversion callback function.
 * @param callback_context Pointer to user-defined callback_context data (can be NULL).
 * @param data The converted ADC data.
 */
typedef void (*adc_conversion_callback_t)(void* callback_context, uint32_t data);

/* ============================================================================================== */

/**
 * @brief ADC port structure. This structure holds the configuration and state of a ADC port.
 */
struct adc_port;

/**
 * @brief ADC operations structure. This structure holds function pointers for ADC operations.
 */
struct adc_ops
{
    /**
     * @brief Initializes the ADC interface.
     * @param self Pointer to the ADC port structure.
     * @return int8_t Returns 0 on success or -ERR on failure (see errno.h).
     */
    int8_t (*initialize)(struct adc_port* self);

    /**
     * @brief Reads a single ADC value from the specified channel (blocking).
     * @param self Pointer to the ADC port structure.
     * @param channel ADC channel to read from.
     * @param value Pointer to store the converted value.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*read)(struct adc_port* self, uint8_t channel, uint32_t* value);

    /**
     * @brief Starts ADC conversion on the specified channel.
     * @param self Pointer to the ADC port structure.
     * @param channel ADC channel to convert.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*start_conversion)(struct adc_port* self, uint8_t channel);

    /**
     * @brief Stops ongoing ADC conversion.
     * @param self Pointer to the ADC port structure.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*stop_conversion)(struct adc_port* self);

    /**
     * @brief Checks if ADC conversion is complete.
     * @param self Pointer to the ADC port structure.
     * @param complete Pointer to store completion status.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*is_conversion_complete)(struct adc_port* self, bool* complete);

    /**
     * @brief Gets the last converted ADC value without blocking.
     * @param self Pointer to the ADC port structure.
     * @param value Pointer to store the converted value.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*get_value)(struct adc_port* self, uint32_t* value);

    /**
     * @brief Sets the callback function for conversion complete events.
     * @param self Pointer to the ADC port structure.
     * @param callback Pointer to the callback function.
     * @param callback_context Pointer to user-defined context data.
     * @return int8_t Returns 0 on success or -ERR on failure.
     */
    int8_t (*set_conversion_callback)(struct adc_port*          self,
                                      adc_conversion_callback_t callback,
                                      void*                     callback_context);

    /**
     * @brief Enables or disables ADC conversion complete interrupt.
     * @param self Pointer to the ADC port structure.
     * @param enable true to enable, false to disable.
     */
    void (*enable_conv_interrupt)(struct adc_port* self, bool enable);
};

/* ============================================================================================== */

struct adc_port
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

    bool  _was_initialized;   // Internal flag to prevent reinitialization or misuse.
    void* _callback_context;  // Pointer to user-defined callback_context data (can be NULL).

    /**
     * @brief Pointer to the ADC operations structure. This structure must be first created and
     * initialized by the user. This structure must be of type 'const struct adc_ops', which
     * ensures that the function pointers cannot be modified after initialization.
     */
    const struct adc_ops* ops;
};

/* ============================================================================================== */

#endif  // ADC_H
