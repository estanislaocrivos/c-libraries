#include "../inc/pic18_mcu.h"

/**
 * Include project's pinout configuration.
 * The project must define PINOUT_CONFIG_HEADER in CMake, e.g.:
 *   target_compile_definitions(${PROJECT_NAME} PRIVATE
 * PINOUT_CONFIG_HEADER="pinout_config.h")
 *
 * Or define all PIN_Xx macros before this file is compiled.
 */
#ifdef PINOUT_CONFIG_HEADER
#include PINOUT_CONFIG_HEADER
#endif

#include "../inc/pic18_gpio.h"
#include "../inc/pic18_pinout.h"

/* ========================================================================== */

const struct mcu_ops pic18_mcu_ops = {
    .initialize          = pic18_mcu_initialize,
    .reset               = pic18_mcu_reset,
    .sleep               = pic18_mcu_sleep,
    .get_clock_frequency = pic18_mcu_get_clock_frequency,
};

/* ========================================================================== */

/**
 * @brief Configures PPS output assignments based on pinout configuration.
 *
 * For each pin with GPIO_Xy_OUT_PPS_SETTING defined to a non-zero value,
 * writes that value to the corresponding RxyPPS register.
 */
static void pic18_pps_out_init_pinout(void)
{
    /* Port A */
#if defined(GPIO_A0_OUT_PPS_SETTING) && GPIO_A0_OUT_PPS_SETTING
    RA0PPS = GPIO_A0_OUT_PPS_SETTING;
#endif
#if defined(GPIO_A1_OUT_PPS_SETTING) && GPIO_A1_OUT_PPS_SETTING
    RA1PPS = GPIO_A1_OUT_PPS_SETTING;
#endif
#if defined(GPIO_A2_OUT_PPS_SETTING) && GPIO_A2_OUT_PPS_SETTING
    RA2PPS = GPIO_A2_OUT_PPS_SETTING;
#endif
#if defined(GPIO_A3_OUT_PPS_SETTING) && GPIO_A3_OUT_PPS_SETTING
    RA3PPS = GPIO_A3_OUT_PPS_SETTING;
#endif
#if defined(GPIO_A4_OUT_PPS_SETTING) && GPIO_A4_OUT_PPS_SETTING
    RA4PPS = GPIO_A4_OUT_PPS_SETTING;
#endif
#if defined(GPIO_A5_OUT_PPS_SETTING) && GPIO_A5_OUT_PPS_SETTING
    RA5PPS = GPIO_A5_OUT_PPS_SETTING;
#endif
#if defined(GPIO_A6_OUT_PPS_SETTING) && GPIO_A6_OUT_PPS_SETTING
    RA6PPS = GPIO_A6_OUT_PPS_SETTING;
#endif
#if defined(GPIO_A7_OUT_PPS_SETTING) && GPIO_A7_OUT_PPS_SETTING
    RA7PPS = GPIO_A7_OUT_PPS_SETTING;
#endif

    /* Port B */
#if defined(GPIO_B0_OUT_PPS_SETTING) && GPIO_B0_OUT_PPS_SETTING
    RB0PPS = GPIO_B0_OUT_PPS_SETTING;
#endif
#if defined(GPIO_B1_OUT_PPS_SETTING) && GPIO_B1_OUT_PPS_SETTING
    RB1PPS = GPIO_B1_OUT_PPS_SETTING;
#endif
#if defined(GPIO_B2_OUT_PPS_SETTING) && GPIO_B2_OUT_PPS_SETTING
    RB2PPS = GPIO_B2_OUT_PPS_SETTING;
#endif
#if defined(GPIO_B3_OUT_PPS_SETTING) && GPIO_B3_OUT_PPS_SETTING
    RB3PPS = GPIO_B3_OUT_PPS_SETTING;
#endif
#if defined(GPIO_B4_OUT_PPS_SETTING) && GPIO_B4_OUT_PPS_SETTING
    RB4PPS = GPIO_B4_OUT_PPS_SETTING;
#endif
#if defined(GPIO_B5_OUT_PPS_SETTING) && GPIO_B5_OUT_PPS_SETTING
    RB5PPS = GPIO_B5_OUT_PPS_SETTING;
#endif
#if defined(GPIO_B6_OUT_PPS_SETTING) && GPIO_B6_OUT_PPS_SETTING
    RB6PPS = GPIO_B6_OUT_PPS_SETTING;
#endif
#if defined(GPIO_B7_OUT_PPS_SETTING) && GPIO_B7_OUT_PPS_SETTING
    RB7PPS = GPIO_B7_OUT_PPS_SETTING;
#endif

    /* Port C */
#if defined(GPIO_C0_OUT_PPS_SETTING) && GPIO_C0_OUT_PPS_SETTING
    RC0PPS = GPIO_C0_OUT_PPS_SETTING;
#endif
#if defined(GPIO_C1_OUT_PPS_SETTING) && GPIO_C1_OUT_PPS_SETTING
    RC1PPS = GPIO_C1_OUT_PPS_SETTING;
#endif
#if defined(GPIO_C2_OUT_PPS_SETTING) && GPIO_C2_OUT_PPS_SETTING
    RC2PPS = GPIO_C2_OUT_PPS_SETTING;
#endif
#if defined(GPIO_C3_OUT_PPS_SETTING) && GPIO_C3_OUT_PPS_SETTING
    RC3PPS = GPIO_C3_OUT_PPS_SETTING;
#endif
#if defined(GPIO_C4_OUT_PPS_SETTING) && GPIO_C4_OUT_PPS_SETTING
    RC4PPS = GPIO_C4_OUT_PPS_SETTING;
#endif
#if defined(GPIO_C5_OUT_PPS_SETTING) && GPIO_C5_OUT_PPS_SETTING
    RC5PPS = GPIO_C5_OUT_PPS_SETTING;
#endif
#if defined(GPIO_C6_OUT_PPS_SETTING) && GPIO_C6_OUT_PPS_SETTING
    RC6PPS = GPIO_C6_OUT_PPS_SETTING;
#endif
#if defined(GPIO_C7_OUT_PPS_SETTING) && GPIO_C7_OUT_PPS_SETTING
    RC7PPS = GPIO_C7_OUT_PPS_SETTING;
#endif

#if defined(PIC18F47K42) || defined(PIC18F57K42)
    /* Port D */
#if defined(GPIO_D0_OUT_PPS_SETTING) && GPIO_D0_OUT_PPS_SETTING
    RD0PPS = GPIO_D0_OUT_PPS_SETTING;
#endif
#if defined(GPIO_D1_OUT_PPS_SETTING) && GPIO_D1_OUT_PPS_SETTING
    RD1PPS = GPIO_D1_OUT_PPS_SETTING;
#endif
#if defined(GPIO_D2_OUT_PPS_SETTING) && GPIO_D2_OUT_PPS_SETTING
    RD2PPS = GPIO_D2_OUT_PPS_SETTING;
#endif
#if defined(GPIO_D3_OUT_PPS_SETTING) && GPIO_D3_OUT_PPS_SETTING
    RD3PPS = GPIO_D3_OUT_PPS_SETTING;
#endif
#if defined(GPIO_D4_OUT_PPS_SETTING) && GPIO_D4_OUT_PPS_SETTING
    RD4PPS = GPIO_D4_OUT_PPS_SETTING;
#endif
#if defined(GPIO_D5_OUT_PPS_SETTING) && GPIO_D5_OUT_PPS_SETTING
    RD5PPS = GPIO_D5_OUT_PPS_SETTING;
#endif
#if defined(GPIO_D6_OUT_PPS_SETTING) && GPIO_D6_OUT_PPS_SETTING
    RD6PPS = GPIO_D6_OUT_PPS_SETTING;
#endif
#if defined(GPIO_D7_OUT_PPS_SETTING) && GPIO_D7_OUT_PPS_SETTING
    RD7PPS = GPIO_D7_OUT_PPS_SETTING;
#endif

    /* Port E */
#if defined(GPIO_E0_OUT_PPS_SETTING) && GPIO_E0_OUT_PPS_SETTING
    RE0PPS = GPIO_E0_OUT_PPS_SETTING;
#endif
#if defined(GPIO_E1_OUT_PPS_SETTING) && GPIO_E1_OUT_PPS_SETTING
    RE1PPS = GPIO_E1_OUT_PPS_SETTING;
#endif
#if defined(GPIO_E2_OUT_PPS_SETTING) && GPIO_E2_OUT_PPS_SETTING
    RE2PPS = GPIO_E2_OUT_PPS_SETTING;
#endif
#endif /* PIC18F47K42 || PIC18F57K42 */

#if defined(PIC18F57K42)
    /* Port F */
#if defined(GPIO_F0_OUT_PPS_SETTING) && GPIO_F0_OUT_PPS_SETTING
    RF0PPS = GPIO_F0_OUT_PPS_SETTING;
#endif
#if defined(GPIO_F1_OUT_PPS_SETTING) && GPIO_F1_OUT_PPS_SETTING
    RF1PPS = GPIO_F1_OUT_PPS_SETTING;
#endif
#if defined(GPIO_F2_OUT_PPS_SETTING) && GPIO_F2_OUT_PPS_SETTING
    RF2PPS = GPIO_F2_OUT_PPS_SETTING;
#endif
#if defined(GPIO_F3_OUT_PPS_SETTING) && GPIO_F3_OUT_PPS_SETTING
    RF3PPS = GPIO_F3_OUT_PPS_SETTING;
#endif
#if defined(GPIO_F4_OUT_PPS_SETTING) && GPIO_F4_OUT_PPS_SETTING
    RF4PPS = GPIO_F4_OUT_PPS_SETTING;
#endif
#if defined(GPIO_F5_OUT_PPS_SETTING) && GPIO_F5_OUT_PPS_SETTING
    RF5PPS = GPIO_F5_OUT_PPS_SETTING;
#endif
#if defined(GPIO_F6_OUT_PPS_SETTING) && GPIO_F6_OUT_PPS_SETTING
    RF6PPS = GPIO_F6_OUT_PPS_SETTING;
#endif
#if defined(GPIO_F7_OUT_PPS_SETTING) && GPIO_F7_OUT_PPS_SETTING
    RF7PPS = GPIO_F7_OUT_PPS_SETTING;
#endif
#endif /* PIC18F57K42 */
}

/**
 * @brief Configures PPS input assignments based on pinout configuration.
 *
 * For each pin with GPIO_Xy_IN_PPS_REG defined, writes the pin's
 * PPS input value (port << 3 | pin) to that register.
 *
 * Example in pinout_config.h:
 *   #define GPIO_C1_IN_PPS_REG  U1RXPPS  // UART1 RX on pin C1
 */
static void pic18_pps_in_init_pinout(void)
{
    /* Port A */
#ifdef GPIO_A0_IN_PPS_REG
    GPIO_A0_IN_PPS_REG = (PORT_A << 3) | 0;
#endif
#ifdef GPIO_A1_IN_PPS_REG
    GPIO_A1_IN_PPS_REG = (PORT_A << 3) | 1;
#endif
#ifdef GPIO_A2_IN_PPS_REG
    GPIO_A2_IN_PPS_REG = (PORT_A << 3) | 2;
#endif
#ifdef GPIO_A3_IN_PPS_REG
    GPIO_A3_IN_PPS_REG = (PORT_A << 3) | 3;
#endif
#ifdef GPIO_A4_IN_PPS_REG
    GPIO_A4_IN_PPS_REG = (PORT_A << 3) | 4;
#endif
#ifdef GPIO_A5_IN_PPS_REG
    GPIO_A5_IN_PPS_REG = (PORT_A << 3) | 5;
#endif
#ifdef GPIO_A6_IN_PPS_REG
    GPIO_A6_IN_PPS_REG = (PORT_A << 3) | 6;
#endif
#ifdef GPIO_A7_IN_PPS_REG
    GPIO_A7_IN_PPS_REG = (PORT_A << 3) | 7;
#endif

    /* Port B */
#ifdef GPIO_B0_IN_PPS_REG
    GPIO_B0_IN_PPS_REG = (PORT_B << 3) | 0;
#endif
#ifdef GPIO_B1_IN_PPS_REG
    GPIO_B1_IN_PPS_REG = (PORT_B << 3) | 1;
#endif
#ifdef GPIO_B2_IN_PPS_REG
    GPIO_B2_IN_PPS_REG = (PORT_B << 3) | 2;
#endif
#ifdef GPIO_B3_IN_PPS_REG
    GPIO_B3_IN_PPS_REG = (PORT_B << 3) | 3;
#endif
#ifdef GPIO_B4_IN_PPS_REG
    GPIO_B4_IN_PPS_REG = (PORT_B << 3) | 4;
#endif
#ifdef GPIO_B5_IN_PPS_REG
    GPIO_B5_IN_PPS_REG = (PORT_B << 3) | 5;
#endif
#ifdef GPIO_B6_IN_PPS_REG
    GPIO_B6_IN_PPS_REG = (PORT_B << 3) | 6;
#endif
#ifdef GPIO_B7_IN_PPS_REG
    GPIO_B7_IN_PPS_REG = (PORT_B << 3) | 7;
#endif

    /* Port C */
#ifdef GPIO_C0_IN_PPS_REG
    GPIO_C0_IN_PPS_REG = (PORT_C << 3) | 0;
#endif
#ifdef GPIO_C1_IN_PPS_REG
    GPIO_C1_IN_PPS_REG = (PORT_C << 3) | 1;
#endif
#ifdef GPIO_C2_IN_PPS_REG
    GPIO_C2_IN_PPS_REG = (PORT_C << 3) | 2;
#endif
#ifdef GPIO_C3_IN_PPS_REG
    GPIO_C3_IN_PPS_REG = (PORT_C << 3) | 3;
#endif
#ifdef GPIO_C4_IN_PPS_REG
    GPIO_C4_IN_PPS_REG = (PORT_C << 3) | 4;
#endif
#ifdef GPIO_C5_IN_PPS_REG
    GPIO_C5_IN_PPS_REG = (PORT_C << 3) | 5;
#endif
#ifdef GPIO_C6_IN_PPS_REG
    GPIO_C6_IN_PPS_REG = (PORT_C << 3) | 6;
#endif
#ifdef GPIO_C7_IN_PPS_REG
    GPIO_C7_IN_PPS_REG = (PORT_C << 3) | 7;
#endif

#if defined(PIC18F47K42) || defined(PIC18F57K42)
    /* Port D */
#ifdef GPIO_D0_IN_PPS_REG
    GPIO_D0_IN_PPS_REG = (PORT_D << 3) | 0;
#endif
#ifdef GPIO_D1_IN_PPS_REG
    GPIO_D1_IN_PPS_REG = (PORT_D << 3) | 1;
#endif
#ifdef GPIO_D2_IN_PPS_REG
    GPIO_D2_IN_PPS_REG = (PORT_D << 3) | 2;
#endif
#ifdef GPIO_D3_IN_PPS_REG
    GPIO_D3_IN_PPS_REG = (PORT_D << 3) | 3;
#endif
#ifdef GPIO_D4_IN_PPS_REG
    GPIO_D4_IN_PPS_REG = (PORT_D << 3) | 4;
#endif
#ifdef GPIO_D5_IN_PPS_REG
    GPIO_D5_IN_PPS_REG = (PORT_D << 3) | 5;
#endif
#ifdef GPIO_D6_IN_PPS_REG
    GPIO_D6_IN_PPS_REG = (PORT_D << 3) | 6;
#endif
#ifdef GPIO_D7_IN_PPS_REG
    GPIO_D7_IN_PPS_REG = (PORT_D << 3) | 7;
#endif

    /* Port E */
#ifdef GPIO_E0_IN_PPS_REG
    GPIO_E0_IN_PPS_REG = (PORT_E << 3) | 0;
#endif
#ifdef GPIO_E1_IN_PPS_REG
    GPIO_E1_IN_PPS_REG = (PORT_E << 3) | 1;
#endif
#ifdef GPIO_E2_IN_PPS_REG
    GPIO_E2_IN_PPS_REG = (PORT_E << 3) | 2;
#endif
#endif /* PIC18F47K42 || PIC18F57K42 */

#if defined(PIC18F57K42)
    /* Port F */
#ifdef GPIO_F0_IN_PPS_REG
    GPIO_F0_IN_PPS_REG = (PORT_F << 3) | 0;
#endif
#ifdef GPIO_F1_IN_PPS_REG
    GPIO_F1_IN_PPS_REG = (PORT_F << 3) | 1;
#endif
#ifdef GPIO_F2_IN_PPS_REG
    GPIO_F2_IN_PPS_REG = (PORT_F << 3) | 2;
#endif
#ifdef GPIO_F3_IN_PPS_REG
    GPIO_F3_IN_PPS_REG = (PORT_F << 3) | 3;
#endif
#ifdef GPIO_F4_IN_PPS_REG
    GPIO_F4_IN_PPS_REG = (PORT_F << 3) | 4;
#endif
#ifdef GPIO_F5_IN_PPS_REG
    GPIO_F5_IN_PPS_REG = (PORT_F << 3) | 5;
#endif
#ifdef GPIO_F6_IN_PPS_REG
    GPIO_F6_IN_PPS_REG = (PORT_F << 3) | 6;
#endif
#ifdef GPIO_F7_IN_PPS_REG
    GPIO_F7_IN_PPS_REG = (PORT_F << 3) | 7;
#endif
#endif /* PIC18F57K42 */
}

/* ========================================================================== */

/**
 * @brief Configures all GPIO pins based on pic18_pinout.h configuration.
 */
static void pic18_mcu_init_pinout(void)
{
    /* Configure analog/digital mode for all ports */
    ANSELA = ANSELA_CONFIG;
    ANSELB = ANSELB_CONFIG;
    ANSELC = ANSELC_CONFIG;

#if defined(PIC18F47K42) || defined(PIC18F57K42)
    ANSELD = ANSELD_CONFIG;
    ANSELE = ANSELE_CONFIG;
#endif

#if defined(PIC18F57K42)
    ANSELF = ANSELF_CONFIG;
#endif

    /* Configure input/output direction for all ports */
    TRISA = TRISA_CONFIG;
    TRISB = TRISB_CONFIG;
    TRISC = TRISC_CONFIG;

#if defined(PIC18F47K42) || defined(PIC18F57K42)
    TRISD = TRISD_CONFIG;
    TRISE = TRISE_CONFIG;
#endif

#if defined(PIC18F57K42)
    TRISF = TRISF_CONFIG;
#endif

    /* Configure open drain mode for all ports */
    ODCONA = ODCONA_CONFIG;
    ODCONB = ODCONB_CONFIG;
    ODCONC = ODCONC_CONFIG;

#if defined(PIC18F47K42) || defined(PIC18F57K42)
    ODCOND = ODCOND_CONFIG;
    ODCONE = ODCONE_CONFIG;
#endif

#if defined(PIC18F57K42)
    ODCONF = ODCONF_CONFIG;
#endif
}

/* ========================================================================== */

/**
 * @brief Configures the oscillator to the target frequency.
 */
static void pic18_mcu_init_oscillator(uint32_t frequency)
{
    /*
     * Select HFINTOSC as the system clock source with no division.
     * This overrides whatever RSTOSC config bit selected at reset
     * (e.g. HFINTOSC_64MHZ forces OSCFRQ = 0x08 and ignores user writes
     * to OSCFRQ until NOSC is explicitly set here).
     *
     * OSCCON1:
     *   NOSC = 0b110 (HFINTOSC)
     *   NDIV = 0b0000 (divide by 1)
     */
    OSCCON1 = (0 << _OSCCON1_NDIV_POSN) | (6 << _OSCCON1_NOSC_POSN);

    /* No clock switch hold */
    OSCCON3 = 0x00;

    /* No oscillators forced on manually */
    OSCEN = 0x00;

    /*
     * Configure HFINTOSC frequency.
     * OSCFRQ register HFFRQ bits:
     * - 0b1000 = 64 MHz
     * - 0b0111 = 48 MHz
     * - 0b0110 = 32 MHz
     * - 0b0101 = 16 MHz
     * - 0b0100 = 12 MHz
     * - 0b0011 = 8 MHz
     * - 0b0010 = 4 MHz
     * - 0b0001 = 2 MHz
     * - 0b0000 = 1 MHz
     */
    switch (frequency)
    {
        case 64000000:
            OSCFRQ = 0x08;
            break;

        case 48000000:
            OSCFRQ = 0x07;
            break;

        case 32000000:
            OSCFRQ = 0x06;
            break;

        case 16000000:
            OSCFRQ = 0x05;
            break;

        case 12000000:
            OSCFRQ = 0x04;
            break;

        case 8000000:
            OSCFRQ = 0x03;
            break;

        case 4000000:
            OSCFRQ = 0x02;
            break;

        case 2000000:
            OSCFRQ = 0x01;
            break;

        case 1000000:
            OSCFRQ = 0x00;
            break;

        default:
            /* Default to 16 MHz if unsupported frequency is requested */
            OSCFRQ = 0x05;
            break;
    }

    OSCTUNE = 0x00;

    /* Wait for the clock switch to complete and the oscillator to stabilize */
    while (!OSCCON3bits.ORDY)
    {
    }
    while (!OSCSTATbits.HFOR)
    {
    }
}

/* ========================================================================== */

int8_t pic18_mcu_initialize(struct mcu* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Initialize oscillator first */
    pic18_mcu_init_oscillator(_XTAL_FREQ);

    /* Configure all GPIO pins */
    pic18_mcu_init_pinout();

    /* Configure PPS assignments */
    pic18_pps_out_init_pinout();
    pic18_pps_in_init_pinout();

    self->clock_frequency = _XTAL_FREQ;
    self->was_initialized = true;
    return 0;
}

/* ========================================================================== */

int8_t pic18_mcu_reset(struct mcu* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Trigger software reset via RESET instruction */
    asm("RESET");

    /* This point should never be reached */
    return 0;
}

/* ========================================================================== */

int8_t pic18_mcu_sleep(struct mcu* self)
{
    if (self == NULL)
    {
        return -EFAULT;
    }

    /* Enter sleep mode via SLEEP instruction */
    asm("SLEEP");

    return 0;
}

/* ========================================================================== */

int8_t pic18_mcu_get_clock_frequency(const struct mcu* self, uint32_t* frequency)
{
    if (self == NULL || frequency == NULL)
    {
        return -EFAULT;
    }

    *frequency = self->clock_frequency;
    return 0;
}

/* ========================================================================== */
