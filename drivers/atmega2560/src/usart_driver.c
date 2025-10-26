#include "../inc/usart_driver.h"

/* ============================================================================================== */

static void*                usart0_callback_context;
static void*                usart1_callback_context;
static serial_rx_callback_t usart0_callback;
static serial_rx_callback_t usart1_callback;

/* ============================================================================================== */

static inline int8_t usart0_initialize(const struct serial_port* port)
{
    uint16_t ubrr = (F_CPU / (16UL * port->baud_rate)) - 1;
    if (ubrr > 4095)
    {
        return -EINVAL;
    }
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (uint8_t)((1 << RXEN0) | (1 << TXEN0));
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    return 0;
}

static inline int8_t usart1_initialize(const struct serial_port* port)
{
    uint16_t ubrr = (F_CPU / (16UL * port->baud_rate)) - 1;
    if (ubrr > 4095)
    {
        return -EINVAL;
    }
    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)ubrr;
    UCSR1B = (uint8_t)((1 << RXEN1) | (1 << TXEN1));
    UCSR1C = (1 << USBS1) | (3 << UCSZ10);
    return 0;
}

int8_t usart_initialize(const struct serial_port* port)
{
    if (port == NULL || port->baud_rate == 0)
    {
        return -EINVAL;
    }
    switch (port->port_id)
    {
        case 0:
            if (usart0_initialize(port))
            {
                return -EINVAL;
            }
            break;

        case 1:
            if (usart1_initialize(port))
            {
                return -EINVAL;
            }
            break;

        default:
            break;
    }
    return 0;
}

/* ============================================================================================== */

static inline void _usart0_transmit_byte(const uint8_t* byte)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }
    UDR0 = *byte;
}

static inline void _usart1_transmit_byte(const uint8_t* byte)
{
    while (!(UCSR1A & (1 << UDRE1)))
    {
    }
    UDR1 = *byte;
}

int8_t usart_transmit(const struct serial_port* port, const uint8_t* buffer, size_t size)
{
    if (port == NULL || buffer == NULL)
    {
        return -EFAULT;
    }
    if (size == 0)
    {
        return -EINVAL;
    }
    switch (port->port_id)
    {
        case 0:

            for (size_t i = 0; i < size; i++)
            {
                _usart0_transmit_byte(buffer + i);
            }
            break;

        case 1:

            for (size_t i = 0; i < size; i++)
            {
                _usart1_transmit_byte(buffer + i);
            }
            break;

        default:

            break;
    }

    return 0;
}

/* ============================================================================================== */

static inline uint8_t _usart0_receive_byte(void)
{
    while (!(UCSR0A & (1 << RXC0)))
    {
    }
    return UDR0;
}

static inline uint8_t _usart1_receive_byte(void)
{
    while (!(UCSR1A & (1 << RXC1)))
    {
    }
    return UDR1;
}

int8_t usart_receive(const struct serial_port* port, uint8_t* buffer, size_t size)
{
    if (port == NULL || buffer == NULL)
    {
        return -EFAULT;
    }
    if (size == 0)
    {
        return -EINVAL;
    }
    switch (port->port_id)
    {
        case 0:
            for (size_t i = 0; i < size; i++)
            {
                buffer[i] = _usart0_receive_byte();
            }
            break;

        case 1:
            for (size_t i = 0; i < size; i++)
            {
                buffer[i] = _usart1_receive_byte();
            }
            break;

        default:
            break;
    }
    return 0;
}

/* ============================================================================================== */

int8_t usart_set_rx_callback(const struct serial_port* port,
                             serial_rx_callback_t      callback,
                             void*                     callback_context)
{
    if (port == NULL || callback == NULL || callback_context == NULL)
    {
        return -EFAULT;
    }
    switch (port->port_id)
    {
        case 0:
            usart0_callback         = callback;
            usart0_callback_context = callback_context;
            break;

        case 1:
            usart1_callback         = callback;
            usart1_callback_context = callback_context;
            break;

        default:
            break;
    }
    return 0;
}

/* ============================================================================================== */

void usart_enable_rx_interrupt(const struct serial_port* port, bool enable)
{
    if (port == NULL)
    {
        return;
    }
    switch (port->port_id)
    {
        case 0:
            if (enable)
            {
                UCSR0B |= (1 << RXCIE0);
            }
            else
            {
                UCSR0B &= ~(1 << RXCIE0);
            }
            break;

        case 1:
            if (enable)
            {
                UCSR1B |= (1 << RXCIE1);
            }
            else
            {
                UCSR1B &= ~(1 << RXCIE1);
            }
            break;

        default:
            break;
    }
}

/* ============================================================================================== */

void usart_clear_buffers(const struct serial_port* port)
{
    (void)port;
    // No buffers to clear in this implementation
}

/* ============================================================================================== */

ISR(USART0_RX_vect)
{
    uint8_t data = UDR0;
    if (usart0_callback)
    {
        usart0_callback(usart0_callback_context, data);
    }
}

ISR(USART1_RX_vect)
{
    uint8_t data = UDR1;
    if (usart1_callback)
    {
        usart1_callback(usart1_callback_context, data);
    }
}

/* ============================================================================================== */
