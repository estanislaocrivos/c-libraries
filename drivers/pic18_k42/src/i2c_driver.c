#include "../inc/i2c_driver.h"

/* ============================================================================================== */

#define I2C_TIME_BETWEEN_OPS_MS 5
#define I2C_TIMEOUT_MS          500
#define I2C_WRITE_BIT           0
#define I2C_READ_BIT            1
#define I2C_DEFAULT_ADDRESS     0x2E /* Default address (ADT7467 controller address) */

/* ============================================================================================== */

static bool exit_operation = false;

static void i2c1_timeout_handler(void)
{
    exit_operation = true;
}

/* ============================================================================================== */

int8_t i2c1_initialize(void)
{
    tmr6_set_timeout_callback(i2c1_timeout_handler);

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
    LOG_DEBUG("I2C1: setting device address");
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
    if (I2C1STAT1bits.TXWE == 1)
    {
#ifdef LOG_ENABLE_LEV_2
        LOG_ERROR("I2C1: tx buffer was full while writing to it");
#endif
        I2C1STAT1bits.TXWE = 0;
        return -EAGAIN;
    }
    LOG_DEBUG("I2C1: wrote byte to tx buffer");
    return 0;
}

static inline void _i2c1_send_start_condition(void)
{
    LOG_DEBUG("I2C1: start condition asserted by master");
    I2C1CON0bits.S = 1;
}

static inline void _i2c1_wait_tx_buffer_empty(void)
{
    tmr6_set_timeout_ms(I2C_TIMEOUT_MS);
    while (I2C1STAT1bits.TXBE == 0)
    {
        LOG_DEBUG("I2C1: waiting for tx buffer to empty");
        if (exit_operation)
        {
#ifdef LOG_ENABLE_LEV_2
            LOG_ERROR("I2C1: timeout waiting for tx buffer to empty");
#endif
            exit_operation = false;
            tmr6_deact_timeout();
            return;
        }
    }
    tmr6_deact_timeout();
    LOG_DEBUG("I2C1: tx buffer empty");
}

static inline void _i2c1_wait_rx_buffer_full(uint8_t* data)
{
    tmr6_set_timeout_ms(I2C_TIMEOUT_MS);
    while (I2C1STAT1bits.RXBF == 0)
    {
        LOG_DEBUG("I2C1: waiting for rx buffer to fill");
        if (exit_operation)
        {
#ifdef LOG_ENABLE_LEV_2
            LOG_ERROR("I2C1: timeout waiting for rx buffer to fill");
#endif
            exit_operation = false;
            tmr6_deact_timeout();
            return;
        }
    }
    tmr6_deact_timeout();
    *data = I2C1RXB;
    LOG_DEBUG("I2C1: rx buffer full, read byte");
}

static inline int8_t _i2c1_check_ack(void)
{
    if (I2C1CON1bits.ACKSTAT == 1)
    {
#ifdef LOG_ENABLE_LEV_2
        LOG_ERROR("I2C1: NACK received");
#endif
        return -EIO;
    }
    LOG_DEBUG("I2C1: ACK received");
    return 0;
}

static inline void _i2c1_send_nack(void)
{
    I2C1CON1bits.ACKCNT = 1;  // Send NACK
    LOG_DEBUG("I2C1: NACK sent");
}

static inline void _i2c1_wait_stop_condition(void)
{
    tmr6_set_timeout_ms(I2C_TIMEOUT_MS);
    while (I2C1PIRbits.PCIF == 0)
    {
        if (exit_operation)
        {
#ifdef LOG_ENABLE_LEV_2
            LOG_ERROR("I2C1: timeout waiting for stop condition");
#endif
            exit_operation = false;
            tmr6_deact_timeout();
            return;
        }
    }
    tmr6_deact_timeout();
    LOG_DEBUG("I2C1: stop condition asserted by master");
}

/* ============================================================================================== */

static inline int8_t _i2c1_write_byte(uint8_t register_address, uint8_t data)
{
    LOG_DEBUG("I2C1: start of write byte op.");

    I2C1CNT = 2;  // Will send two bytes, one after another, the first is the register address, the
                  // second is the data
    _i2c1_set_device_address(I2C_DEFAULT_ADDRESS, true);
    LOG_DEBUG("I2C1: setting tx buffer with adt7467 register address");
    if (_i2c1_write_tx_buffer(register_address))
    {
        return -EAGAIN;
    }
    _i2c1_send_start_condition();
    _i2c1_wait_tx_buffer_empty();
    if (_i2c1_check_ack())
    {
        return -EIO;
    }
    LOG_DEBUG("I2C1: setting tx buffer with data byte");
    if (_i2c1_write_tx_buffer(data))
    {
        return -EAGAIN;
    }
    _i2c1_wait_tx_buffer_empty();
    if (_i2c1_check_ack())
    {
        return -EIO;
    }
    _i2c1_wait_stop_condition();
    return 0;
}

/* ============================================================================================== */

static inline int8_t i2c1_write_address(uint8_t register_address)
{
    LOG_DEBUG("I2C1: start of write address op.");
    I2C1CNT = 1;  // Will send one byte, the register address
    _i2c1_set_device_address(I2C_DEFAULT_ADDRESS, true);
    LOG_DEBUG("I2C1: setting tx buffer with adt7467 register address");
    if (_i2c1_write_tx_buffer(register_address))
    {
        return -EAGAIN;
    }
    _i2c1_send_start_condition();
    _i2c1_wait_tx_buffer_empty();
    if (_i2c1_check_ack())
    {
        return -EIO;
    }
    _i2c1_wait_stop_condition();
    return 0;
}

/* ============================================================================================== */

static inline int8_t _i2c1_read_byte(uint8_t register_address, uint8_t* data)
{
    LOG_DEBUG("I2C1: start of read byte op.");

    /* First, write reg. address to slave */
    if (i2c1_write_address(register_address) != 0)
    {
        /* Error writing address */
#ifdef LOG_ENABLE_LEV_2
        LOG_ERROR("I2C1: error writing address");
#endif
        *data = 0;
        return -EIO;
    }
    __delay_ms(I2C_TIME_BETWEEN_OPS_MS);

    LOG_DEBUG("I2C1: address written, proceeding to read data");

    /* Secondly, read byte from slave */
    I2C1CNT = 1;  // Will expect to read one byte
    _i2c1_set_device_address(I2C_DEFAULT_ADDRESS, false);
    _i2c1_send_start_condition();
    _i2c1_wait_tx_buffer_empty();
    _i2c1_check_ack();
    _i2c1_wait_rx_buffer_full(data);
    _i2c1_send_nack();
    _i2c1_wait_stop_condition();
    return 0;
}

/* ============================================================================================== */

/* PUBLIC */

/* ============================================================================================== */

int8_t i2c1_read_byte(uint8_t register_address, uint8_t* data)
{
    atomic_lock();
    int8_t result = _i2c1_read_byte(register_address, data);
    atomic_unlock();
    __delay_ms(I2C_TIME_BETWEEN_OPS_MS);
    return result;
}

/* ============================================================================================== */

int8_t i2c1_write_byte(uint8_t register_address, uint8_t data)
{
    atomic_lock();
    int8_t result = _i2c1_write_byte(register_address, data);
    atomic_unlock();
    __delay_ms(I2C_TIME_BETWEEN_OPS_MS);
    return result;
}

/* ============================================================================================== */
