#include "../inc/pic18_i2c2.h"

/* ========================================================================== */

#define I2C2_WRITE_BIT            0
#define I2C2_READ_BIT             1
#define I2C2_MAX_PAYLOAD_SIZE     255
#define I2C2_MAX_REG_PAYLOAD_SIZE 254 /* 255 - 1 byte for register address */
#define I2C2_SW_TIMEOUT           UINT16_MAX /* ~4ms @ 64MHz PIC18 (4 cyc/instr) */

/* ========================================================================== */

const struct i2c_ops pic18_i2c2_ops = {
    .initialize = pic18_i2c2_initialize,
    .write      = pic18_i2c2_write,
    .read       = pic18_i2c2_read,
    .write_raw  = pic18_i2c2_write_raw,
    .read_raw   = pic18_i2c2_read_raw,
    .probe      = pic18_i2c2_probe,
};

/* ========================================================================== */

/**
 * @brief Enables the I2C2 module with silicon errata workaround.
 * Spurious SCIF/PCIF flags may be set after enabling the module.
 * Reference: Errata DS80000870F, Section 1.3.2.
 */
static inline void pic18_i2c2_enable(void)
{
    I2C2CON0bits.EN = 1;
    __delay_us(1);
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    I2C2PIRbits.SCIF = 0;
    I2C2PIRbits.PCIF = 0;
}

/* ========================================================================== */

/**
 * @brief Checks for hardware bus errors (timeout or collision).
 * @return int8_t 0 if no error, -EIO on bus collision, -ETIMEDOUT on bus
 * timeout.
 */
static inline int8_t pic18_i2c2_check_bus_error(void)
{
    if (I2C2ERRbits.BCLIF)
    {
        return -EIO;
    }
    if (I2C2ERRbits.BTOIF)
    {
        return -ETIMEDOUT;
    }
    return 0;
}

/* ========================================================================== */

/**
 * @brief Checks if the I2C bus is free.
 * @return true if bus is free, false if busy.
 */
static inline bool pic18_i2c2_is_bus_free(void)
{
    return I2C2STAT0bits.BFRE;
}

/* ========================================================================== */

/**
 * @brief Clears all I2C2 interrupt and error flags.
 */
static inline void pic18_i2c2_clear_flags(void)
{
    I2C2PIR            = 0x00;
    I2C2ERRbits.BCLIF  = 0;
    I2C2ERRbits.BTOIF  = 0;
    I2C2ERRbits.NACKIF = 0;
}

/* ========================================================================== */

/**
 * @brief Resets the I2C2 bus by disabling and re-enabling the module.
 * Clears all flags, buffers, and applies the silicon errata workaround.
 */
static inline void pic18_i2c2_reset_bus(void)
{
    I2C2CON0bits.RSEN   = 0;
    I2C2PIR             = 0x00;
    I2C2ERRbits.BCLIF   = 0;
    I2C2ERRbits.BTOIF   = 0;
    I2C2ERRbits.NACKIF  = 0;
    I2C2STAT1bits.CLRBF = 1;
    I2C2CON0bits.EN     = 0;
    pic18_i2c2_enable();
}

/* ========================================================================== */

/**
 * @brief Sets the device address for the I2C transaction.
 * @param self Pointer to the I2C structure (used for address and address mode).
 * @param read true for read operation, false for write operation.
 */
static inline void pic18_i2c2_set_device_address(
    const struct i2c* self, bool read)
{
    uint8_t rw_bit = read ? I2C2_READ_BIT : I2C2_WRITE_BIT;

    if (self->address_mode == I2C_ADDR_10BIT)
    {
        /* 10-bit: ADB1 = 11110[A9:A8](R/W), ADB0 = A[7:0] */
        I2C2ADB1
            = (uint8_t)(0xF0 | ((self->slave_address >> 7) & 0x06) | rw_bit);
        I2C2ADB0 = (uint8_t)(self->slave_address & 0xFF);
    }
    else
    {
        /* 7-bit: ADB1 = [A6:A0](R/W) */
        I2C2ADB1 = (uint8_t)((self->slave_address << 1) | rw_bit);
    }
}

/* ========================================================================== */

/**
 * @brief Writes a byte to the TX buffer.
 * @param payload Byte to write.
 * @return int8_t 0 on success, -EAGAIN if buffer was full.
 */
static inline int8_t pic18_i2c2_write_tx_buffer(uint8_t payload)
{
    I2C2TXB = payload;
    if (I2C2STAT1bits.TXWE == 1)
    {
        I2C2STAT1bits.TXWE = 0;
        return -EAGAIN;
    }
    return 0;
}

/* ========================================================================== */

/**
 * @brief Sends the start condition.
 */
static inline void pic18_i2c2_send_start(void)
{
    I2C2CON0bits.S = 1;
}

/* ========================================================================== */

/**
 * @brief Waits for the TX buffer to empty.
 * @return int8_t 0 on success, -ETIMEDOUT on bus timeout, -EIO on bus
 * collision.
 */
static inline int8_t pic18_i2c2_wait_tx_empty(void)
{
    uint16_t timeout = I2C2_SW_TIMEOUT;
    while (I2C2STAT1bits.TXBE == 0)
    {
        int8_t err = pic18_i2c2_check_bus_error();
        if (err != 0)
        {
            return err;
        }
        if (--timeout == 0)
        {
            return -ETIMEDOUT;
        }
    }
    return 0;
}

/* ========================================================================== */

/**
 * @brief Waits for the RX buffer to be full and reads the byte.
 * @param data Pointer to store the received byte.
 * @return int8_t 0 on success, -ETIMEDOUT on bus timeout, -EIO on bus
 * collision.
 */
static inline int8_t pic18_i2c2_wait_rx_full(uint8_t* data)
{
    uint16_t timeout = I2C2_SW_TIMEOUT;
    while (I2C2STAT1bits.RXBF == 0)
    {
        int8_t err = pic18_i2c2_check_bus_error();
        if (err != 0)
        {
            return err;
        }
        if (--timeout == 0)
        {
            return -ETIMEDOUT;
        }
    }
    *data = I2C2RXB;
    return 0;
}

/* ========================================================================== */

/**
 * @brief Checks if a NACK was received, differentiating between address and
 * data phase.
 * @return int8_t 0 if ACK received, -ENXIO if address NACK, -EIO if data NACK.
 */
static inline int8_t pic18_i2c2_check_nack(void)
{
    if (I2C2CON1bits.ACKSTAT == 1)
    {
        return I2C2STAT0bits.D ? -EIO : -ENXIO;
    }
    return 0;
}

/* ========================================================================== */

/**
 * @brief Configures ACK/NACK response for next received byte.
 * @param ack true to send ACK, false to send NACK.
 */
static inline void pic18_i2c2_set_ack_response(bool ack)
{
    I2C2CON1bits.ACKCNT = ack ? 0 : 1;
}

/* ========================================================================== */

/**
 * @brief Waits for the stop condition to complete.
 * @return int8_t 0 on success, -ETIMEDOUT on bus timeout, -EIO on bus
 * collision.
 */
static inline int8_t pic18_i2c2_wait_stop(void)
{
    uint16_t timeout = I2C2_SW_TIMEOUT;
    while (I2C2PIRbits.PCIF == 0)
    {
        int8_t err = pic18_i2c2_check_bus_error();
        if (err != 0)
        {
            return err;
        }
        if (--timeout == 0)
        {
            return -ETIMEDOUT;
        }
    }
    return 0;
}

/* ========================================================================== */

/* Unused for ADT747x slaves */
#if 0
/**
 * @brief Waits for the hardware-generated restart condition to complete.
 * @return int8_t 0 on success, -ETIMEDOUT on bus timeout, -EIO on bus collision.
 */
static inline int8_t pic18_i2c2_wait_restart(void)
{
    uint16_t timeout = I2C2_SW_TIMEOUT;
    while (I2C2PIRbits.RSCIF == 0)
    {
        int8_t err = pic18_i2c2_check_bus_error();
        if (err != 0)
        {
            return err;
        }
        if (--timeout == 0)
        {
            return -ETIMEDOUT;
        }
    }
    I2C2PIRbits.RSCIF = 0;
    I2C2PIRbits.SCIF  = 0;
    I2C2PIRbits.CNTIF = 0;
    return 0;
}
#endif

/* ========================================================================== */

int8_t pic18_i2c2_initialize(struct i2c* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Disable I2C during configuration */
    I2C2CON0bits.EN = 0;

    I2C2CON0bits.RSEN = 0; /* Restart condition disabled (manually controlled)
                            */

    /*
     * MODE[2:0]: I2C Mode Select bits
     * 101 = I2C Host mode, 10-bit address
     * 100 = I2C Host mode, 7-bit address
     * 010 = I2C Client mode, two 10-bit address
     * 000 = I2C Client mode, four 7-bit address
     */
    if (self->master)
    {
        I2C2CON0bits.MODE = (self->address_mode == I2C_ADDR_10BIT) ? 0b101 :
                                                                     0b100;
    }
    else
    {
        I2C2CON0bits.MODE = (self->address_mode == I2C_ADDR_10BIT) ? 0b010 :
                                                                     0b000;

        /* Configure own address for client mode */
        I2C2ADR0 = (uint8_t)(self->own_address << 1);
        I2C2ADR1 = 0;
        I2C2ADR2 = 0;
        I2C2ADR3 = 0;
    }

    I2C2CON1bits.ACKCNT = 1; /* NACK after receiving (will be changed
                                dynamically) */
    I2C2CON1bits.ACKDT = 0;  /* ACK value is 0 (acknowledge) */
    I2C2CON1bits.RXO   = 0;  /* Receive overflow handling disabled */
    I2C2CON1bits.TXU   = 0;  /* Transmit underflow handling disabled */
    I2C2CON1bits.CSD   = 0;  /* Clock stretching enabled */

    I2C2CON2bits.ACNT  = 0;    /* Address byte count mode disabled */
    I2C2CON2bits.GCEN  = 0;    /* General call disabled */
    I2C2CON2bits.ABD   = 0;    /* Address buffer disabled */
    I2C2CON2bits.SDAHT = 0;    /* SDA hold time 300ns */
    I2C2CON2bits.BFRET = 0b11; /* Bus free time = 8 I2C clock pulses */

    /*
     * Configure clock source and speed:
     * I2C2CLK: 0011 = MFINTOSC (500kHz), 0010 = HFINTOSC, 0001 = FOSC, 0000 =
     * FOSC/4 FME: 0 = FSCL = FI2CXCLK/5, 1 = FSCL = FI2CXCLK/4
     *
     * Common configurations:
     * - 100kHz: MFINTOSC (500kHz), FME=0 → 500/5 = 100kHz
     * - 125kHz: MFINTOSC (500kHz), FME=1 → 500/4 = 125kHz
     * - 400kHz: FOSC/4 with appropriate FOSC, or HFINTOSC
     */
    if (self->frequency <= 100000)
    {
        /* Standard mode: 100kHz using MFINTOSC */
        I2C2CLK          = 0b0011; /* MFINTOSC (500kHz) */
        I2C2CON2bits.FME = 0;      /* FSCL = 500kHz / 5 = 100kHz */
    }
    else if (self->frequency <= 125000)
    {
        /* 125kHz using MFINTOSC */
        I2C2CLK          = 0b0011; /* MFINTOSC (500kHz) */
        I2C2CON2bits.FME = 1;      /* FSCL = 500kHz / 4 = 125kHz */
    }
    else
    {
        /* Fast mode: 400kHz using FOSC/4 (assumes _XTAL_FREQ allows it) */
        I2C2CLK          = 0b0000; /* FOSC/4 */
        I2C2CON2bits.FME = 1;      /* FSCL = (FOSC/4) / 4 */
    }

    /* Enable hardware bus timeout and collision detection */
    I2C2ERRbits.BTOIE = 1;
    I2C2ERRbits.BCLIE = 1;

    I2C2STAT1bits.CLRBF = 1;
    pic18_i2c2_clear_flags();
    pic18_i2c2_enable();

    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t pic18_i2c2_write(
    const struct i2c* self,
    uint8_t           reg_address,
    const uint8_t*    tx_payload,
    size_t            tx_payload_size)
{
    if (self == NULL || tx_payload == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (!self->master)
    {
        return -ENOTSUP;
    }
    if (tx_payload_size == 0 || tx_payload_size > I2C2_MAX_REG_PAYLOAD_SIZE)
    {
        return -EINVAL;
    }
    if (!pic18_i2c2_is_bus_free())
    {
        return -EBUSY;
    }

    int8_t result = 0;

    pic18_i2c2_clear_flags();

    /* Set byte count: 1 (register address) + payload size */
    I2C2CNT = (uint8_t)(1 + tx_payload_size);

    pic18_i2c2_set_device_address(self, false);

    result = pic18_i2c2_write_tx_buffer(reg_address);
    if (result)
    {
        return result;
    }

    pic18_i2c2_send_start();

    result = pic18_i2c2_wait_tx_empty();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    result = pic18_i2c2_check_nack();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    for (size_t i = 0; i < tx_payload_size; i++)
    {
        result = pic18_i2c2_write_tx_buffer(tx_payload[i]);
        if (result)
        {
            pic18_i2c2_reset_bus();
            return result;
        }

        result = pic18_i2c2_wait_tx_empty();
        if (result)
        {
            pic18_i2c2_reset_bus();
            return result;
        }

        result = pic18_i2c2_check_nack();
        if (result)
        {
            pic18_i2c2_reset_bus();
            return result;
        }
    }

    result = pic18_i2c2_wait_stop();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    uint16_t timeout = I2C2_SW_TIMEOUT;
    while (!pic18_i2c2_is_bus_free())
    {
        int8_t err = pic18_i2c2_check_bus_error();
        if (err != 0)
        {
            pic18_i2c2_reset_bus();
            return err;
        }
        if (--timeout == 0)
        {
            pic18_i2c2_reset_bus();
            return -ETIMEDOUT;
        }
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_i2c2_read(
    const struct i2c* self,
    uint8_t           reg_address,
    uint8_t*          rx_payload,
    size_t            rx_payload_size)
{
    if (self == NULL || rx_payload == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (!self->master)
    {
        return -ENOTSUP;
    }
    if (rx_payload_size == 0 || rx_payload_size > I2C2_MAX_PAYLOAD_SIZE)
    {
        return -EINVAL;
    }
    if (!pic18_i2c2_is_bus_free())
    {
        return -EBUSY;
    }

    int8_t result = 0;

    pic18_i2c2_clear_flags();

    /* Write phase: send register address */
    I2C2CNT = 1;

    pic18_i2c2_set_device_address(self, false);

    if (pic18_i2c2_write_tx_buffer(reg_address))
    {
        return -EIO;
    }

    pic18_i2c2_send_start();

    if (pic18_i2c2_wait_tx_empty())
    {
        pic18_i2c2_reset_bus();
        return -EIO;
    }

    if (pic18_i2c2_check_nack())
    {
        pic18_i2c2_reset_bus();
        return -EIO;
    }

    if (pic18_i2c2_wait_stop())
    {
        pic18_i2c2_reset_bus();
        return -EIO;
    }

    /* Read phase */
    pic18_i2c2_clear_flags();

    I2C2CNT = (uint8_t)rx_payload_size;

    pic18_i2c2_set_device_address(self, true);

    pic18_i2c2_send_start();

    if (pic18_i2c2_wait_tx_empty())
    {
        pic18_i2c2_reset_bus();
        return -EIO;
    }

    if (pic18_i2c2_check_nack())
    {
        pic18_i2c2_reset_bus();
        return -EIO;
    }

    for (size_t i = 0; i < rx_payload_size; i++)
    {
        /* NACK on last byte, ACK otherwise */
        pic18_i2c2_set_ack_response(i != rx_payload_size - 1);

        if (pic18_i2c2_wait_rx_full(&rx_payload[i]))
        {
            pic18_i2c2_reset_bus();
            return -EIO;
        }
    }

    if (pic18_i2c2_wait_stop())
    {
        pic18_i2c2_reset_bus();
        return -EIO;
    }

    uint16_t timeout = I2C2_SW_TIMEOUT;
    while (!pic18_i2c2_is_bus_free())
    {
        int8_t err = pic18_i2c2_check_bus_error();
        if (err != 0)
        {
            pic18_i2c2_reset_bus();
            return err;
        }
        if (--timeout == 0)
        {
            pic18_i2c2_reset_bus();
            return -ETIMEDOUT;
        }
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_i2c2_write_raw(
    const struct i2c* self, const uint8_t* tx_payload, size_t tx_payload_size)
{
    if (self == NULL || tx_payload == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (!self->master)
    {
        return -ENOTSUP;
    }
    if (tx_payload_size == 0 || tx_payload_size > I2C2_MAX_PAYLOAD_SIZE)
    {
        return -EINVAL;
    }
    if (!pic18_i2c2_is_bus_free())
    {
        return -EBUSY;
    }

    int8_t result = 0;

    pic18_i2c2_clear_flags();

    I2C2CNT = (uint8_t)tx_payload_size;

    pic18_i2c2_set_device_address(self, false);

    if (pic18_i2c2_write_tx_buffer(tx_payload[0]))
    {
        return -EIO;
    }

    pic18_i2c2_send_start();

    result = pic18_i2c2_wait_tx_empty();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    result = pic18_i2c2_check_nack();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    for (size_t i = 1; i < tx_payload_size; i++)
    {
        result = pic18_i2c2_write_tx_buffer(tx_payload[i]);
        if (result)
        {
            pic18_i2c2_reset_bus();
            return result;
        }

        result = pic18_i2c2_wait_tx_empty();
        if (result)
        {
            pic18_i2c2_reset_bus();
            return result;
        }

        result = pic18_i2c2_check_nack();
        if (result)
        {
            pic18_i2c2_reset_bus();
            return result;
        }
    }

    result = pic18_i2c2_wait_stop();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_i2c2_read_raw(
    const struct i2c* self, uint8_t* rx_payload, size_t rx_payload_size)
{
    if (self == NULL || rx_payload == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (!self->master)
    {
        return -ENOTSUP;
    }
    if (rx_payload_size == 0 || rx_payload_size > I2C2_MAX_PAYLOAD_SIZE)
    {
        return -EINVAL;
    }
    if (!pic18_i2c2_is_bus_free())
    {
        return -EBUSY;
    }

    int8_t result = 0;

    pic18_i2c2_clear_flags();

    I2C2CNT = (uint8_t)rx_payload_size;

    pic18_i2c2_set_device_address(self, true);

    pic18_i2c2_send_start();

    result = pic18_i2c2_wait_tx_empty();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    result = pic18_i2c2_check_nack();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    for (size_t i = 0; i < rx_payload_size; i++)
    {
        /* NACK on last byte, ACK otherwise */
        pic18_i2c2_set_ack_response(i != rx_payload_size - 1);

        result = pic18_i2c2_wait_rx_full(&rx_payload[i]);
        if (result)
        {
            pic18_i2c2_reset_bus();
            return result;
        }
    }

    result = pic18_i2c2_wait_stop();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    return 0;
}

/* ========================================================================== */

int8_t pic18_i2c2_probe(const struct i2c* self)
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
        return -ENOTSUP;
    }
    if (!pic18_i2c2_is_bus_free())
    {
        return -EBUSY;
    }

    pic18_i2c2_clear_flags();

    /* Set byte count to 0 - just address phase */
    I2C2CNT = 0;

    pic18_i2c2_set_device_address(self, false);

    pic18_i2c2_send_start();

    int8_t result = pic18_i2c2_wait_tx_empty();
    if (result)
    {
        pic18_i2c2_reset_bus();
        return result;
    }

    result = pic18_i2c2_check_nack();

    int8_t stop_result = pic18_i2c2_wait_stop();
    if (stop_result)
    {
        pic18_i2c2_reset_bus();
        return stop_result;
    }

    return result;
}

/* ========================================================================== */
