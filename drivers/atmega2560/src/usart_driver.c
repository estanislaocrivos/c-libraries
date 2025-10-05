#include "../inc/usart_driver.h"

/* ============================================================================================== */

static void* usart0_callback_context;

static serial_rx_callback_t usart0_callback;

/* ============================================================================================== */

static inline void usart0_initialize(struct serial_port* port)
{
    uint16_t ubrr = (F_CPU / (16UL * port->baud_rate)) - 1;
    if (ubrr > 4095)
    {
        return;
    }
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (uint8_t)((1 << RXEN0) | (1 << TXEN0));
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

static inline void usart1_initialize(struct serial_port* port)
{
    uint16_t ubrr = (F_CPU / (16UL * port->baud_rate)) - 1;
    if (ubrr > 4095)
    {
        return;
    }
    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)ubrr;
    UCSR1B = (uint8_t)((1 << RXEN1) | (1 << TXEN1));
    UCSR1C = (1 << USBS1) | (3 << UCSZ10);
}

int8_t usart_initialize(struct serial_port* port)
{
    if (port == NULL || port->baud_rate == 0)
    {
        return -EINVAL;
    }
    switch (port->port_id)
    {
        case 0:
            usart0_initialize(port);
            break;

        case 1:
            usart1_initialize(port);
            break;

        default:
            break;
    }
    return 0;
}

static inline void _usart0_transmit_byte(const uint8_t* byte)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }
    UDR0 = *byte;
}

int8_t usart_transmit(struct serial_port* port, const uint8_t* buffer, size_t size)
{
    if (buffer == NULL || size == 0)
    {
        return -EINVAL;
    }
    for (size_t i = 0; i < size; i++)
    {
        _usart0_transmit_byte(buffer + i);
    }
    return 0;
}

static inline uint8_t _usart0_receive_byte(void)
{
    while (!(UCSR0A & (1 << RXC0)))
    {
    }
    return UDR0;
}

int8_t usart_receive(struct serial_port* port, uint8_t* buffer, size_t size)
{
    if (buffer == NULL || size == 0)
    {
        return -EINVAL;
    }
    for (size_t i = 0; i < size; i++)
    {
        buffer[i] = _usart0_receive_byte();
    }
    return 0;
}

int8_t usart_set_rx_callback(struct serial_port*  port,
                             serial_rx_callback_t callback,
                             void*                callback_context)
{
    if (port->port_id == 0)
    {
        usart0_callback         = callback;
        usart0_callback_context = callback_context;
    }
    return 0;
}

void usart0_clear_buffers(void)
{
}

/* ============================================================================================== */

static usart_rx_callback_t usart1_rx_callback = NULL;

static inline void _usart1_transmit_byte(const uint8_t* byte)
{
    while (!(UCSR1A & (1 << UDRE1)))
    {
    }
    UDR1 = *byte;
}

int8_t usart1_transmit(const uint8_t* buffer, size_t length)
{
    if (buffer == NULL || length == 0)
    {
        return -EINVAL;
    }
    for (size_t i = 0; i < length; i++)
    {
        _usart1_transmit_byte(buffer + i);
    }
    return 0;
}

static inline uint8_t _usart1_receive_byte(void)
{
    while (!(UCSR1A & (1 << RXC1)))
    {
    }
    return UDR1;
}

int8_t usart1_receive(uint8_t* buffer, size_t length)
{
    if (buffer == NULL || length == 0)
    {
        return -EINVAL;
    }
    for (size_t i = 0; i < length; i++)
    {
        buffer[i] = _usart1_receive_byte();
    }
    return 0;
}

int8_t usart1_set_rx_callback(usart_rx_callback_t callback)
{
    usart1_rx_callback = callback;
    return 0;
}

void usart1_clear_buffers(void)
{
}

/* ============================================================================================== */
