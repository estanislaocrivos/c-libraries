#ifndef LOG_H
#define LOG_H

/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* ========================================================================== */

#include "../../embedded-hal/inc/serial.h"

/* ========================================================================== */

/**
 * @brief Log levels in order of severity.
 *
 * Messages are filtered based on the configured minimum level.
 * Only messages with level >= min_level will be output.
 */
enum log_level
{
    LOG_LEVEL_DEBUG = 0, /**< Detailed debug information */
    LOG_LEVEL_INFO  = 1, /**< General information */
    LOG_LEVEL_WARN  = 2, /**< Warning conditions */
    LOG_LEVEL_ERROR = 3, /**< Error conditions */
    LOG_LEVEL_FATAL = 4, /**< Fatal/critical errors */
    LOG_LEVEL_NONE  = 5  /**< Disable all logging */
};

enum value_type
{
    LOG_VALUE_UINT8,
    LOG_VALUE_UINT16,
    LOG_VALUE_UINT32,
    LOG_VALUE_INT8,
    LOG_VALUE_INT16,
    LOG_VALUE_INT32,
};

/* ========================================================================== */

/**
 * struct log_config - Log utility configuration
 * @serial_output: Serial port for log output (required)
 * @min_level: Minimum level to output (runtime filter)
 * @show_level: Show level prefix [DEBUG], [INFO], etc.
 *
 * A log utility is useful for logging messages over a serial interface. It
 * allows to see insights during system operation. All fields must be set before
 * calling log_init().
 */
struct log_config
{
    /* public: user-configurable fields - set before init (const after init) */
    const struct serial* serial_output;
    enum log_level       min_level;
    bool                 show_level;
};

/* ========================================================================== */

/**
 * @brief Initialize the log module.
 * @param config Pointer to log configuration.
 * @return 0 on success, -EFAULT if config or output is NULL.
 */
int8_t log_init(const struct log_config* config);

/**
 * @brief Set minimum log level at runtime.
 * @param level New minimum level.
 * @return 0 on success, -EPERM if log not initialized.
 */
int8_t log_set_level(enum log_level level);

/**
 * @brief Get current minimum log level.
 * @param level Pointer to store current level.
 * @return 0 on success, -EFAULT if level is NULL, -EPERM if not initialized.
 */
int8_t log_get_level(enum log_level* level);

/**
 * @brief Enable or disable level prefix in output.
 * @param enable true to show prefix, false to hide.
 * @return 0 on success, -EPERM if not initialized.
 */
int8_t log_show_level(bool enable);

/* ========================================================================== */

/**
 * @brief Log a message at the specified level.
 * @param level Log level for this message.
 * @param msg Null-terminated message string.
 * @return 0 on success, -EFAULT if msg is NULL, -EPERM if not initialized.
 *
 * @note Prefer using the level-specific functions (log_debug, log_info, etc.)
 *       or macros (LOG_DEBUG, LOG_INFO, etc.) instead of calling this directly.
 */
int8_t log_write(enum log_level level, const char* msg);

/**
 * @brief Log a debug message.
 * @param msg Null-terminated message string.
 * @return 0 on success, negative errno on error.
 */
int8_t log_debug(const char* msg);

/**
 * @brief Log an info message.
 * @param msg Null-terminated message string.
 * @return 0 on success, negative errno on error.
 */
int8_t log_info(const char* msg);

/**
 * @brief Log a warning message.
 * @param msg Null-terminated message string.
 * @return 0 on success, negative errno on error.
 */
int8_t log_warn(const char* msg);

/**
 * @brief Log an error message.
 * @param msg Null-terminated message string.
 * @return 0 on success, negative errno on error.
 */
int8_t log_error(const char* msg);

/**
 * @brief Log a fatal message.
 * @param msg Null-terminated message string.
 * @return 0 on success, negative errno on error.
 */
int8_t log_fatal(const char* msg);

/**
 * @brief Log a formatted message with a single value.
 * @param msg Format string (e.g. "Value: %d") - must contain a single format
 * specifier for the value.
 * @param value Pointer to the value to log (e.g. pointer to int, float, etc.).
 * The function will format the value according to the format specifier in msg.
 * @return 0 on success, negative errno on error.
 */
int8_t log_value(const char* msg, void* value, enum value_type type);

/* ========================================================================== */

/*
 * Compile-time log level filtering.
 *
 * Define LOG_COMPILE_LEVEL before including this header to strip
 * log calls below that level from the binary (saves ROM).
 *
 * Example:
 *   #define LOG_COMPILE_LEVEL LOG_LEVEL_WARN  // Only WARN, ERROR, FATAL
 * compiled #include "logging.h"
 *
 * Or define via compiler flag:
 *   -DLOG_COMPILE_LEVEL=LOG_LEVEL_WARN
 */

#ifndef LOG_COMPILE_LEVEL
#define LOG_COMPILE_LEVEL LOG_LEVEL_DEBUG /* Default: compile all levels */
#endif

/* Compile-time filtering macros */
#if LOG_COMPILE_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(msg) log_debug(msg)
#else
#define LOG_DEBUG(msg) ((void)0)
#endif

#if LOG_COMPILE_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(msg) log_info(msg)
#else
#define LOG_INFO(msg) ((void)0)
#endif

#if LOG_COMPILE_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN(msg) log_warn(msg)
#else
#define LOG_WARN(msg) ((void)0)
#endif

#if LOG_COMPILE_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(msg) log_error(msg)
#else
#define LOG_ERROR(msg) ((void)0)
#endif

#if LOG_COMPILE_LEVEL <= LOG_LEVEL_FATAL
#define LOG_FATAL(msg) log_fatal(msg)
#else
#define LOG_FATAL(msg) ((void)0)
#endif

/* ========================================================================== */

#endif /* LOG_H */
