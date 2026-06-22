#include "../inc/pic18_adcc.h"

#include "../inc/pic18_gpio.h"
#include "../inc/pic18_interrupts.h"
#include "errno.h"

#include <stdint.h>

/* ========================================================================== */

const struct adc_ops pic18_adcc_ops = {
    .initialize              = pic18_adcc_initialize,
    .read                    = pic18_adcc_read,
    .start_conversion        = pic18_adcc_start_conversion,
    .stop_conversion         = pic18_adcc_stop_conversion,
    .is_conversion_complete  = pic18_adcc_is_conversion_complete,
    .get_value               = pic18_adcc_get_value,
    .set_conversion_callback = pic18_adcc_set_conversion_callback,
    .enable_conv_interrupt   = pic18_adcc_enable_conv_interrupt,
};

/* ========================================================================== */

static adc_conversion_callback_t pic18_adcc_conversion_callback = NULL;
static void*                     pic18_adcc_callback_context    = NULL;

/*
 * Active computation mode, cached at init.
 * Drives whether the read path returns ADRES (basic) or ADFLTR (computed).
 */
static uint8_t pic18_adcc_active_mode = PIC18_ADCC_MODE_BASIC;

/* ISR handler definition at the end of the file */
static inline void pic18_adcc_isr_handler(void);

/* ========================================================================== */

/**
 * @brief Configures the voltage reference based on the volt_reference
 * setting.
 * @param volt_reference Voltage reference selection from enum adc_vref.
 */
static void pic18_adcc_configure_vref(enum adc_vref volt_reference)
{
    switch (volt_reference)
    {
        case ADC_VREF_FVR_1024:
            /* Enable FVR with 1.024V for ADC */
            FVRCONbits.ADFVR = 0b01;
            FVRCONbits.FVREN = 1;
            /* Wait for FVR to stabilize */
            while (!FVRCONbits.FVRRDY)
            {
            }
            ADREFbits.ADPREF = 0b11; /* FVR as positive reference */
            break;

        case ADC_VREF_FVR_2048:
            FVRCONbits.ADFVR = 0b10;
            FVRCONbits.FVREN = 1;
            while (!FVRCONbits.FVRRDY)
            {
            }
            ADREFbits.ADPREF = 0b11;
            break;

        case ADC_VREF_FVR_4096:
            FVRCONbits.ADFVR = 0b11;
            FVRCONbits.FVREN = 1;
            while (!FVRCONbits.FVRRDY)
            {
            }
            ADREFbits.ADPREF = 0b11;
            break;

        case ADC_VREF_EXTERNAL:
            ADREFbits.ADPREF = 0b10; /* External VREF+ pin */
            break;

        case ADC_VREF_VDD:
        default:
            ADREFbits.ADPREF = 0b00; /* VDD as positive reference */
            break;
    }

    /* VSS as negative reference */
    ADREFbits.ADNREF = 0;
}

/* ========================================================================== */

/**
 * @brief Checks if a conversion is currently in progress.
 * @return true if conversion is in progress, false otherwise.
 */
static inline bool pic18_adcc_is_busy(void)
{
    return ADCON0bits.ADGO == 1;
}

/* ========================================================================== */

/**
 * @brief Waits for the current conversion to complete.
 * @note Polls the ADGO bit which is cleared by hardware when conversion ends.
 */
static inline void pic18_adcc_wait_conversion(void)
{
    while (ADCON0bits.ADGO)
    {
    }
}

/* ========================================================================== */

/**
 * @brief Reads the conversion result.
 *
 * In BASIC mode the result is the raw 12-bit value from ADRES. In any
 * computation mode (accumulate / average / burst average / low-pass) the
 * hardware leaves the computed value in ADFLTR, so we read that instead.
 *
 * @return The 16-bit, right-justified result word.
 */
static inline uint16_t pic18_adcc_read_result(void)
{
    if (pic18_adcc_active_mode == PIC18_ADCC_MODE_BASIC)
    {
        return (uint16_t)(((uint16_t)(ADRESH) << 8) | ADRESL);
    }
    return (uint16_t)(((uint16_t)(ADFLTRH) << 8) | ADFLTRL);
}

/* ========================================================================== */

/**
 * @brief Clears ADC interrupt flags.
 */
static inline void pic18_adcc_clear_flags(void)
{
    PIR1bits.ADIF  = 0;
    PIR1bits.ADTIF = 0;
}

/* ========================================================================== */

int8_t pic18_adcc_initialize(struct adc* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Disable ADC during configuration */
    ADCON0bits.ADON = 0;

    /* Clear all ADC registers to known state */
    ADLTHL  = 0x00;
    ADLTHH  = 0x00;
    ADUTHL  = 0x00;
    ADUTHH  = 0x00;
    ADSTPTL = 0x00;
    ADSTPTH = 0x00;
    ADACCU  = 0x00;
    ADRPT   = 0x00;
    ADPCH   = 0x00;
    ADCAP   = 0x00;
    ADPREL  = 0x00;
    ADPREH  = 0x00;
    ADCON1  = 0x00;
    ADCON2  = 0x00;
    ADCON3  = 0x00;
    ADSTAT  = 0x00;
    ADACT   = 0x00;

    /* Configure acquisition time from self->sampling_time */
    uint16_t acq_time = self->sampling_time;
    if (acq_time == 0)
    {
        acq_time = PIC18_ADCC_DEFAULT_ACQ_TIME;
    }
    ADACQL = (uint8_t)(acq_time & 0xFF);
    ADACQH = (uint8_t)((acq_time >> 8) & 0xFF);

    /* Configure voltage reference */
    pic18_adcc_configure_vref(self->volt_reference);

    /*
     * Configure ADC clock:
     * ADCLK register sets the clock divider: FOSC / (2 * (ADCS + 1))
     * For reliable operation, ADC clock should be <= 4MHz (Tad >= 250ns)
     *
     * Common configurations:
     * - FOSC = 64MHz: ADCS = 15 -> FOSC/32 = 2MHz (Tad = 500ns)
     * - FOSC = 32MHz: ADCS = 15 -> FOSC/32 = 1MHz (Tad = 1us)
     * - FOSC = 16MHz: ADCS = 7  -> FOSC/16 = 1MHz (Tad = 1us)
     */
    ADCLK = 0x0F; /* FOSC/32 - safe default for most clock frequencies */

    /*
     * ADCON0 configuration:
     * - ADON: Will be enabled at the end
     * - ADCONT: 0 = Single conversion mode
     * - ADCS: 0 = Clock from ADCLK register (FOSC-derived)
     * - ADFM: 1 = Right-justified result
     * - ADGO: 0 = Conversion not started
     */
    ADCON0          = 0x00;
    ADCON0bits.ADFM = 1; /* Right-justified result */

    /*
     * ADCON2 configuration (computation mode):
     * - ADMD:   from self->mode (see enum pic18_adcc_mode)
     * - ADACLR: 1 = Auto-clear accumulator at end of burst
     * - ADCRS:  shift applied to the accumulator before storing in ADFLTR
     * - ADPSIS: 0 = ADRES as input to computation
     *
     * For averaging modes ADRPT loads the burst length and ADCRS = log2(N)
     * so ADFLTR = sum >> ADCRS = arithmetic mean.
     */
    if (self->mode > PIC18_ADCC_MODE_LOW_PASS)
    {
        return -EINVAL;
    }

    ADCON2bits.ADMD   = self->mode;
    ADCON2bits.ADACLR = 1;
    ADCON2bits.ADPSIS = 0;

    if (self->mode == PIC18_ADCC_MODE_BASIC)
    {
        ADRPT            = 0;
        ADCON2bits.ADCRS = 0;
    }
    else
    {
        ADRPT            = PIC18_ADCC_BURST_SAMPLES;
        ADCON2bits.ADCRS = PIC18_ADCC_BURST_SHIFT;
    }

    pic18_adcc_active_mode = self->mode;

    /* Clear interrupt flags and disable interrupts */
    pic18_adcc_clear_flags();
    PIE1bits.ADIE  = 0;
    PIE1bits.ADTIE = 0;

    /* Enable ADC */
    ADCON0bits.ADON = 1;

    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

static inline uint8_t pic18_adcc_select_channel(uint8_t channel)
{
    /* The mapping between the channel and the ADC channel is 1 to 1 using the
     * macros values defined on libraries/c/drivers/pic18-k42/inc/pic18_gpio.h
     */
    ADPCH = channel;

    /* Allow sample & hold capacitor to settle after channel switch */
    __delay_us(20);
    return 0;
}

int8_t pic18_adcc_read(struct adc* self, uint8_t channel, uint32_t* value)
{
    if (self == NULL || value == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (pic18_adcc_is_busy())
    {
        return -EBUSY;
    }

    /* Select channel */
    pic18_adcc_select_channel(channel);

    /* Disable continuous conversion mode for single read */
    ADCON0bits.ADCONT = 0;

    /* Clear interrupt flag */
    PIR1bits.ADIF = 0;

    /* Start conversion */
    ADCON0bits.ADGO = 1;

    /* Wait for conversion to complete (polls ADGO bit) */
    pic18_adcc_wait_conversion();

    /* Read result */
    *value = (uint32_t)pic18_adcc_read_result();

    return 0;
}

/* ========================================================================== */

int8_t pic18_adcc_start_conversion(struct adc* self, uint8_t channel)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }
    if (pic18_adcc_is_busy())
    {
        return -EBUSY;
    }

    /* Select channel */
    pic18_adcc_select_channel(channel);

    /* Disable continuous conversion mode */
    ADCON0bits.ADCONT = 0;

    /* Clear interrupt flag */
    PIR1bits.ADIF = 0;

    /* Start conversion */
    ADCON0bits.ADGO = 1;

    return 0;
}

/* ========================================================================== */

int8_t pic18_adcc_stop_conversion(struct adc* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }

    /* Stop ongoing conversion */
    ADCON0bits.ADGO = 0;

    return 0;
}

/* ========================================================================== */

int8_t pic18_adcc_is_conversion_complete(struct adc* self, bool* complete)
{
    if (self == NULL || complete == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }

    /* Conversion is complete when ADGO is cleared by hardware */
    *complete = (ADCON0bits.ADGO == 0);

    return 0;
}

/* ========================================================================== */

int8_t pic18_adcc_get_value(struct adc* self, uint32_t* value)
{
    if (self == NULL || value == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }

    /* Check if conversion is still in progress */
    if (pic18_adcc_is_busy())
    {
        return -EAGAIN;
    }

    /* Read result */
    *value = (uint32_t)pic18_adcc_read_result();

    return 0;
}

/* ========================================================================== */

int8_t pic18_adcc_set_conversion_callback(
    struct adc*               self,
    adc_conversion_callback_t callback,
    void*                     callback_context)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (!self->was_initialized)
    {
        return -EPERM;
    }

    self->callback         = callback;
    self->callback_context = callback_context;
    return 0;
}

/* ========================================================================== */

void pic18_adcc_enable_conv_interrupt(struct adc* self, bool enable)
{
    if (self == NULL || !self->was_initialized)
    {
        return;
    }

    if (enable)
    {
        PIR1bits.ADIF = 0; /* Clear flag before enabling */
        PIE1bits.ADIE = 1;
    }
    else
    {
        PIE1bits.ADIE = 0;
    }
    pic18_isr_register(PIC18_IRQ_ADCC_CONVERSION, pic18_adcc_isr_handler);
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = (uint8_t)enable;
}

/* ========================================================================== */

static inline void pic18_adcc_isr_handler(void)
{
    /* Clear interrupt flag */
    PIR1bits.ADIF = 0;

    if (pic18_adcc_conversion_callback != NULL)
    {
        uint32_t result = (uint32_t)pic18_adcc_read_result();
        pic18_adcc_conversion_callback(pic18_adcc_callback_context, result);
    }
}

/* ========================================================================== */
