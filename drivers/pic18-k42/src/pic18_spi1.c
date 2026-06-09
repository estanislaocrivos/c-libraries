#include "../inc/pic18_spi1.h"

#include "../inc/pic18_interrupts.h"

/* ========================================================================== */

const struct spi_ops pic18_spi1_ops = {
    .initialize          = pic18_spi1_initialize,
    .transmit            = pic18_spi1_transmit,
    .receive             = pic18_spi1_receive,
    .transfer            = pic18_spi1_transfer,
    .set_cs              = pic18_spi1_set_cs,
    .set_frequency       = pic18_spi1_set_frequency,
    .set_rx_callback     = pic18_spi1_set_rx_callback,
    .enable_rx_interrupt = pic18_spi1_enable_rx_interrupt,
    .flush_tx            = pic18_spi1_flush_tx,
    .flush_rx            = pic18_spi1_flush_rx,
};

/* ========================================================================== */

/* Static state for RX callback */
static spi_rx_callback_t pic18_spi1_rx_callback  = NULL;
static void*             pic18_spi1_callback_ctx = NULL;

/* ========================================================================== */

/**
 * @brief Internal ISR handler for SPI1 RX interrupt.
 *
 * This function is registered with the interrupt system and called when
 * SPI1 RX interrupt fires. The interrupt flag is cleared by the interrupt
 * dispatcher before calling this handler.
 */
static inline void pic18_spi1_rx_isr_handler(void)
{
    PIR2bits.SPI1RXIF = 0;
    uint8_t byte      = SPI1RXB;
    if (pic18_spi1_rx_callback != NULL)
    {
        pic18_spi1_rx_callback(pic18_spi1_callback_ctx, byte);
    }
}

/* ========================================================================== */

/**
 * @brief Calculate baud rate divisor for desired frequency.
 *
 * SPI clock = FOSC / (2 * (BAUD + 1))
 * Therefore: BAUD = (FOSC / (2 * freq)) - 1
 *
 * @param frequency Desired frequency in Hz.
 * @return Baud rate register value.
 */
static uint8_t calculate_baud(uint32_t frequency_hz)
{
    if (frequency_hz == 0)
    {
        return 0xFF; /* Minimum frequency */
    }

    /* FSCK = FOSC / (2 * (BAUD + 1)), therefore BAUD = (FOSC / (2 * FSCK)) - 1
     */
    uint32_t divisor = _XTAL_FREQ / (2UL * frequency_hz);
    uint32_t baud    = (divisor > 0) ? (divisor - 1) : 0;

    if (baud > 255)
    {
        baud = 255;
    }

    return (uint8_t)baud;
}

/* ========================================================================== */

/**
 * @brief Clear hardware FIFO buffers.
 */
static inline void clear_hw_buffers(void)
{
    SPI1STATUSbits.CLRBF = 1;
}

/**
 * @brief Wait for TX buffer to be empty (ready for new data).
 */
static inline void wait_tx_ready(void)
{
    while (SPI1CON2bits.BUSY)
    {
        /* Wait for SPI to be ready */
    }
}

/**
 * @brief Transfer a single byte (full-duplex).
 * @param tx_byte Byte to transmit.
 * @return Received byte.
 */
static uint8_t transfer_byte(uint8_t tx_byte)
{
    SPI1TXB = tx_byte;
    wait_tx_ready();
    return SPI1RXB;
}

/* ========================================================================== */

int8_t pic18_spi1_initialize(struct spi* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Disable SPI during configuration */
    SPI1CON0bits.EN = 0;

    /* SPI1CON0: Control Register 0 */

    /* Master/Slave mode */
    SPI1CON0bits.MST = self->master ? 1 : 0;

    /* Byte mode: transfers occur when RXFIFO not full and TXFIFO has data */
    SPI1CON0bits.BMODE = 1;

    /* Bit order: MSB or LSB first */
    SPI1CON0bits.LSBF = self->msb_first ? 0 : 1;

    /* SPI1CON1: Control Register 1 (Clock Polarity and Phase) */

    /* SPI Modes:
     *   Mode 0: CPOL=0, CPHA=0 -> CKP=0, CKE=1
     *   Mode 1: CPOL=0, CPHA=1 -> CKP=0, CKE=0
     *   Mode 2: CPOL=1, CPHA=0 -> CKP=1, CKE=1
     *   Mode 3: CPOL=1, CPHA=1 -> CKP=1, CKE=0
     *
     * PIC18 mapping:
     *   CKP = Clock Polarity (idle state)
     *   CKE = Clock Edge (0 = idle->active, 1 = active->idle)
     */

    switch (self->mode)
    {
        case 0: /* CPOL=0, CPHA=0 */
            SPI1CON1bits.CKP = 0;
            SPI1CON1bits.CKE = 1;
            break;
        case 1: /* CPOL=0, CPHA=1 */
            SPI1CON1bits.CKP = 0;
            SPI1CON1bits.CKE = 0;
            break;
        case 2: /* CPOL=1, CPHA=0 */
            SPI1CON1bits.CKP = 1;
            SPI1CON1bits.CKE = 1;
            break;
        case 3: /* CPOL=1, CPHA=1 */
            SPI1CON1bits.CKP = 1;
            SPI1CON1bits.CKE = 0;
            break;
        default:
            return -EINVAL;
    }

    /* Sample point: middle of data output time */
    SPI1CON1bits.SMP = 0;

    /* SDI/SDO polarity: active high */
    SPI1CON1bits.SDIP = 0;
    SPI1CON1bits.SDOP = 0;

    /* SS polarity */
    SPI1CON1bits.SSP = self->cs_active_high ? 1 : 0;

    /* SPI1CON2: Control Register 2 */

    /* SS pin control: hardware or software */
    SPI1CON2bits.SSET = self->hardware_cs ? 1 : 0;

    /* TX required for transfer (full-duplex) */
    SPI1CON2bits.TXR = 1;

    /* RX required: suspend if RXFIFO full */
    SPI1CON2bits.RXR = 1;

    /* Clock source and baud rate (master mode only) */

    if (self->master)
    {
        SPI1CLK  = 0x00; /* Clock source: FOSC */
        SPI1BAUD = calculate_baud(self->frequency_hz);
    }
    else
    {
        SPI1CLK  = 0x00;
        SPI1BAUD = 0x00; /* Not used in slave mode */
    }

    /* Interrupts: disabled by default, user enables via API */

    PIE2bits.SPI1RXIE = 0;
    PIE2bits.SPI1TXIE = 0;
    PIR2bits.SPI1RXIF = 0;
    PIR2bits.SPI1TXIF = 0;

    /* Set high priority for RX interrupt */
    IPR2bits.SPI1RXIP = 1;
    IPR2bits.SPI1TXIP = 1;

    /* Register ISR handler with interrupt system */
    pic18_isr_register(PIC18_IRQ_SPI1_RX, pic18_spi1_rx_isr_handler);

    /* Clear any pending data in buffers */
    clear_hw_buffers();

    /* Enable SPI module */
    SPI1CON0bits.EN = 1;

    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_transmit(
    const struct spi* self, const uint8_t* buffer, size_t size)
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
        return 0;
    }

    for (size_t i = 0; i < size; i++)
    {
        (void)transfer_byte(buffer[i]);
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_receive(const struct spi* self, uint8_t* data)
{
    if (self == NULL || data == NULL)
    {
        return -EFAULT;
    }

    if (!self->was_initialized)
    {
        return -EPERM;
    }

    /* In master mode, must transmit to generate clock */
    *data = transfer_byte(0xFF);
    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_transfer(
    const struct spi* self,
    const uint8_t*    tx_buffer,
    uint8_t*          rx_buffer,
    size_t            size)
{
    if (self == NULL || tx_buffer == NULL || rx_buffer == NULL)
    {
        return -EFAULT;
    }

    if (!self->was_initialized)
    {
        return -EPERM;
    }

    if (size == 0)
    {
        return 0;
    }

    for (size_t i = 0; i < size; i++)
    {
        uint8_t tx_byte = (tx_buffer != NULL) ? tx_buffer[i] : 0xFF;
        rx_buffer[i]    = transfer_byte(tx_byte);
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_set_cs(const struct spi* self, bool active)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /*
     * Note: This is a placeholder implementation.
     * For proper multi-device support, CS should be controlled via GPIO
     * outside of this driver. The user should use pic18_gpio to control
     * the CS pin for each device.
     *
     * If hardware CS is enabled (SSET=1), this function has no effect
     * as the hardware controls the SS pin automatically.
     */

    (void)active;
    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_set_frequency(const struct spi* self, uint32_t frequency)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    if (!self->was_initialized)
    {
        return -EPERM;
    }

    if (!self->master)
    {
        return -EPERM; /* Frequency only applicable in master mode */
    }

    SPI1CON0bits.EN = 0;
    SPI1BAUD        = calculate_baud(frequency);
    SPI1CON0bits.EN = 1;
    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_set_rx_callback(
    const struct spi* self, spi_rx_callback_t callback, void* callback_context)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    pic18_spi1_rx_callback  = callback;
    pic18_spi1_callback_ctx = callback_context;
    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_enable_rx_interrupt(const struct spi* self, bool enable)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    PIR2bits.SPI1RXIF = 0;
    PIE2bits.SPI1RXIE = enable ? 1 : 0;
    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_flush_tx(const struct spi* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Wait for any pending TX to complete */
    wait_tx_ready();
    return 0;
}

/* ========================================================================== */

int8_t pic18_spi1_flush_rx(const struct spi* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Clear hardware buffers */
    clear_hw_buffers();
    return 0;
}

/* ========================================================================== */
