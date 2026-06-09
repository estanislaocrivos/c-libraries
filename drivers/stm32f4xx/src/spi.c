#include "../inc/spi.h"

#include "../../../libraries/inc/errno.h"
#include "stm32f4xx.h"

#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#define SPI_BR_MASK    (0x7U << 3)
#define SPI_BR_MAX     7U
#define SPI_DUMMY_BYTE 0xFFU
#define SPI_MIN_DIV    2U
#define SPI_MAX_DIV    256U

/* ========================================================================== */

struct spi_ctx
{
    SPI_TypeDef*      periph;
    uint32_t          pclk_hz;
    IRQn_Type         irqn;
    spi_rx_callback_t callback;
    void*             callback_context;
};

static struct spi_ctx g_spi1_ctx
    = {SPI1, STM32F4XX_PCLK2_HZ, SPI1_IRQn, NULL, NULL};
static struct spi_ctx g_spi2_ctx
    = {SPI2, STM32F4XX_PCLK1_HZ, SPI2_IRQn, NULL, NULL};
static struct spi_ctx g_spi3_ctx
    = {SPI3, STM32F4XX_PCLK1_HZ, SPI3_IRQn, NULL, NULL};
static struct spi_ctx g_spi4_ctx
    = {SPI4, STM32F4XX_PCLK2_HZ, SPI4_IRQn, NULL, NULL};

/* ========================================================================== */

static void enable_periph_clock(const struct spi_ctx* ctx)
{
    if (ctx->periph == SPI1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    }
    else if (ctx->periph == SPI2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    }
    else if (ctx->periph == SPI3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
    }
    else if (ctx->periph == SPI4)
    {
        RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
    }
}

/* ========================================================================== */

/* Returns the smallest BR (prescaler) index whose resulting bit rate is less
 * than or equal to target_freq_hz. STM32F4 supports divisors 2..256 in
 * powers of 2, encoded as BR = 0..7. */
static int8_t calculate_br(
    uint32_t pclk_hz, uint32_t target_freq_hz, uint8_t* br_out)
{
    if (target_freq_hz == 0U)
    {
        return -EINVAL;
    }
    if (target_freq_hz > pclk_hz / SPI_MIN_DIV)
    {
        return -EINVAL;
    }
    if (target_freq_hz < pclk_hz / SPI_MAX_DIV)
    {
        return -EINVAL;
    }
    uint8_t  br  = 0;
    uint32_t div = SPI_MIN_DIV;
    while ((pclk_hz / div) > target_freq_hz && br < SPI_BR_MAX)
    {
        br++;
        div <<= 1;
    }
    *br_out = br;
    return 0;
}

/* ========================================================================== */

/* Access DR as 8-bit to avoid 16-bit reads/writes when DFF=0. This is the
 * canonical gotcha of the STM32F4 SPI: a 16-bit read of DR while DFF=0
 * returns two RXNE events worth of data, corrupting the byte stream. */
static inline void dr_write_byte(SPI_TypeDef* p, uint8_t value)
{
    *((volatile uint8_t*)&p->DR) = value;
}

static inline uint8_t dr_read_byte(SPI_TypeDef* p)
{
    return *((volatile uint8_t*)&p->DR);
}

/* ========================================================================== */

static int8_t initialize_ctx(struct spi_ctx* ctx, struct spi* s)
{
    if (s == NULL)
    {
        return -EINVAL;
    }
    if (!s->master)
    {
        return -ENOTSUP;
    }
    if (s->mode > 3U)
    {
        return -EINVAL;
    }

    uint8_t br;
    int8_t  err = calculate_br(ctx->pclk_hz, s->frequency, &br);
    if (err)
    {
        return err;
    }

    enable_periph_clock(ctx);

    SPI_TypeDef* p = ctx->periph;
    p->CR1         = 0U;
    p->CR2         = 0U;

    uint32_t cr1 = 0U;
    cr1 |= ((uint32_t)br << 3) & SPI_BR_MASK;
    cr1 |= SPI_CR1_MSTR;
    cr1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    if ((s->mode & 0x2U) != 0U)
    {
        cr1 |= SPI_CR1_CPOL;
    }
    if ((s->mode & 0x1U) != 0U)
    {
        cr1 |= SPI_CR1_CPHA;
    }
    if (!s->msb_first)
    {
        cr1 |= SPI_CR1_LSBFIRST;
    }
    cr1 |= SPI_CR1_SPE;
    p->CR1 = cr1;

    s->was_initialized = true;
    return 0;
}

/* ========================================================================== */

static int8_t transmit_ctx(
    const struct spi_ctx* ctx, const uint8_t* buffer, size_t size)
{
    if (buffer == NULL)
    {
        return -EFAULT;
    }
    if (size == 0U)
    {
        return -EINVAL;
    }
    SPI_TypeDef* p = ctx->periph;
    for (size_t i = 0; i < size; ++i)
    {
        while (!(p->SR & SPI_SR_TXE))
        {
        }
        dr_write_byte(p, buffer[i]);
        while (!(p->SR & SPI_SR_RXNE))
        {
        }
        (void)dr_read_byte(p);
    }
    while (p->SR & SPI_SR_BSY)
    {
    }
    return 0;
}

/* ========================================================================== */

static int8_t receive_ctx(const struct spi_ctx* ctx, uint8_t* byte)
{
    if (byte == NULL)
    {
        return -EFAULT;
    }
    SPI_TypeDef* p = ctx->periph;
    while (!(p->SR & SPI_SR_TXE))
    {
    }
    dr_write_byte(p, SPI_DUMMY_BYTE);
    while (!(p->SR & SPI_SR_RXNE))
    {
    }
    *byte = dr_read_byte(p);
    while (p->SR & SPI_SR_BSY)
    {
    }
    return 0;
}

/* ========================================================================== */

static int8_t transfer_ctx(
    const struct spi_ctx* ctx,
    const uint8_t*        tx_buffer,
    uint8_t*              rx_buffer,
    size_t                size)
{
    if (tx_buffer == NULL || rx_buffer == NULL)
    {
        return -EFAULT;
    }
    if (size == 0U)
    {
        return -EINVAL;
    }
    SPI_TypeDef* p = ctx->periph;
    for (size_t i = 0; i < size; ++i)
    {
        while (!(p->SR & SPI_SR_TXE))
        {
        }
        dr_write_byte(p, tx_buffer[i]);
        while (!(p->SR & SPI_SR_RXNE))
        {
        }
        rx_buffer[i] = dr_read_byte(p);
    }
    while (p->SR & SPI_SR_BSY)
    {
    }
    return 0;
}

/* ========================================================================== */

static int8_t set_cs_ctx(const struct spi_ctx* ctx, bool active)
{
    (void)ctx;
    (void)active;
    /* CS is software-managed by the caller via GPIO. The driver does not
     * know which pin is wired as CS. */
    return -ENOTSUP;
}

/* ========================================================================== */

static int8_t set_frequency_ctx(const struct spi_ctx* ctx, uint32_t frequency)
{
    uint8_t br;
    int8_t  err = calculate_br(ctx->pclk_hz, frequency, &br);
    if (err)
    {
        return err;
    }
    SPI_TypeDef* p   = ctx->periph;
    uint32_t     cr1 = p->CR1;
    cr1 &= ~SPI_CR1_SPE;
    p->CR1 = cr1;
    cr1 &= ~SPI_BR_MASK;
    cr1 |= ((uint32_t)br << 3) & SPI_BR_MASK;
    cr1 |= SPI_CR1_SPE;
    p->CR1 = cr1;
    return 0;
}

/* ========================================================================== */

static int8_t set_rx_callback_ctx(
    struct spi_ctx* ctx, spi_rx_callback_t callback, void* callback_context)
{
    if (callback == NULL)
    {
        return -EFAULT;
    }
    ctx->callback         = callback;
    ctx->callback_context = callback_context;
    return 0;
}

/* ========================================================================== */

static int8_t enable_rx_interrupt_ctx(struct spi_ctx* ctx, bool enable)
{
    if (enable)
    {
        ctx->periph->CR2 |= SPI_CR2_RXNEIE;
        NVIC_EnableIRQ(ctx->irqn);
    }
    else
    {
        ctx->periph->CR2 &= ~SPI_CR2_RXNEIE;
        NVIC_DisableIRQ(ctx->irqn);
    }
    return 0;
}

/* ========================================================================== */

static int8_t flush_tx_ctx(const struct spi_ctx* ctx)
{
    SPI_TypeDef* p = ctx->periph;
    while (!(p->SR & SPI_SR_TXE))
    {
    }
    while (p->SR & SPI_SR_BSY)
    {
    }
    return 0;
}

/* ========================================================================== */

static int8_t flush_rx_ctx(const struct spi_ctx* ctx)
{
    SPI_TypeDef* p = ctx->periph;
    /* Drain RXNE if pending. STM32F4 SPI has no FIFO, so at most one byte
     * is sitting in DR. */
    if (p->SR & SPI_SR_RXNE)
    {
        (void)dr_read_byte(p);
    }
    /* Clear OVR if set: read DR then SR (per reference manual). */
    if (p->SR & SPI_SR_OVR)
    {
        (void)dr_read_byte(p);
        (void)p->SR;
    }
    return 0;
}

/* ========================================================================== */
/* SPI1 op wrappers */

static int8_t spi1_initialize(struct spi* self)
{
    return initialize_ctx(&g_spi1_ctx, self);
}

static int8_t spi1_transmit(
    const struct spi* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    return transmit_ctx(&g_spi1_ctx, buffer, size);
}

static int8_t spi1_receive(const struct spi* self, uint8_t* byte)
{
    (void)self;
    return receive_ctx(&g_spi1_ctx, byte);
}

static int8_t spi1_transfer(
    const struct spi* self,
    const uint8_t*    tx_buffer,
    uint8_t*          rx_buffer,
    size_t            size)
{
    (void)self;
    return transfer_ctx(&g_spi1_ctx, tx_buffer, rx_buffer, size);
}

static int8_t spi1_set_cs(const struct spi* self, bool active)
{
    (void)self;
    return set_cs_ctx(&g_spi1_ctx, active);
}

static int8_t spi1_set_frequency(const struct spi* self, uint32_t frequency)
{
    (void)self;
    return set_frequency_ctx(&g_spi1_ctx, frequency);
}

static int8_t spi1_set_rx_callback(
    const struct spi* self, spi_rx_callback_t callback, void* callback_context)
{
    (void)self;
    return set_rx_callback_ctx(&g_spi1_ctx, callback, callback_context);
}

static int8_t spi1_enable_rx_interrupt(const struct spi* self, bool enable)
{
    (void)self;
    return enable_rx_interrupt_ctx(&g_spi1_ctx, enable);
}

static int8_t spi1_flush_tx(const struct spi* self)
{
    (void)self;
    return flush_tx_ctx(&g_spi1_ctx);
}

static int8_t spi1_flush_rx(const struct spi* self)
{
    (void)self;
    return flush_rx_ctx(&g_spi1_ctx);
}

const struct spi_ops SPI1_OPS = {
    .initialize          = spi1_initialize,
    .transmit            = spi1_transmit,
    .receive             = spi1_receive,
    .transfer            = spi1_transfer,
    .set_cs              = spi1_set_cs,
    .set_frequency       = spi1_set_frequency,
    .set_rx_callback     = spi1_set_rx_callback,
    .enable_rx_interrupt = spi1_enable_rx_interrupt,
    .flush_tx            = spi1_flush_tx,
    .flush_rx            = spi1_flush_rx,
};

/* ========================================================================== */
/* SPI2 op wrappers */

static int8_t spi2_initialize(struct spi* self)
{
    return initialize_ctx(&g_spi2_ctx, self);
}

static int8_t spi2_transmit(
    const struct spi* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    return transmit_ctx(&g_spi2_ctx, buffer, size);
}

static int8_t spi2_receive(const struct spi* self, uint8_t* byte)
{
    (void)self;
    return receive_ctx(&g_spi2_ctx, byte);
}

static int8_t spi2_transfer(
    const struct spi* self,
    const uint8_t*    tx_buffer,
    uint8_t*          rx_buffer,
    size_t            size)
{
    (void)self;
    return transfer_ctx(&g_spi2_ctx, tx_buffer, rx_buffer, size);
}

static int8_t spi2_set_cs(const struct spi* self, bool active)
{
    (void)self;
    return set_cs_ctx(&g_spi2_ctx, active);
}

static int8_t spi2_set_frequency(const struct spi* self, uint32_t frequency)
{
    (void)self;
    return set_frequency_ctx(&g_spi2_ctx, frequency);
}

static int8_t spi2_set_rx_callback(
    const struct spi* self, spi_rx_callback_t callback, void* callback_context)
{
    (void)self;
    return set_rx_callback_ctx(&g_spi2_ctx, callback, callback_context);
}

static int8_t spi2_enable_rx_interrupt(const struct spi* self, bool enable)
{
    (void)self;
    return enable_rx_interrupt_ctx(&g_spi2_ctx, enable);
}

static int8_t spi2_flush_tx(const struct spi* self)
{
    (void)self;
    return flush_tx_ctx(&g_spi2_ctx);
}

static int8_t spi2_flush_rx(const struct spi* self)
{
    (void)self;
    return flush_rx_ctx(&g_spi2_ctx);
}

const struct spi_ops SPI2_OPS = {
    .initialize          = spi2_initialize,
    .transmit            = spi2_transmit,
    .receive             = spi2_receive,
    .transfer            = spi2_transfer,
    .set_cs              = spi2_set_cs,
    .set_frequency       = spi2_set_frequency,
    .set_rx_callback     = spi2_set_rx_callback,
    .enable_rx_interrupt = spi2_enable_rx_interrupt,
    .flush_tx            = spi2_flush_tx,
    .flush_rx            = spi2_flush_rx,
};

/* ========================================================================== */
/* SPI3 op wrappers */

static int8_t spi3_initialize(struct spi* self)
{
    return initialize_ctx(&g_spi3_ctx, self);
}

static int8_t spi3_transmit(
    const struct spi* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    return transmit_ctx(&g_spi3_ctx, buffer, size);
}

static int8_t spi3_receive(const struct spi* self, uint8_t* byte)
{
    (void)self;
    return receive_ctx(&g_spi3_ctx, byte);
}

static int8_t spi3_transfer(
    const struct spi* self,
    const uint8_t*    tx_buffer,
    uint8_t*          rx_buffer,
    size_t            size)
{
    (void)self;
    return transfer_ctx(&g_spi3_ctx, tx_buffer, rx_buffer, size);
}

static int8_t spi3_set_cs(const struct spi* self, bool active)
{
    (void)self;
    return set_cs_ctx(&g_spi3_ctx, active);
}

static int8_t spi3_set_frequency(const struct spi* self, uint32_t frequency)
{
    (void)self;
    return set_frequency_ctx(&g_spi3_ctx, frequency);
}

static int8_t spi3_set_rx_callback(
    const struct spi* self, spi_rx_callback_t callback, void* callback_context)
{
    (void)self;
    return set_rx_callback_ctx(&g_spi3_ctx, callback, callback_context);
}

static int8_t spi3_enable_rx_interrupt(const struct spi* self, bool enable)
{
    (void)self;
    return enable_rx_interrupt_ctx(&g_spi3_ctx, enable);
}

static int8_t spi3_flush_tx(const struct spi* self)
{
    (void)self;
    return flush_tx_ctx(&g_spi3_ctx);
}

static int8_t spi3_flush_rx(const struct spi* self)
{
    (void)self;
    return flush_rx_ctx(&g_spi3_ctx);
}

const struct spi_ops SPI3_OPS = {
    .initialize          = spi3_initialize,
    .transmit            = spi3_transmit,
    .receive             = spi3_receive,
    .transfer            = spi3_transfer,
    .set_cs              = spi3_set_cs,
    .set_frequency       = spi3_set_frequency,
    .set_rx_callback     = spi3_set_rx_callback,
    .enable_rx_interrupt = spi3_enable_rx_interrupt,
    .flush_tx            = spi3_flush_tx,
    .flush_rx            = spi3_flush_rx,
};

/* ========================================================================== */
/* SPI4 op wrappers */

static int8_t spi4_initialize(struct spi* self)
{
    return initialize_ctx(&g_spi4_ctx, self);
}

static int8_t spi4_transmit(
    const struct spi* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    return transmit_ctx(&g_spi4_ctx, buffer, size);
}

static int8_t spi4_receive(const struct spi* self, uint8_t* byte)
{
    (void)self;
    return receive_ctx(&g_spi4_ctx, byte);
}

static int8_t spi4_transfer(
    const struct spi* self,
    const uint8_t*    tx_buffer,
    uint8_t*          rx_buffer,
    size_t            size)
{
    (void)self;
    return transfer_ctx(&g_spi4_ctx, tx_buffer, rx_buffer, size);
}

static int8_t spi4_set_cs(const struct spi* self, bool active)
{
    (void)self;
    return set_cs_ctx(&g_spi4_ctx, active);
}

static int8_t spi4_set_frequency(const struct spi* self, uint32_t frequency)
{
    (void)self;
    return set_frequency_ctx(&g_spi4_ctx, frequency);
}

static int8_t spi4_set_rx_callback(
    const struct spi* self, spi_rx_callback_t callback, void* callback_context)
{
    (void)self;
    return set_rx_callback_ctx(&g_spi4_ctx, callback, callback_context);
}

static int8_t spi4_enable_rx_interrupt(const struct spi* self, bool enable)
{
    (void)self;
    return enable_rx_interrupt_ctx(&g_spi4_ctx, enable);
}

static int8_t spi4_flush_tx(const struct spi* self)
{
    (void)self;
    return flush_tx_ctx(&g_spi4_ctx);
}

static int8_t spi4_flush_rx(const struct spi* self)
{
    (void)self;
    return flush_rx_ctx(&g_spi4_ctx);
}

const struct spi_ops SPI4_OPS = {
    .initialize          = spi4_initialize,
    .transmit            = spi4_transmit,
    .receive             = spi4_receive,
    .transfer            = spi4_transfer,
    .set_cs              = spi4_set_cs,
    .set_frequency       = spi4_set_frequency,
    .set_rx_callback     = spi4_set_rx_callback,
    .enable_rx_interrupt = spi4_enable_rx_interrupt,
    .flush_tx            = spi4_flush_tx,
    .flush_rx            = spi4_flush_rx,
};

/* ========================================================================== */
/* IRQ handlers. CMSIS-Device declares these as weak in startup code, so
 * defining them here overrides the defaults. Invoked by the NVIC when the
 * corresponding SPI raises an interrupt. */

static inline void handle_rx_isr(struct spi_ctx* ctx)
{
    SPI_TypeDef* p = ctx->periph;
    if (p->SR & SPI_SR_RXNE)
    {
        uint8_t data = dr_read_byte(p);
        if (ctx->callback != NULL)
        {
            ctx->callback(ctx->callback_context, data);
        }
    }
}

void SPI1_IRQHandler(void)
{
    handle_rx_isr(&g_spi1_ctx);
}

void SPI2_IRQHandler(void)
{
    handle_rx_isr(&g_spi2_ctx);
}

void SPI3_IRQHandler(void)
{
    handle_rx_isr(&g_spi3_ctx);
}

void SPI4_IRQHandler(void)
{
    handle_rx_isr(&g_spi4_ctx);
}

/* ========================================================================== */
