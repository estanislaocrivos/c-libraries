#include "../inc/stm32f4xx_i2c.h"

#include "../../../libraries/inc/errno.h"
#include "stm32f4xx.h"

#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#define I2C_STANDARD_MAX_HZ 100000UL
#define I2C_FAST_MAX_HZ     400000UL

/* ========================================================================== */

struct i2c_ctx
{
    I2C_TypeDef* periph;
    uint32_t     pclk_hz;
};

static struct i2c_ctx g_i2c1_ctx = {I2C1, STM32F4XX_PCLK1_HZ};
static struct i2c_ctx g_i2c2_ctx = {I2C2, STM32F4XX_PCLK1_HZ};
static struct i2c_ctx g_i2c3_ctx = {I2C3, STM32F4XX_PCLK1_HZ};

/* ========================================================================== */

static void enable_periph_clock(const struct i2c_ctx* ctx)
{
    if (ctx->periph == I2C1)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    }
    else if (ctx->periph == I2C2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    }
    else if (ctx->periph == I2C3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    }
}

/* ========================================================================== */

/* Busy-wait for an SR1 flag. Returns -EIO on NACK, bus error, or arbitration
 * loss, clearing the error flags before returning. */
static int8_t wait_sr1(I2C_TypeDef* p, uint32_t flag)
{
    uint32_t sr1;
    do
    {
        sr1 = p->SR1;
        if (sr1 & (I2C_SR1_AF | I2C_SR1_BERR | I2C_SR1_ARLO))
        {
            p->SR1 = 0U;
            return -EIO;
        }
    } while (!(sr1 & flag));
    return 0;
}

/* ========================================================================== */

/* Generate START, send 7-bit address byte, wait for ADDR, clear ADDR.
 * dir: 0 = write, 1 = read. */
static int8_t send_start_addr(I2C_TypeDef* p, uint16_t slave_addr, uint8_t dir)
{
    while (p->SR2 & I2C_SR2_BUSY) {}

    p->CR1 |= I2C_CR1_START;

    int8_t err = wait_sr1(p, I2C_SR1_SB);
    if (err)
    {
        return err;
    }

    p->DR = (uint8_t)(((uint8_t)(slave_addr << 1)) | dir);

    err = wait_sr1(p, I2C_SR1_ADDR);
    if (err)
    {
        return err;
    }

    (void)p->SR1;
    (void)p->SR2;
    return 0;
}

/* ========================================================================== */

static int8_t initialize_ctx(struct i2c_ctx* ctx, struct i2c* s)
{
    if (s == NULL)
    {
        return -EINVAL;
    }
    if (!s->master)
    {
        return -ENOTSUP;
    }
    if (s->frequency == 0U || s->frequency > I2C_FAST_MAX_HZ)
    {
        return -EINVAL;
    }
    if (s->address_mode != I2C_ADDR_7BIT)
    {
        return -ENOTSUP;
    }

    enable_periph_clock(ctx);

    I2C_TypeDef* p = ctx->periph;

    p->CR1 &= ~I2C_CR1_PE;
    p->CR1 |= I2C_CR1_SWRST;
    p->CR1 &= ~I2C_CR1_SWRST;

    uint32_t pclk_mhz = ctx->pclk_hz / 1000000UL;
    p->CR2             = pclk_mhz & 0x3FU;

    if (s->frequency <= I2C_STANDARD_MAX_HZ)
    {
        uint32_t ccr = ctx->pclk_hz / (2U * s->frequency);
        if (ccr < 4U)
        {
            ccr = 4U;
        }
        p->CCR   = ccr & 0xFFFU;
        p->TRISE = pclk_mhz + 1U;
    }
    else
    {
        /* Fast mode, 2:1 duty cycle (DUTY=0): Thigh=CCR*Tpclk, Tlow=2*CCR*Tpclk */
        uint32_t ccr = ctx->pclk_hz / (3U * s->frequency);
        if (ccr < 1U)
        {
            ccr = 1U;
        }
        p->CCR   = I2C_CCR_FS | (ccr & 0xFFFU);
        /* Max rise time in fast mode is 300 ns */
        p->TRISE = pclk_mhz * 3U / 10U + 1U;
    }

    p->OAR1 = (uint32_t)((s->own_address & 0x7FU) << 1);
    p->CR1  |= I2C_CR1_PE | I2C_CR1_ACK;

    s->was_initialized = true;
    return 0;
}

/* ========================================================================== */

static int8_t write_ctx(
    const struct i2c_ctx* ctx,
    const struct i2c*     s,
    uint8_t               reg_address,
    const uint8_t*        tx_payload,
    size_t                tx_payload_size)
{
    if (tx_payload == NULL)
    {
        return -EFAULT;
    }
    if (tx_payload_size == 0U)
    {
        return -EINVAL;
    }

    I2C_TypeDef* p   = ctx->periph;
    int8_t       err = send_start_addr(p, s->slave_address, 0U);
    if (err)
    {
        return err;
    }

    p->DR = reg_address;
    err   = wait_sr1(p, I2C_SR1_TXE);
    if (err)
    {
        return err;
    }

    for (size_t i = 0U; i < tx_payload_size; i++)
    {
        p->DR = tx_payload[i];
        err   = wait_sr1(p, I2C_SR1_TXE);
        if (err)
        {
            return err;
        }
    }

    err = wait_sr1(p, I2C_SR1_BTF);
    if (err)
    {
        return err;
    }

    p->CR1 |= I2C_CR1_STOP;
    return 0;
}

/* ========================================================================== */

static int8_t read_ctx(
    const struct i2c_ctx* ctx,
    const struct i2c*     s,
    uint8_t               reg_address,
    uint8_t*              rx_payload,
    size_t                rx_payload_size)
{
    if (rx_payload == NULL)
    {
        return -EFAULT;
    }
    if (rx_payload_size == 0U)
    {
        return -EINVAL;
    }

    I2C_TypeDef* p   = ctx->periph;
    int8_t       err = send_start_addr(p, s->slave_address, 0U);
    if (err)
    {
        return err;
    }

    p->DR = reg_address;
    err   = wait_sr1(p, I2C_SR1_TXE);
    if (err)
    {
        return err;
    }

    /* Repeated START for the read phase */
    p->CR1 |= I2C_CR1_START;
    err = wait_sr1(p, I2C_SR1_SB);
    if (err)
    {
        return err;
    }

    p->DR = (uint8_t)(((uint8_t)(s->slave_address << 1)) | 0x01U);

    if (rx_payload_size == 1U)
    {
        /* Disable ACK before clearing ADDR so the NACK is sent after the byte */
        p->CR1 &= ~I2C_CR1_ACK;
        err = wait_sr1(p, I2C_SR1_ADDR);
        if (err)
        {
            p->CR1 |= I2C_CR1_ACK;
            return err;
        }
        (void)p->SR1;
        (void)p->SR2;
        p->CR1 |= I2C_CR1_STOP;
        err = wait_sr1(p, I2C_SR1_RXNE);
        if (err)
        {
            p->CR1 |= I2C_CR1_ACK;
            return err;
        }
        rx_payload[0] = (uint8_t)p->DR;
        p->CR1        |= I2C_CR1_ACK;
        return 0;
    }

    err = wait_sr1(p, I2C_SR1_ADDR);
    if (err)
    {
        return err;
    }
    (void)p->SR1;
    (void)p->SR2;

    for (size_t i = 0U; i < rx_payload_size; i++)
    {
        if (i == rx_payload_size - 1U)
        {
            p->CR1 &= ~I2C_CR1_ACK;
            p->CR1 |= I2C_CR1_STOP;
        }
        err = wait_sr1(p, I2C_SR1_RXNE);
        if (err)
        {
            p->CR1 |= I2C_CR1_ACK;
            return err;
        }
        rx_payload[i] = (uint8_t)p->DR;
    }

    p->CR1 |= I2C_CR1_ACK;
    return 0;
}

/* ========================================================================== */

static int8_t write_raw_ctx(
    const struct i2c_ctx* ctx,
    const struct i2c*     s,
    const uint8_t*        tx_payload,
    size_t                tx_payload_size)
{
    if (tx_payload == NULL)
    {
        return -EFAULT;
    }
    if (tx_payload_size == 0U)
    {
        return -EINVAL;
    }

    I2C_TypeDef* p   = ctx->periph;
    int8_t       err = send_start_addr(p, s->slave_address, 0U);
    if (err)
    {
        return err;
    }

    for (size_t i = 0U; i < tx_payload_size; i++)
    {
        p->DR = tx_payload[i];
        err   = wait_sr1(p, I2C_SR1_TXE);
        if (err)
        {
            return err;
        }
    }

    err = wait_sr1(p, I2C_SR1_BTF);
    if (err)
    {
        return err;
    }

    p->CR1 |= I2C_CR1_STOP;
    return 0;
}

/* ========================================================================== */

static int8_t read_raw_ctx(
    const struct i2c_ctx* ctx,
    const struct i2c*     s,
    uint8_t*              rx_payload,
    size_t                rx_payload_size)
{
    if (rx_payload == NULL)
    {
        return -EFAULT;
    }
    if (rx_payload_size == 0U)
    {
        return -EINVAL;
    }

    I2C_TypeDef* p   = ctx->periph;
    int8_t       err;

    if (rx_payload_size == 1U)
    {
        while (p->SR2 & I2C_SR2_BUSY) {}
        p->CR1 &= ~I2C_CR1_ACK;
        p->CR1 |= I2C_CR1_START;
        err = wait_sr1(p, I2C_SR1_SB);
        if (err)
        {
            p->CR1 |= I2C_CR1_ACK;
            return err;
        }
        p->DR = (uint8_t)(((uint8_t)(s->slave_address << 1)) | 0x01U);
        err   = wait_sr1(p, I2C_SR1_ADDR);
        if (err)
        {
            p->CR1 |= I2C_CR1_ACK;
            return err;
        }
        (void)p->SR1;
        (void)p->SR2;
        p->CR1 |= I2C_CR1_STOP;
        err = wait_sr1(p, I2C_SR1_RXNE);
        if (err)
        {
            p->CR1 |= I2C_CR1_ACK;
            return err;
        }
        rx_payload[0] = (uint8_t)p->DR;
        p->CR1        |= I2C_CR1_ACK;
        return 0;
    }

    err = send_start_addr(p, s->slave_address, 1U);
    if (err)
    {
        return err;
    }

    for (size_t i = 0U; i < rx_payload_size; i++)
    {
        if (i == rx_payload_size - 1U)
        {
            p->CR1 &= ~I2C_CR1_ACK;
            p->CR1 |= I2C_CR1_STOP;
        }
        err = wait_sr1(p, I2C_SR1_RXNE);
        if (err)
        {
            p->CR1 |= I2C_CR1_ACK;
            return err;
        }
        rx_payload[i] = (uint8_t)p->DR;
    }

    p->CR1 |= I2C_CR1_ACK;
    return 0;
}

/* ========================================================================== */

static int8_t probe_ctx(const struct i2c_ctx* ctx, const struct i2c* s)
{
    I2C_TypeDef* p = ctx->periph;

    while (p->SR2 & I2C_SR2_BUSY) {}

    p->CR1 |= I2C_CR1_START;

    int8_t err = wait_sr1(p, I2C_SR1_SB);
    if (err)
    {
        return err;
    }

    p->DR = (uint8_t)((uint8_t)(s->slave_address << 1));

    uint32_t sr1;
    do
    {
        sr1 = p->SR1;
        if (sr1 & I2C_SR1_AF)
        {
            p->SR1 &= ~I2C_SR1_AF;
            p->CR1 |= I2C_CR1_STOP;
            return -ENXIO;
        }
        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO))
        {
            p->SR1 = 0U;
            return -EIO;
        }
    } while (!(sr1 & I2C_SR1_ADDR));

    (void)p->SR1;
    (void)p->SR2;
    p->CR1 |= I2C_CR1_STOP;
    return 0;
}

/* ========================================================================== */
/* I2C1 op wrappers                                                           */

static int8_t i2c1_initialize(struct i2c* self)
{
    return initialize_ctx(&g_i2c1_ctx, self);
}

static int8_t i2c1_write(
    const struct i2c* self,
    uint8_t           reg_address,
    const uint8_t*    tx_payload,
    size_t            tx_payload_size)
{
    return write_ctx(&g_i2c1_ctx, self, reg_address, tx_payload, tx_payload_size);
}

static int8_t i2c1_read(
    const struct i2c* self,
    uint8_t           reg_address,
    uint8_t*          rx_payload,
    size_t            rx_payload_size)
{
    return read_ctx(&g_i2c1_ctx, self, reg_address, rx_payload, rx_payload_size);
}

static int8_t i2c1_write_raw(
    const struct i2c* self, const uint8_t* tx_payload, size_t tx_payload_size)
{
    return write_raw_ctx(&g_i2c1_ctx, self, tx_payload, tx_payload_size);
}

static int8_t i2c1_read_raw(
    const struct i2c* self, uint8_t* rx_payload, size_t rx_payload_size)
{
    return read_raw_ctx(&g_i2c1_ctx, self, rx_payload, rx_payload_size);
}

static int8_t i2c1_probe(const struct i2c* self)
{
    return probe_ctx(&g_i2c1_ctx, self);
}

const struct i2c_ops stm32f4xx_i2c1_ops = {
    .initialize = i2c1_initialize,
    .write      = i2c1_write,
    .read       = i2c1_read,
    .write_raw  = i2c1_write_raw,
    .read_raw   = i2c1_read_raw,
    .probe      = i2c1_probe,
};

/* ========================================================================== */
/* I2C2 op wrappers                                                           */

static int8_t i2c2_initialize(struct i2c* self)
{
    return initialize_ctx(&g_i2c2_ctx, self);
}

static int8_t i2c2_write(
    const struct i2c* self,
    uint8_t           reg_address,
    const uint8_t*    tx_payload,
    size_t            tx_payload_size)
{
    return write_ctx(&g_i2c2_ctx, self, reg_address, tx_payload, tx_payload_size);
}

static int8_t i2c2_read(
    const struct i2c* self,
    uint8_t           reg_address,
    uint8_t*          rx_payload,
    size_t            rx_payload_size)
{
    return read_ctx(&g_i2c2_ctx, self, reg_address, rx_payload, rx_payload_size);
}

static int8_t i2c2_write_raw(
    const struct i2c* self, const uint8_t* tx_payload, size_t tx_payload_size)
{
    return write_raw_ctx(&g_i2c2_ctx, self, tx_payload, tx_payload_size);
}

static int8_t i2c2_read_raw(
    const struct i2c* self, uint8_t* rx_payload, size_t rx_payload_size)
{
    return read_raw_ctx(&g_i2c2_ctx, self, rx_payload, rx_payload_size);
}

static int8_t i2c2_probe(const struct i2c* self)
{
    return probe_ctx(&g_i2c2_ctx, self);
}

const struct i2c_ops stm32f4xx_i2c2_ops = {
    .initialize = i2c2_initialize,
    .write      = i2c2_write,
    .read       = i2c2_read,
    .write_raw  = i2c2_write_raw,
    .read_raw   = i2c2_read_raw,
    .probe      = i2c2_probe,
};

/* ========================================================================== */
/* I2C3 op wrappers                                                           */

static int8_t i2c3_initialize(struct i2c* self)
{
    return initialize_ctx(&g_i2c3_ctx, self);
}

static int8_t i2c3_write(
    const struct i2c* self,
    uint8_t           reg_address,
    const uint8_t*    tx_payload,
    size_t            tx_payload_size)
{
    return write_ctx(&g_i2c3_ctx, self, reg_address, tx_payload, tx_payload_size);
}

static int8_t i2c3_read(
    const struct i2c* self,
    uint8_t           reg_address,
    uint8_t*          rx_payload,
    size_t            rx_payload_size)
{
    return read_ctx(&g_i2c3_ctx, self, reg_address, rx_payload, rx_payload_size);
}

static int8_t i2c3_write_raw(
    const struct i2c* self, const uint8_t* tx_payload, size_t tx_payload_size)
{
    return write_raw_ctx(&g_i2c3_ctx, self, tx_payload, tx_payload_size);
}

static int8_t i2c3_read_raw(
    const struct i2c* self, uint8_t* rx_payload, size_t rx_payload_size)
{
    return read_raw_ctx(&g_i2c3_ctx, self, rx_payload, rx_payload_size);
}

static int8_t i2c3_probe(const struct i2c* self)
{
    return probe_ctx(&g_i2c3_ctx, self);
}

const struct i2c_ops stm32f4xx_i2c3_ops = {
    .initialize = i2c3_initialize,
    .write      = i2c3_write,
    .read       = i2c3_read,
    .write_raw  = i2c3_write_raw,
    .read_raw   = i2c3_read_raw,
    .probe      = i2c3_probe,
};

/* ========================================================================== */
