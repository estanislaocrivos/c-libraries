#include "../inc/logging.h"

/* ========================================================================== */

#include "../../inc/errno.h"

/* ========================================================================== */

/* PRIVATE */

/* ========================================================================== */

#ifndef LOG_MAX_BUFFER_SIZE
#define LOG_MAX_BUFFER_SIZE 128
#endif

static const struct serial* g_serial_port = NULL;
static enum log_level       g_min_level   = LOG_LEVEL_DEBUG;
static bool                 g_show_level  = true;
static bool                 g_initialized = false;

/**
 * @brief Copy string to buffer, character by character.
 *
 * This approach handles strings from both RAM and ROM (program memory),
 * making it portable across Harvard architectures like PIC18F.
 *
 * @param dest Destination buffer pointer
 * @param src Source string (can be in ROM or RAM)
 * @param max_len Maximum characters to copy
 * @return Number of characters copied
 */
static size_t _copy_string(char* dest, const char* src, size_t max_len)
{
    size_t count = 0;
    while (*src != '\0' && count < max_len)
    {
        dest[count++] = *src++;
    }
    return count;
}

/* ========================================================================== */

/* PUBLIC */

/* ========================================================================== */

int8_t log_init(const struct log_config* config)
{
    if (config == NULL || config->serial_output == NULL)
    {
        return -EFAULT;
    }
    g_serial_port = config->serial_output;
    g_min_level   = config->min_level;
    g_show_level  = config->show_level;
    g_initialized = true;
    return 0;
}

int8_t log_set_level(enum log_level level)
{
    if (!g_initialized)
    {
        return -EPERM;
    }
    g_min_level = level;
    return 0;
}

int8_t log_get_level(enum log_level* level)
{
    if (level == NULL)
    {
        return -EFAULT;
    }
    if (!g_initialized)
    {
        return -EPERM;
    }
    *level = g_min_level;
    return 0;
}

int8_t log_show_level(bool enable)
{
    if (!g_initialized)
    {
        return -EPERM;
    }

    g_show_level = enable;
    return 0;
}

int8_t log_write(enum log_level level, const char* msg)
{
    if (msg == NULL)
    {
        return -EFAULT;
    }
    if (!g_initialized)
    {
        return -EPERM;
    }
    if (level < g_min_level)
    {
        return 0; /* Filtered out, not an error */
    }

    static const char* const level_names[]
        = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

    char   buffer[LOG_MAX_BUFFER_SIZE];
    size_t pos = 0;

    /* Build level prefix if enabled: "[LEVEL] " */
    if (g_show_level && level < LOG_LEVEL_NONE)
    {
        buffer[pos++] = '[';
        pos += _copy_string(
            &buffer[pos], level_names[level], LOG_MAX_BUFFER_SIZE - pos - 1);
        buffer[pos++] = ']';
        buffer[pos++] = ' ';
    }

    /* Copy message, leaving room for \r\n\0 */
    pos += _copy_string(&buffer[pos], msg, LOG_MAX_BUFFER_SIZE - pos - 3);

    /* Append newline */
    buffer[pos++] = '\r';
    buffer[pos++] = '\n';
    buffer[pos]   = '\0';

    return g_serial_port->ops->transmit(
        g_serial_port, (const uint8_t*)buffer, pos);
}

int8_t log_debug(const char* msg)
{
    return log_write(LOG_LEVEL_DEBUG, msg);
}

int8_t log_info(const char* msg)
{
    return log_write(LOG_LEVEL_INFO, msg);
}

int8_t log_warn(const char* msg)
{
    return log_write(LOG_LEVEL_WARN, msg);
}

int8_t log_error(const char* msg)
{
    return log_write(LOG_LEVEL_ERROR, msg);
}

int8_t log_fatal(const char* msg)
{
    return log_write(LOG_LEVEL_FATAL, msg);
}

int8_t log_value(const char* msg, void* value, enum value_type type)
{
    char buffer[LOG_MAX_BUFFER_SIZE];
    switch (type)
    {
        case LOG_VALUE_UINT8:
            snprintf(buffer, sizeof(buffer), msg, *(uint8_t*)value);
            break;

        case LOG_VALUE_UINT16:
            snprintf(buffer, sizeof(buffer), msg, *(uint16_t*)value);
            break;

        case LOG_VALUE_UINT32:
            snprintf(buffer, sizeof(buffer), msg, *(uint32_t*)value);
            break;

        case LOG_VALUE_INT8:
            snprintf(buffer, sizeof(buffer), msg, *(int8_t*)value);
            break;

        case LOG_VALUE_INT16:
            snprintf(buffer, sizeof(buffer), msg, *(int16_t*)value);
            break;

        case LOG_VALUE_INT32:
            snprintf(buffer, sizeof(buffer), msg, *(int32_t*)value);
            break;

        default:
            return -EINVAL;
            break;
    }
    return log_write(LOG_LEVEL_INFO, buffer);
}

/* ========================================================================== */
