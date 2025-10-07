#include "../inc/esp8266ex.h"

/* ============================================================================================== */

#include <string.h>
#include <stdio.h>

/* ============================================================================================== */

static void _esp8266ex_send_command(struct esp8266ex* self, const char* command)
{
    if (self == NULL || self->is_initialized == false || command == NULL)
    {
        return;
    }

    // Send the AT command via UART
    self->uart->ops->transmit(self->uart, (const uint8_t*)command, strlen(command));

    // Send carriage return and line feed
    self->uart->ops->transmit(self->uart, (const uint8_t*)"\r\n", 2);
}

/* ============================================================================================== */

int8_t esp8266ex_initialize(struct esp8266ex* self)
{
    if (self == NULL || self->uart == NULL || self->uart->ops == NULL)
    {
        return -EFAULT;
    }

    // Test ESP8266EX communication with AT command
    _esp8266ex_send_command(self, "AT");

    // Reset the module
    _esp8266ex_send_command(self, "AT+RST");

    // Set WiFi mode to station mode
    _esp8266ex_send_command(self, "AT+CWMODE=1");

    // Initialize struct fields
    memset(self->ssid, 0, sizeof(self->ssid));
    memset(self->password, 0, sizeof(self->password));
    memset(self->ipv4_address, 0, sizeof(self->ipv4_address));
    self->is_initialized = true;
    self->is_connected   = false;
    return 0;
}

/* ============================================================================================== */
int8_t esp8266ex_connect(struct esp8266ex* self, const char* ssid, const char* password)
{
    if (self == NULL || ssid == NULL || password == NULL)
    {
        return -EFAULT;
    }

    if (!self->is_initialized)
    {
        return -ENOENT;
    }

    // Store SSID and password
    strncpy(self->ssid, ssid, sizeof(self->ssid) - 1);
    self->ssid[sizeof(self->ssid) - 1] = '\0';

    strncpy(self->password, password, sizeof(self->password) - 1);
    self->password[sizeof(self->password) - 1] = '\0';

    // Create connect command
    char connect_cmd[128];
    snprintf(connect_cmd, sizeof(connect_cmd), "AT+CWJAP=\"%s\",\"%s\"", ssid, password);

    // Send connect command
    _esp8266ex_send_command(self, connect_cmd);

    // Get IP address
    _esp8266ex_send_command(self, "AT+CIFSR");

    self->is_connected = true;

    return 0;
}

/* ============================================================================================== */

int8_t esp8266ex_disconnect(struct esp8266ex* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    if (!self->is_initialized)
    {
        return -ENOENT;
    }

    // Disconnect from WiFi
    _esp8266ex_send_command(self, "AT+CWQAP");

    // Clear connection info
    memset(self->ssid, 0, sizeof(self->ssid));
    memset(self->password, 0, sizeof(self->password));
    memset(self->ipv4_address, 0, sizeof(self->ipv4_address));
    self->is_connected = false;

    return 0;
}

/* ============================================================================================== */

int8_t esp8266ex_send_data(struct esp8266ex* self, const uint8_t* data, size_t length)
{
    if (self == NULL || data == NULL || length == 0)
    {
        return -EFAULT;
    }

    if (!self->is_connected)
    {
        return -ENOENT;
    }

    // Prepare data length command
    char length_cmd[32];
    snprintf(length_cmd, sizeof(length_cmd), "AT+CIPSEND=%zu", length);

    // Send data length command
    _esp8266ex_send_command(self, length_cmd);

    // Send actual data (without AT command formatting)
    self->uart->ops->transmit(self->uart, data, length);

    return 0;
}

/* ============================================================================================== */
