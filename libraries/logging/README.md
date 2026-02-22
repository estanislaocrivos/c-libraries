# Logging Utility

A simple C library to log messages for debugging and monitoring purposes through a serial interface.

## Usage Example

The log object is of type singleton, so only one instance can be created.

```c
#include "serial.h"
#include "platform.h"
#include "logging.h"

void main(void)
{
    /* 1. Create serial object for logging */
    SERIAL_DEFINE(uart_log, PLATFORM_SERIAL_OPS, 115200, 8, 'N', 1);

    /* 2. Initialize serial interface */
    uart_log.ops->initialize(&uart_log);

    /* 3. Configure and initialize log */
    struct log_config log_cfg = {
        .output     = &uart_log,
        .min_level  = LOG_LEVEL_DEBUG,  // Runtime: show all
        .show_level = true              // Show prefixes [INFO], etc.
    };
    log_init(&log_cfg);

    /* 4. Use! */
    LOG_DEBUG("Sistema iniciando...");
    LOG_INFO("Configuración cargada");
    LOG_WARN("Batería baja");
    LOG_ERROR("Fallo en sensor");
}
```
