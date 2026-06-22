/**
 * @file pic18_interrupts.c
 * @brief PIC18 interrupt driver implementation.
 *
 */

/* ========================================================================== */

#include "../inc/pic18_interrupts.h"

/* ========================================================================== */

const struct interrupt_ops pic18_interrupt_ops = {
    .enable_global        = pic18_global_interrupt_enable,
    .enable_low_priority  = pic18_low_priority_interrupt_enable,
    .enable_high_priority = pic18_high_priority_interrupt_enable,
    .atomic_lock          = pic18_interrupt_atomic_lock,
    .atomic_unlock        = pic18_interrupt_atomic_unlock,
};

/* ========================================================================== */

/**
 * @brief Counter for nested atomic lock calls.
 */
static volatile uint8_t interrupt_lock_count = 0;

/* ========================================================================== */

void pic18_global_interrupt_enable(bool enable)
{
    INTCON0bits.GIE = enable ? 1 : 0;
}

/* ========================================================================== */

void pic18_interrupt_atomic_lock(void)
{
    if (interrupt_lock_count == 0)
    {
        INTCON0bits.GIE = 0;
    }
    interrupt_lock_count += 1;
}

/* ========================================================================== */

void pic18_interrupt_atomic_unlock(void)
{
    if (interrupt_lock_count > 0)
    {
        interrupt_lock_count -= 1;
        if (interrupt_lock_count == 0)
        {
            INTCON0bits.GIE = 1;
        }
    }
}

/* ========================================================================== */

void pic18_low_priority_interrupt_enable(bool enable)
{
    INTCON0bits.GIEL = enable ? 1 : 0;
}

/* ========================================================================== */

void pic18_high_priority_interrupt_enable(bool enable)
{
    INTCON0bits.GIEH = enable ? 1 : 0;
}

/* ========================================================================== */

/**
 * @brief Table of registered ISR callbacks.
 */
static pic18_isr_callback_t isr_callbacks[PIC18_IRQ_COUNT] = {0};

/* ========================================================================== */

int8_t pic18_isr_register(
    pic18_irq_source_t source, pic18_isr_callback_t callback)
{
    if (source >= PIC18_IRQ_COUNT)
    {
        return -EINVAL;
    }
    isr_callbacks[source] = callback;
    return 0;
}

/* ========================================================================== */

void __interrupt(irq(default)) DefaultISR(void)
{
    if (PIR1)
    {
        /* ADC Conversion Complete */
        if (PIE1bits.ADIE && PIR1bits.ADIF)
        {
            PIR1bits.ADIF = 0;
            if (isr_callbacks[PIC18_IRQ_ADCC_CONVERSION] != NULL)
                isr_callbacks[PIC18_IRQ_ADCC_CONVERSION]();
        }
    }
    if (PIR2)
    {
        /* SPI1 RX */
        if (PIE2bits.SPI1RXIE && PIR2bits.SPI1RXIF)
        {
            PIR2bits.SPI1RXIF = 0;
            if (isr_callbacks[PIC18_IRQ_SPI1_RX] != NULL)
                isr_callbacks[PIC18_IRQ_SPI1_RX]();
        }

        /* SPI1 TX */
        if (PIE2bits.SPI1TXIE && PIR2bits.SPI1TXIF)
        {
            PIR2bits.SPI1TXIF = 0;
            if (isr_callbacks[PIC18_IRQ_SPI1_TX] != NULL)
                isr_callbacks[PIC18_IRQ_SPI1_TX]();
        }
    }
    if (PIR3)
    {
        /* UART1 RX */
        if (PIE3bits.U1RXIE && PIR3bits.U1RXIF)
        {
            PIR3bits.U1RXIF = 0;
            if (isr_callbacks[PIC18_IRQ_UART1_RX] != NULL)
                isr_callbacks[PIC18_IRQ_UART1_RX]();
        }
    }
    if (PIR4)
    {
        /* CCP1 */
        if (PIE4bits.CCP1IE && PIR4bits.CCP1IF)
        {
            /* Note: Flag cleared by callback (tnr_generator) */
            if (isr_callbacks[PIC18_IRQ_CCP1] != NULL)
                isr_callbacks[PIC18_IRQ_CCP1]();
        }

        /* TMR1 */
        if (PIE4bits.TMR1IE && PIR4bits.TMR1IF)
        {
            PIR4bits.TMR1IF = 0;
            if (isr_callbacks[PIC18_IRQ_TMR1] != NULL)
                isr_callbacks[PIC18_IRQ_TMR1]();
        }
    }
    if (PIR5)
    {
    }
    if (PIR6)
    {
        /* UART2 RX */
        if (PIE6bits.U2RXIE && PIR6bits.U2RXIF)
        {
            PIR6bits.U2RXIF = 0;
            if (isr_callbacks[PIC18_IRQ_UART2_RX] != NULL)
                isr_callbacks[PIC18_IRQ_UART2_RX]();
        }

        /* TMR3 */
        if (PIE6bits.TMR3IE && PIR6bits.TMR3IF)
        {
            /* Note: Flag cleared by callback (tnr_generator) */
            if (isr_callbacks[PIC18_IRQ_TMR3] != NULL)
                isr_callbacks[PIC18_IRQ_TMR3]();
        }
    }
    if (PIR7)
    {
        /* CCP2 */
        if (PIE7bits.CCP2IE && PIR7bits.CCP2IF)
        {
            /* Note: Flag cleared by callback (tnr_generator) */
            if (isr_callbacks[PIC18_IRQ_CCP2] != NULL)
                isr_callbacks[PIC18_IRQ_CCP2]();
        }
    }
    if (PIR8)
    {
        /* TMR5 */
        if (PIE8bits.TMR5IE && PIR8bits.TMR5IF)
        {
            /* Note: Flag cleared by callback (tnr_generator) */
            if (isr_callbacks[PIC18_IRQ_TMR5] != NULL)
                isr_callbacks[PIC18_IRQ_TMR5]();
        }
    }
    if (PIR9)
    {
        /* CCP3 */
        if (PIE9bits.CCP3IE && PIR9bits.CCP3IF)
        {
            /* Note: Flag cleared by callback (tnr_generator) */
            if (isr_callbacks[PIC18_IRQ_CCP3] != NULL)
                isr_callbacks[PIC18_IRQ_CCP3]();
        }
    }
}

/* ========================================================================== */
