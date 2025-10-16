#include "../inc/i2c_driver.h"

/* ============================================================================================== */

#define I2C_TIME_BETWEEN_OPS_MS 5
#define I2C_TIMEOUT_MS          500

#define I2C_WRITE_BIT           0
#define I2C_READ_BIT            1

/* ============================================================================================== */

int8_t initialize(struct i2c_port* self)
{
    I2C1CON0bits.EN     = 0;
    I2C1CON0bits.RSEN   = 0;
    I2C1CON0bits.MODE   = 0b111;
    I2C1CON1bits.ACKCNT = 1;
    I2C1CON1bits.ACKDT  = 0;
    I2C1CON1bits.RXO    = 0;
    I2C1CON1bits.TXU    = 0;
    I2C1CON1bits.CSD    = 0;
    I2C1CON2bits.ACNT   = 0;
    I2C1CON2bits.GCEN   = 0;
    I2C1CON2bits.FME    = 0;
    I2C1CON2bits.ABD    = 0;
    I2C1CON2bits.SDAHT  = 0;
    I2C1CON2bits.BFRET  = 0b11;
    I2C1CLK = 0b0011;  // MFINTOSC = 500kHz (exceeds max I2C clock speed of 400kHz, but works fine
                       // in practice. Other clock settings make I2C fail)
    I2C1STAT1bits.CLRBF = 1;
    I2C1CON0bits.EN     = 1;
    return 0;
}

/* ============================================================================================== */

static inline void _i2c1_set_device_address(uint8_t address, bool write)
{
    if (write)
    {
        I2C1ADB1 = (uint8_t)((address << 1) | I2C_WRITE_BIT);
    }
    else
    {
        I2C1ADB1 = (uint8_t)((address << 1) | I2C_READ_BIT);
    }
}

static inline int8_t _i2c1_write_tx_buffer(uint8_t payload)
{
    I2C1TXB = payload;
    if (I2C1STAT1bits.TXWE)
    {
        I2C1STAT1bits.TXWE = 0;
        return -EAGAIN;
    }
    return 0;
}

static inline void _i2c1_send_start_condition(void)
{
    I2C1CON0bits.S = 1;
}

static inline void _i2c1_wait_tx_buffer_empty(void)
{
    while (I2C1STAT1bits.TXBE == 0)
    {
    }
}

static inline void _i2c1_wait_rx_buffer_full(uint8_t* data)
{
    while (I2C1STAT1bits.RXBF == 0)
    {
    }
    *data = I2C1RXB;
}

static inline int8_t _i2c1_check_ack(void)
{
    if (I2C1CON1bits.ACKSTAT == 1)
    {
        return -EIO;
    }
    return 0;
}

static inline void _i2c1_send_nack(void)
{
    I2C1CON1bits.ACKCNT = 1;  // Send NACK
}

static inline void _i2c1_wait_stop_condition(void)
{
    while (I2C1PIRbits.PCIF == 0)
    {
    }
}

/* ============================================================================================== */

static inline int8_t _i2c1_write(struct i2c_port* self,
                                 uint8_t          reg_address,
                                 const uint8_t*   tx_payload,
                                 size_t           tx_payload_size)
{
    I2C1CNT = tx_payload_size + 1;  // Will send tx_payload_size + 1 bytes, one after another, the
                                    // first is the register address, the second is the data
    _i2c1_set_device_address(self->slave_address, true);
    if (_i2c1_write_tx_buffer(reg_address))
    {
        return -EAGAIN;
    }
    _i2c1_send_start_condition();  // Start condition
    _i2c1_wait_tx_buffer_empty();
    if (_i2c1_check_ack())
    {
        return -EIO;
    }
    for (size_t i = 0; i < tx_payload_size - 1; i++)
    {
        if (_i2c1_write_tx_buffer(tx_payload[i]))
        {
            return -EAGAIN;
        }
        _i2c1_wait_tx_buffer_empty();
        if (_i2c1_check_ack())
        {
            return -EIO;
        }
    }
    _i2c1_wait_stop_condition();  // Stop condition
    return 0;
}

/* ============================================================================================== */

static inline int8_t _i2c1_read(struct i2c_port* self,
                                uint8_t          reg_address,
                                uint8_t*         rx_payload,
                                size_t           rx_payload_size)
{
    // First i2c transaction: write reg. address to slave
    I2C1CNT = 1;  // Will send 1 byte, the register address
    _i2c1_set_device_address(self->slave_address, true);
    if (_i2c1_write_tx_buffer(reg_address))
    {
        return -EAGAIN;
    }
    _i2c1_send_start_condition();  // Start condition
    _i2c1_wait_tx_buffer_empty();
    if (_i2c1_check_ack())
    {
        return -EIO;
    }
    _i2c1_wait_stop_condition();  // Stop condition

    __delay_ms(I2C_TIME_BETWEEN_OPS_MS);

    // Second i2c transaction: read data from slave
    I2C1CNT = rx_payload_size;  // Will expect to read rx_payload_size bytes
    _i2c1_set_device_address(self->slave_address, false);
    _i2c1_send_start_condition();  // Repeated start condition
    _i2c1_wait_tx_buffer_empty();
    if (_i2c1_check_ack())
    {
        return -EIO;
    }
    for (size_t i = 0; i < rx_payload_size - 1; i++)
    {
        _i2c1_wait_rx_buffer_full(&rx_payload[i]);
        if (_i2c1_check_ack())
        {
            return -EIO;
        }
    }
    _i2c1_send_nack();
    _i2c1_wait_stop_condition();  // Stop condition
    return 0;
}

/* ============================================================================================== */

/* PUBLIC */

/* ============================================================================================== */

int8_t i2c_read(struct i2c_port* self,
                uint8_t          reg_address,
                uint8_t*         rx_payload,
                size_t           rx_payload_size)
{
    atomic_lock();
    int8_t result = _i2c1_read(self, reg_address, rx_payload, rx_payload_size);
    atomic_unlock();
    __delay_ms(I2C_TIME_BETWEEN_OPS_MS);
    return result;
}

/* ============================================================================================== */

int8_t write(struct i2c_port* self,
             uint8_t          reg_address,
             const uint8_t*   tx_payload,
             size_t           tx_payload_size)
{
    atomic_lock();
    int8_t result = _i2c1_write(self, reg_address, tx_payload, tx_payload_size);
    atomic_unlock();
    __delay_ms(I2C_TIME_BETWEEN_OPS_MS);
    return result;
}

/* ============================================================================================== */
