#include "../inc/pic18_uart2.h"

#include "../inc/pic18_interrupts.h"

/* ========================================================================== */

const struct serial_ops pic18_uart2_ops = {
    .initialize                = pic18_uart2_initialize,
    .transmit                  = pic18_uart2_transmit,
    .receive                   = pic18_uart2_receive,
    .set_rx_callback           = pic18_uart2_set_rx_callback,
    .enable_rx_interrupt       = pic18_uart2_enable_rx_interrupt,
    .set_rx_interrupt_priority = pic18_uart2_set_rx_interrupt_priority,
    .flush_tx                  = pic18_uart2_flush_tx,
    .flush_rx                  = pic18_uart2_flush_rx,
};

/* ========================================================================== */

/**
 * @brief UART2 RX callback and context.
 */
static serial_rx_callback_t pic18_uart2_serial_rx_callback;
static void*                pic18_uart2_callback_context;

/**
 * @brief Internal ISR handler registered with the interrupt module.
 * Reads the received byte and dispatches to the user callback.
 */
static void pic18_uart2_isr_handler(void)
{
    PIR6bits.U2RXIF = 0;

    /* Check for RX errors before reading the byte */
    if (U2ERRIRbits.FERIF || U2ERRIRbits.PERIF || U2ERRIRbits.RXFOIF)
    {
        U2ERRIRbits.FERIF  = 0;
        U2ERRIRbits.PERIF  = 0;
        U2ERRIRbits.RXFOIF = 0;
        (void)U2RXB; /* Discard corrupt byte */
        return;
    }

    uint8_t byte = U2RXB;
    if (pic18_uart2_serial_rx_callback != NULL)
    {
        pic18_uart2_serial_rx_callback(pic18_uart2_callback_context, byte);
    }
}

/* ========================================================================== */

/**
 * @brief Determines the MODE bits value for U2CON0 based on data_bits and
 * parity configuration.
 *
 * MODE values (from datasheet):
 * - 0b0000 = Asynchronous 8-bit UART mode
 * - 0b0001 = Asynchronous 7-bit UART mode
 * - 0b0010 = Asynchronous 8-bit UART mode with 9th bit odd parity
 * - 0b0011 = Asynchronous 8-bit UART mode with 9th bit even parity
 * - 0b0100 = Asynchronous 9-bit UART Address mode
 *
 * @param data_bits Number of data bits (7, 8, or 9)
 * @param parity Parity configuration: 'N' (None), 'E' (Even), 'O' (Odd)
 * @param mode_out Pointer to store the calculated MODE value
 * @return int8_t 0 on success, -EINVAL if combination is not supported
 */
static int8_t get_uart_mode(uint8_t data_bits, char parity, uint8_t* mode_out)
{
    if (mode_out == NULL)
    {
        return -EFAULT;
    }

    switch (data_bits)
    {
        case 7:
            if (parity == 'N')
            {
                *mode_out = 0b0001;
                return 0;
            }
            break;

        case 8:
            switch (parity)
            {
                case 'N':
                    *mode_out = 0b0000;
                    return 0;
                case 'O':
                    *mode_out = 0b0010;
                    return 0;
                case 'E':
                    *mode_out = 0b0011;
                    return 0;
                default:
                    break;
            }
            break;

        case 9:
            if (parity == 'N')
            {
                *mode_out = 0b0100;
                return 0;
            }
            break;

        default:
            break;
    }

    return -EINVAL;
}

/* ========================================================================== */

int8_t pic18_uart2_initialize(struct serial* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (self->baud_rate == 0)
    {
        return -EINVAL;
    }

    /* Determine MODE based on data_bits and parity */
    uint8_t mode;
    int8_t  result = get_uart_mode(self->data_bits, self->parity, &mode);
    if (result)
    {
        return result;
    }

    /* Disable UART during configuration */
    U2CON1bits.ON = 0;

    /* Configure U2CON0: mode, baud rate speed, auto-baud, TX/RX enable */
    U2CON0bits.BRGS  = 0; /* Baud rate generator uses FOSC/16 */
    U2CON0bits.ABDEN = 0;
    U2CON0bits.TXEN  = 1;
    U2CON0bits.RXEN  = 1;
    U2CON0bits.MODE  = mode;

    /* Configure baud rate: BAUD = FOSC / (16 * (BRG + 1)) */
    uint16_t brg_value
        = (uint16_t)((_XTAL_FREQ / (16UL * self->baud_rate)) - 1U);
    U2BRGL = (uint8_t)(brg_value & 0xFF);
    U2BRGH = (uint8_t)(brg_value >> 8);

    /* Configure U2CON2: stop bits, polarity */
    U2CON2            = 0;
    U2CON2bits.RUNOVF = 1; /* RX shifter stops on FIFO overflow */
    if (self->stop_bits == 2)
    {
        U2CON2bits.STP = 0b10;
    }
    else if (self->stop_bits == 1)
    {
        U2CON2bits.STP = 0b00;
    }
    else
    {
        return -EINVAL;
    }

    pic18_isr_register(PIC18_IRQ_UART2_RX, pic18_uart2_isr_handler);

    U2CON1bits.ON         = 1;
    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

static inline int8_t pic18_uart2_transmit_byte(uint8_t byte)
{
    if (!U2CON0bits.TXEN)
    {
        return -EPERM;
    }
    while (U2FIFObits.TXBF)
    {
    }
    U2TXB = byte;
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart2_transmit(
    const struct serial* self, const uint8_t* buffer, size_t size)
{
    if (self == NULL || buffer == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (size == 0)
    {
        return -EINVAL;
    }
    for (size_t i = 0; i < size; i++)
    {
        int8_t result = pic18_uart2_transmit_byte(buffer[i]);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart2_receive(const struct serial* self, uint8_t* data)
{
    if (self == NULL || data == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    while (!U2FIFObits.RXBF)
    {
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart2_set_rx_callback(
    const struct serial* self,
    serial_rx_callback_t callback,
    void*                callback_context)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    pic18_uart2_serial_rx_callback = callback;
    if (callback_context != NULL)
    {
        pic18_uart2_callback_context = callback_context;
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart2_enable_rx_interrupt(const struct serial* self, bool enable)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    PIR6bits.U2RXIF = 0;
    PIE6bits.U2RXIE = (uint8_t)enable;
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart2_set_rx_interrupt_priority(
    const struct serial* self, uint8_t priority)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    INTCON0bits.IPEN = 1;
    if (priority == 0)
    {
        IPR6bits.U2RXIP = 1;  // High priority
    }
    else
    {
        IPR6bits.U2RXIP = 0;  // Low priority
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart2_flush_tx(const struct serial* self)
{
    U2FIFObits.TXBE = 1;
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart2_flush_rx(const struct serial* self)
{
    U2FIFObits.RXBE = 1;
    return 0;
}

/* ========================================================================== */
