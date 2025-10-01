#ifndef ESP8266EX_H
#define ESP8266EX_H

/* ============================================================================================== */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ============================================================================================== */

#include "../../embedded-hal/inc/embedded_hal.h"
#include "../../inc/errno.h"

/* ============================================================================================== */

struct esp8266ex
{
    struct uart_port* uart;
    char              ssid[32];
    char              password[64];
    char              ipv4_address[16];
    bool              is_initialized;
    bool              is_connected;
};

/* ============================================================================================== */

int8_t esp8266ex_initialize(struct esp8266ex* self);

/* ============================================================================================== */

int8_t esp8266ex_connect(struct esp8266ex* self, const char* ssid, const char* password);

/* ============================================================================================== */

int8_t esp8266ex_disconnect(struct esp8266ex* self);

/* ============================================================================================== */

int8_t esp8266ex_send_data(struct esp8266ex* self, const uint8_t* data, size_t length);

/* ============================================================================================== */

#endif  // ESP8266EX_H
