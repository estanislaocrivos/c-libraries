#include "../inc/usart.h"

#include "../../../libraries/inc/errno.h"
#include "stm32f4xx.h"

#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

struct usart_ctx
{
    USART_TypeDef*       periph;
    uint32_t             pclk_hz;
    IRQn_Type            irqn;
    serial_rx_callback_t callback;
    void*                callback_context;
};

static struct usart_ctx g_usart1_ctx
    = {USART1, STM32F4XX_PCLK2_HZ, USART1_IRQn, NULL, NULL};
static struct usart_ctx g_usart2_ctx
    = {USART2, STM32F4XX_PCLK1_HZ, USART2_IRQn, NULL, NULL};
static struct usart_ctx g_usart6_ctx
    = {USART6, STM32F4XX_PCLK2_HZ, USART6_IRQn, NULL, NULL};

/* ========================================================================== */

static void enable_periph_clock(const struct usart_ctx* ctx)
{
    if (ctx->periph == USART1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if (ctx->periph == USART2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
    else if (ctx->periph == USART6)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    }
}

/* ========================================================================== */

static int8_t apply_frame_format(USART_TypeDef* periph, const struct serial* s)
{
    uint32_t cr1 = periph->CR1;
    uint32_t cr2 = periph->CR2;

    /* Word length: 8 -> M=0, 9 -> M=1. Anything else is unsupported. */
    if (s->data_bits == 8U)
    {
        cr1 &= ~USART_CR1_M;
    }
    else if (s->data_bits == 9U)
    {
        cr1 |= USART_CR1_M;
    }
    else
    {
        return -ENOTSUP;
    }

    /* Parity. */
    switch (s->parity)
    {
        case 'N':
            cr1 &= ~(USART_CR1_PCE | USART_CR1_PS);
            break;
        case 'E':
            cr1 |= USART_CR1_PCE;
            cr1 &= ~USART_CR1_PS;
            break;
        case 'O':
            cr1 |= USART_CR1_PCE | USART_CR1_PS;
            break;
        default:
            return -ENOTSUP;
    }

    /* Stop bits. STM32F4 USART supports 1 or 2 stop bits via CR2.STOP. */
    cr2 &= ~USART_CR2_STOP;
    if (s->stop_bits == 1U)
    {
        /* STOP = 00b */
    }
    else if (s->stop_bits == 2U)
    {
        cr2 |= USART_CR2_STOP_1; /* 10b */
    }
    else
    {
        return -ENOTSUP;
    }

    periph->CR1 = cr1;
    periph->CR2 = cr2;
    return 0;
}

/* ========================================================================== */

static int8_t initialize_ctx(struct usart_ctx* ctx, struct serial* s)
{
    if (s == NULL || s->baud_rate == 0U)
    {
        return -EINVAL;
    }

    enable_periph_clock(ctx);

    USART_TypeDef* p = ctx->periph;

    /* Disable while reconfiguring. */
    p->CR1 = 0U;
    p->CR2 = 0U;
    p->CR3 = 0U;

    /* OVER8 = 0 (16x oversampling). With that, BRR = fck / baud (rounded). */
    uint32_t brr = (ctx->pclk_hz + (s->baud_rate / 2U)) / s->baud_rate;
    if (brr < 16U || brr > 0xFFFFU)
    {
        return -EINVAL;
    }
    p->BRR = brr;

    int8_t err = apply_frame_format(p, s);
    if (err)
    {
        return err;
    }

    /* Enable USART, TX, RX. */
    p->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

    s->was_initialized = true;
    return 0;
}

/* ========================================================================== */

static int8_t transmit_ctx(
    const struct usart_ctx* ctx, const uint8_t* buffer, size_t size)
{
    if (buffer == NULL)
    {
        return -EFAULT;
    }
    if (size == 0U)
    {
        return -EINVAL;
    }
    USART_TypeDef* p = ctx->periph;
    for (size_t i = 0; i < size; ++i)
    {
        while (!(p->SR & USART_SR_TXE))
        {
        }
        p->DR = (uint16_t)buffer[i];
    }
    return 0;
}

/* ========================================================================== */

static int8_t receive_ctx(const struct usart_ctx* ctx, uint8_t* byte)
{
    if (byte == NULL)
    {
        return -EFAULT;
    }
    USART_TypeDef* p = ctx->periph;
    while (!(p->SR & USART_SR_RXNE))
    {
    }
    *byte = (uint8_t)(p->DR & 0xFFU);
    return 0;
}

/* ========================================================================== */

static int8_t set_rx_callback_ctx(
    struct usart_ctx*    ctx,
    serial_rx_callback_t callback,
    void*                callback_context)
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

static int8_t enable_rx_interrupt_ctx(struct usart_ctx* ctx, bool enable)
{
    if (enable)
    {
        ctx->periph->CR1 |= USART_CR1_RXNEIE;
        NVIC_EnableIRQ(ctx->irqn);
    }
    else
    {
        ctx->periph->CR1 &= ~USART_CR1_RXNEIE;
        NVIC_DisableIRQ(ctx->irqn);
    }
    return 0;
}

/* ========================================================================== */

static int8_t set_rx_interrupt_priority_ctx(
    const struct usart_ctx* ctx, uint8_t priority)
{
    NVIC_SetPriority(ctx->irqn, priority);
    return 0;
}

/* ========================================================================== */

static int8_t flush_tx_ctx(const struct usart_ctx* ctx)
{
    /* Wait for last frame to leave the shift register. */
    while (!(ctx->periph->SR & USART_SR_TC))
    {
    }
    return 0;
}

/* ========================================================================== */

static int8_t flush_rx_ctx(const struct usart_ctx* ctx)
{
    /* Drain RDR until RXNE is clear. STM32F4 USART has no FIFO, so at most
     * one byte is sitting in the data register. Reading DR also clears the
     * standard error flags after SR was read. */
    USART_TypeDef* p = ctx->periph;
    (void)p->SR;
    (void)p->DR;
    return 0;
}

/* ========================================================================== */
/* USART1 op wrappers */

static int8_t usart1_initialize(struct serial* self)
{
    return initialize_ctx(&g_usart1_ctx, self);
}

static int8_t usart1_transmit(
    const struct serial* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    return transmit_ctx(&g_usart1_ctx, buffer, size);
}

static int8_t usart1_receive(const struct serial* self, uint8_t* byte)
{
    (void)self;
    return receive_ctx(&g_usart1_ctx, byte);
}

static int8_t usart1_set_rx_callback(
    const struct serial* self,
    serial_rx_callback_t callback,
    void*                callback_context)
{
    (void)self;
    return set_rx_callback_ctx(&g_usart1_ctx, callback, callback_context);
}

static int8_t usart1_enable_rx_interrupt(const struct serial* self, bool enable)
{
    (void)self;
    return enable_rx_interrupt_ctx(&g_usart1_ctx, enable);
}

static int8_t usart1_set_rx_interrupt_priority(
    const struct serial* self, uint8_t priority)
{
    (void)self;
    return set_rx_interrupt_priority_ctx(&g_usart1_ctx, priority);
}

static int8_t usart1_flush_tx(const struct serial* self)
{
    (void)self;
    return flush_tx_ctx(&g_usart1_ctx);
}

static int8_t usart1_flush_rx(const struct serial* self)
{
    (void)self;
    return flush_rx_ctx(&g_usart1_ctx);
}

const struct serial_ops USART1_OPS = {
    .initialize                = usart1_initialize,
    .transmit                  = usart1_transmit,
    .receive                   = usart1_receive,
    .set_rx_callback           = usart1_set_rx_callback,
    .enable_rx_interrupt       = usart1_enable_rx_interrupt,
    .set_rx_interrupt_priority = usart1_set_rx_interrupt_priority,
    .flush_tx                  = usart1_flush_tx,
    .flush_rx                  = usart1_flush_rx,
};

/* ========================================================================== */
/* USART2 op wrappers */

static int8_t usart2_initialize(struct serial* self)
{
    return initialize_ctx(&g_usart2_ctx, self);
}

static int8_t usart2_transmit(
    const struct serial* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    return transmit_ctx(&g_usart2_ctx, buffer, size);
}

static int8_t usart2_receive(const struct serial* self, uint8_t* byte)
{
    (void)self;
    return receive_ctx(&g_usart2_ctx, byte);
}

static int8_t usart2_set_rx_callback(
    const struct serial* self,
    serial_rx_callback_t callback,
    void*                callback_context)
{
    (void)self;
    return set_rx_callback_ctx(&g_usart2_ctx, callback, callback_context);
}

static int8_t usart2_enable_rx_interrupt(const struct serial* self, bool enable)
{
    (void)self;
    return enable_rx_interrupt_ctx(&g_usart2_ctx, enable);
}

static int8_t usart2_set_rx_interrupt_priority(
    const struct serial* self, uint8_t priority)
{
    (void)self;
    return set_rx_interrupt_priority_ctx(&g_usart2_ctx, priority);
}

static int8_t usart2_flush_tx(const struct serial* self)
{
    (void)self;
    return flush_tx_ctx(&g_usart2_ctx);
}

static int8_t usart2_flush_rx(const struct serial* self)
{
    (void)self;
    return flush_rx_ctx(&g_usart2_ctx);
}

const struct serial_ops USART2_OPS = {
    .initialize                = usart2_initialize,
    .transmit                  = usart2_transmit,
    .receive                   = usart2_receive,
    .set_rx_callback           = usart2_set_rx_callback,
    .enable_rx_interrupt       = usart2_enable_rx_interrupt,
    .set_rx_interrupt_priority = usart2_set_rx_interrupt_priority,
    .flush_tx                  = usart2_flush_tx,
    .flush_rx                  = usart2_flush_rx,
};

/* ========================================================================== */
/* USART6 op wrappers */

static int8_t usart6_initialize(struct serial* self)
{
    return initialize_ctx(&g_usart6_ctx, self);
}

static int8_t usart6_transmit(
    const struct serial* self, const uint8_t* buffer, size_t size)
{
    (void)self;
    return transmit_ctx(&g_usart6_ctx, buffer, size);
}

static int8_t usart6_receive(const struct serial* self, uint8_t* byte)
{
    (void)self;
    return receive_ctx(&g_usart6_ctx, byte);
}

static int8_t usart6_set_rx_callback(
    const struct serial* self,
    serial_rx_callback_t callback,
    void*                callback_context)
{
    (void)self;
    return set_rx_callback_ctx(&g_usart6_ctx, callback, callback_context);
}

static int8_t usart6_enable_rx_interrupt(const struct serial* self, bool enable)
{
    (void)self;
    return enable_rx_interrupt_ctx(&g_usart6_ctx, enable);
}

static int8_t usart6_set_rx_interrupt_priority(
    const struct serial* self, uint8_t priority)
{
    (void)self;
    return set_rx_interrupt_priority_ctx(&g_usart6_ctx, priority);
}

static int8_t usart6_flush_tx(const struct serial* self)
{
    (void)self;
    return flush_tx_ctx(&g_usart6_ctx);
}

static int8_t usart6_flush_rx(const struct serial* self)
{
    (void)self;
    return flush_rx_ctx(&g_usart6_ctx);
}

const struct serial_ops USART6_OPS = {
    .initialize                = usart6_initialize,
    .transmit                  = usart6_transmit,
    .receive                   = usart6_receive,
    .set_rx_callback           = usart6_set_rx_callback,
    .enable_rx_interrupt       = usart6_enable_rx_interrupt,
    .set_rx_interrupt_priority = usart6_set_rx_interrupt_priority,
    .flush_tx                  = usart6_flush_tx,
    .flush_rx                  = usart6_flush_rx,
};

/* ========================================================================== */
/* IRQ handlers. CMSIS-Device declares these as weak in startup code, so
 * defining them here overrides the defaults. They are invoked by the NVIC
 * when the corresponding USART raises an interrupt. */

static inline void handle_rx_isr(struct usart_ctx* ctx)
{
    USART_TypeDef* p = ctx->periph;
    if (p->SR & USART_SR_RXNE)
    {
        uint8_t data = (uint8_t)(p->DR & 0xFFU);
        if (ctx->callback != NULL)
        {
            ctx->callback(ctx->callback_context, data);
        }
    }
}

void USART1_IRQHandler(void)
{
    handle_rx_isr(&g_usart1_ctx);
}

void USART2_IRQHandler(void)
{
    handle_rx_isr(&g_usart2_ctx);
}

void USART6_IRQHandler(void)
{
    handle_rx_isr(&g_usart6_ctx);
}

/* ========================================================================== */
