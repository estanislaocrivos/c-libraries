#include "../inc/pic18_uart1.h"

#include "../inc/pic18_interrupts.h"

/* ========================================================================== */

const struct serial_ops pic18_uart1_ops = {
    .initialize                = pic18_uart1_initialize,
    .transmit                  = pic18_uart1_transmit,
    .receive                   = pic18_uart1_receive,
    .set_rx_callback           = pic18_uart1_set_rx_callback,
    .enable_rx_interrupt       = pic18_uart1_enable_rx_interrupt,
    .set_rx_interrupt_priority = pic18_uart1_set_rx_interrupt_priority,
    .flush_tx                  = pic18_uart1_flush_tx,
    .flush_rx                  = pic18_uart1_flush_rx,
};

/* ========================================================================== */

/**
 * @brief UART1 RX callback and context.
 */
static serial_rx_callback_t pic18_uart1_serial_rx_callback;
static void*                pic18_uart1_callback_context;

/**
 * @brief Internal ISR handler registered with the interrupt module.
 * Reads the received byte and dispatches to the user callback.
 */
static inline void pic18_uart1_isr_handler(void)
{
    PIR3bits.U1RXIF = 0;

    /* Check for RX errors before reading the byte */
    if (U1ERRIRbits.FERIF || U1ERRIRbits.PERIF || U1ERRIRbits.RXFOIF)
    {
        U1ERRIRbits.FERIF  = 0;
        U1ERRIRbits.PERIF  = 0;
        U1ERRIRbits.RXFOIF = 0;
        (void)U1RXB; /* Discard corrupt byte */
        return;
    }

    uint8_t byte = U1RXB;
    if (pic18_uart1_serial_rx_callback != NULL)
    {
        pic18_uart1_serial_rx_callback(pic18_uart1_callback_context, byte);
    }
}

/* ========================================================================== */

/**
 * @brief Determines the MODE bits value for U1CON0 based on data_bits and
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

int8_t pic18_uart1_initialize(struct serial* self)
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
    U1CON1bits.ON = 0;

    /* Configure U1CON0: mode, baud rate speed, auto-baud, TX/RX enable */
    U1CON0bits.BRGS  = 0; /* Baud rate generator uses FOSC/16 */
    U1CON0bits.ABDEN = 0;
    U1CON0bits.TXEN  = 1;
    U1CON0bits.RXEN  = 1;
    U1CON0bits.MODE  = mode;

    /* Configure baud rate: BAUD = FOSC / (16 * (BRG + 1)) */
    uint16_t brg_value
        = (uint16_t)((_XTAL_FREQ / (16UL * self->baud_rate)) - 1U);
    U1BRGL = (uint8_t)(brg_value & 0xFF);
    U1BRGH = (uint8_t)(brg_value >> 8);

    /* Configure U1CON2: stop bits, polarity, overflow behavior */
    U1CON2            = 0;
    U1CON2bits.RUNOVF = 1; /* RX shifter stops on FIFO overflow */
    if (self->stop_bits == 2)
    {
        U1CON2bits.STP = 0b10;
    }
    else if (self->stop_bits == 1)
    {
        U1CON2bits.STP = 0b00;
    }
    else /* Unsupported stop bits */
    {
        return -EINVAL;
    }

    pic18_isr_register(PIC18_IRQ_UART1_RX, pic18_uart1_isr_handler);

    U1CON1bits.ON         = 1;
    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

static inline int8_t pic18_uart1_transmit_byte(uint8_t byte)
{
    if (!U1CON0bits.TXEN)
    {
        return -EPERM;
    }
    while (U1FIFObits.TXBF)
    {
    }
    U1TXB = byte;
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart1_transmit(
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
        int8_t result = pic18_uart1_transmit_byte(buffer[i]);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart1_receive(const struct serial* self, uint8_t* data)
{
    if (self == NULL || data == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    while (!U1FIFObits.RXBF)
    {
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart1_set_rx_callback(
    const struct serial* self,
    serial_rx_callback_t callback,
    void*                callback_context)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    pic18_uart1_serial_rx_callback = callback;
    if (callback_context != NULL)
    {
        pic18_uart1_callback_context = callback_context;
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart1_enable_rx_interrupt(const struct serial* self, bool enable)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    PIR3bits.U1RXIF = 0;
    PIE3bits.U1RXIE = (uint8_t)enable;
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart1_set_rx_interrupt_priority(
    const struct serial* self, uint8_t priority)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    INTCON0bits.IPEN = 1;
    if (priority == 0)
    {
        IPR3bits.U1RXIP = 1;  // High priority
    }
    else
    {
        IPR3bits.U1RXIP = 0;  // Low priority
    }
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart1_flush_tx(const struct serial* self)
{
    U1FIFObits.TXBE = 1;
    return 0;
}

/* ========================================================================== */

int8_t pic18_uart1_flush_rx(const struct serial* self)
{
    U1FIFObits.RXBE = 1;
    return 0;
}

/* ========================================================================== */
