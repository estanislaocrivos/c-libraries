#include "../inc/esp8266ex.h"

/* ============================================================================================== */

static void _send_command(struct esp8266ex* self, const char* command)
{
    // Function to send AT command to ESP8266EX via UART
    // Implementation depends on the uart_port structure and its methods
}

/* ============================================================================================== */

int8_t esp8266ex_initialize(struct esp8266ex* self)
{
    if (self == NULL)
    {
        return -EINVAL;
    }

    // Initialization code here

    return 0;
}

/* ============================================================================================== */
int8_t esp8266ex_connect(struct esp8266ex* self, const char* ssid, const char* password)
{
    if (self == NULL || ssid == NULL || password == NULL)
    {
        return -EINVAL;
    }

    // Connection code here

    return 0;
}

/* ============================================================================================== */

int8_t esp8266ex_disconnect(struct esp8266ex* self)
{
    if (self == NULL)
    {
        return -EINVAL;
    }

    // Disconnection code here

    return 0;
}

/* ============================================================================================== */

int8_t esp8266ex_send_data(struct esp8266ex* self, const uint8_t* data, size_t length)
{
    if (self == NULL || data == NULL || length == 0)
    {
        return -EINVAL;
    }

    // Data sending code here

    return 0;
}

/* ============================================================================================== */
