/**
 * @file
 * @brief PIC18-K42 pinout configuration macros.
 *
 * This header provides macros to generate ANSEL and TRIS register values
 * from individual pin mode definitions.
 *
 * Usage in your project's pinout_config.h:
 *
 *   #include "pic18_pinout.h"
 *
 *   // Define each pin mode
 *   #define GPIO_A0  PIN_DO   // Digital Output
 *   #define GPIO_A1  PIN_DI   // Digital Input
 *   #define GPIO_A2  PIN_AI   // Analog Input
 *   #define GPIO_A3  PIN_NC   // Not Connected
 *   // ... define all pins for your variant
 *
 *   // Include the generator macros
 *   #include "pic18_pinout_config.h"
 */

/* ========================================================================== */

#ifndef PIC18_PINOUT_H
#define PIC18_PINOUT_H

#define ADGRDB_OUT_PPS_SETTING     0x32
#define ADGRDA_OUT_PPS_SETTING     0x31
#define CWG3D_OUT_PPS_SETTING      0x30
#define CWG3C_OUT_PPS_SETTING      0x2F
#define CWG3B_OUT_PPS_SETTING      0x2E
#define CWG3A_OUT_PPS_SETTING      0x2D
#define CWG2D_OUT_PPS_SETTING      0x2C
#define CWG2C_OUT_PPS_SETTING      0x2B
#define CWG2B_OUT_PPS_SETTING      0x2A
#define CWG2A_OUT_PPS_SETTING      0x29
#define DSM1_OUT_PPS_SETTING       0x28
#define CLKR_OUT_PPS_SETTING       0x27
#define NCO1_OUT_PPS_SETTING       0x26
#define TMR0_OUT_PPS_SETTING       0x25
#define I2C2_SDA_OUT_PPS_SETTING   0x24
#define I2C2_SCL_OUT_PPS_SETTING   0x23
#define I2C1_SDA_OUT_PPS_SETTING   0x22
#define I2C1_SCL_OUT_PPS_SETTING   0x21
#define SPI1_SS_OUT_PPS_SETTING    0x20
#define SPI1_SDO_OUT_PPS_SETTING   0x1F
#define SPI1_SCK_OUT_PPS_SETTING   0x1E
#define C2_OUT_OUT_PPS_SETTING     0x1D
#define C1_OUT_OUT_PPS_SETTING     0x1C
#define UART2_RTS_OUT_PPS_SETTING  0x18
#define UART2_TXDE_OUT_PPS_SETTING 0x17
#define UART2_TX_OUT_PPS_SETTING   0x16
#define UART1_RTS_OUT_PPS_SETTING  0x15
#define UART1_TXDE_OUT_PPS_SETTING 0x14
#define UART1_TX_OUT_PPS_SETTING   0x13
#define PWM8_OUT_PPS_SETTING       0x10
#define PWM7_OUT_PPS_SETTING       0x0F
#define PWM6_OUT_PPS_SETTING       0x0E
#define PWM5_OUT_PPS_SETTING       0x0D
#define CCP4_OUT_PPS_SETTING       0x0C
#define CCP3_OUT_PPS_SETTING       0x0B
#define CCP2_OUT_PPS_SETTING       0x0A
#define CCP1_OUT_PPS_SETTING       0x09
#define CWG1D_OUT_PPS_SETTING      0x08
#define CWG1C_OUT_PPS_SETTING      0x07
#define CWG1B_OUT_PPS_SETTING      0x06
#define CWG1A_OUT_PPS_SETTING      0x05
#define CLC4OUT_OUT_PPS_SETTING    0x04
#define CLC3OUT_OUT_PPS_SETTING    0x03
#define CLC2OUT_OUT_PPS_SETTING    0x02
#define CLC1OUT_OUT_PPS_SETTING    0x01

#define INT0PPS_IN_PPS_REG         INT0PPS
#define INT1PPS_IN_PPS_REG         INT1PPS
#define INT2PPS_IN_PPS_REG         INT2PPS
#define T0CKIPPS_IN_PPS_REG        T0CKIPPS
#define T1CKIPPS_IN_PPS_REG        T1CKIPPS
#define T1GPPS_IN_PPS_REG          T1GPPS
#define T3CKIPPS_IN_PPS_REG        T3CKIPPS
#define T3GPPS_IN_PPS_REG          T3GPPS
#define T5CKIPPS_IN_PPS_REG        T5CKIPPS
#define T5GPPS_IN_PPS_REG          T5GPPS
#define T2INPPS_IN_PPS_REG         T2INPPS
#define T4INPPS_IN_PPS_REG         T4INPPS
#define T6INPPS_IN_PPS_REG         T6INPPS
#define CCP1PPS_IN_PPS_REG         CCP1PPS
#define CCP2PPS_IN_PPS_REG         CCP2PPS
#define CCP3PPS_IN_PPS_REG         CCP3PPS
#define CCP4PPS_IN_PPS_REG         CCP4PPS
#define SMT1WINPPS_IN_PPS_REG      SMT1WINPPS
#define SMT1SIGPPS_IN_PPS_REG      SMT1SIGPPS
#define CWG1PPS_IN_PPS_REG         CWG1PPS
#define CWG2PPS_IN_PPS_REG         CWG2PPS
#define CWG3PPS_IN_PPS_REG         CWG3PPS
#define MD1CARLPPS_IN_PPS_REG      MD1CARLPPS
#define MD1CARHPPS_IN_PPS_REG      MD1CARHPPS
#define MD1SRCPPS_IN_PPS_REG       MD1SRCPPS
#define CLCIN0PPS_IN_PPS_REG       CLCIN0PPS
#define CLCIN1PPS_IN_PPS_REG       CLCIN1PPS
#define CLCIN2PPS_IN_PPS_REG       CLCIN2PPS
#define CLCIN3PPS_IN_PPS_REG       CLCIN3PPS
#define ADACTPPS_IN_PPS_REG        ADACTPPS
#define SPI1SCKPPS_IN_PPS_REG      SPI1SCKPPS
#define SPI1SDIPPS_IN_PPS_REG      SPI1SDIPPS
#define SPI1SSPPS_IN_PPS_REG       SPI1SSPPS
#define I2C1SCLPPS_IN_PPS_REG      I2C1SCLPPS
#define I2C1SDAPPS_IN_PPS_REG      I2C1SDAPPS
#define I2C2SCLPPS_IN_PPS_REG      I2C2SCLPPS
#define I2C2SDAPPS_IN_PPS_REG      I2C2SDAPPS
#define U1RXPPS_IN_PPS_REG         U1RXPPS
#define U1CTSPPS_IN_PPS_REG        U1CTSPPS
#define U2RXPPS_IN_PPS_REG         U2RXPPS
#define U2CTSPPS_IN_PPS_REG        U2CTSPPS

/* ========================================================================== */

/**
 * @brief Pin mode definitions. Use these values to define each pin's mode in
 * your project's pinout_config.h
 */
#define PIN_NC \
    0 /* Not Connected / Not Configured (defaults to digital input) */
#define PIN_DI                        1 /* Digital Input */
#define PIN_DO                        2 /* Digital Output */
#define PIN_AI                        3 /* Analog Input */
#define PIN_OD                        4 /* Open Drain Output */

/* ========================================================================== */

/**
 * @brief Helper macros to generate register values from pin modes.
 */

/* Check if pin mode is analog (for ANSEL register: 1 = analog) */
#define GPIO_IS_ANALOG(mode)          ((mode) == PIN_AI ? 1 : 0)

/* Check if pin mode is input (for TRIS register: 1 = input, 0 = output) */
#define GPIO_IS_INPUT(mode)           (((mode) != PIN_DO && (mode) != PIN_OD) ? 1 : 0)

/* Check if pin mode is open drain (for ODCON register: 1 = open drain) */
#define GPIO_IS_OPEN_DRAIN(mode)      ((mode) == PIN_OD ? 1 : 0)

/* Generate bit value for a specific pin position */
#define GPIO_BIT(mode, bit, check_fn) (uint8_t)((check_fn(mode)) << (bit))

/* ========================================================================== */

/**
 * @brief Macros to generate ANSEL register values (1 = analog input enabled).
 *
 * These require PIN_Ax, PIN_Bx, etc. to be defined before use.
 */
#define ANSELA_CONFIG                       \
    (GPIO_BIT(GPIO_A0, 0, GPIO_IS_ANALOG)   \
     | GPIO_BIT(GPIO_A1, 1, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_A2, 2, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_A3, 3, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_A4, 4, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_A5, 5, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_A6, 6, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_A7, 7, GPIO_IS_ANALOG))

#define ANSELB_CONFIG                       \
    (GPIO_BIT(GPIO_B0, 0, GPIO_IS_ANALOG)   \
     | GPIO_BIT(GPIO_B1, 1, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_B2, 2, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_B3, 3, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_B4, 4, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_B5, 5, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_B6, 6, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_B7, 7, GPIO_IS_ANALOG))

#define ANSELC_CONFIG                       \
    (GPIO_BIT(GPIO_C0, 0, GPIO_IS_ANALOG)   \
     | GPIO_BIT(GPIO_C1, 1, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_C2, 2, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_C3, 3, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_C4, 4, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_C5, 5, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_C6, 6, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_C7, 7, GPIO_IS_ANALOG))

#if defined(PIC18F47K42) || defined(PIC18F57K42)
#define ANSELD_CONFIG                       \
    (GPIO_BIT(GPIO_D0, 0, GPIO_IS_ANALOG)   \
     | GPIO_BIT(GPIO_D1, 1, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_D2, 2, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_D3, 3, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_D4, 4, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_D5, 5, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_D6, 6, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_D7, 7, GPIO_IS_ANALOG))

#define ANSELE_CONFIG                       \
    (GPIO_BIT(GPIO_E0, 0, GPIO_IS_ANALOG)   \
     | GPIO_BIT(GPIO_E1, 1, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_E2, 2, GPIO_IS_ANALOG))
#endif /* PIC18F47K42 || PIC18F57K42 */

#if defined(PIC18F57K42)
#define ANSELF_CONFIG                       \
    (GPIO_BIT(GPIO_F0, 0, GPIO_IS_ANALOG)   \
     | GPIO_BIT(GPIO_F1, 1, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_F2, 2, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_F3, 3, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_F4, 4, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_F5, 5, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_F6, 6, GPIO_IS_ANALOG) \
     | GPIO_BIT(GPIO_F7, 7, GPIO_IS_ANALOG))
#endif /* PIC18F57K42 */

/* ========================================================================== */

/**
 * @brief Macros to generate TRIS register values (1 = input, 0 = output).
 *
 * These require PIN_Ax, PIN_Bx, etc. to be defined before use.
 */
#define TRISA_CONFIG                                                           \
    (GPIO_BIT(GPIO_A0, 0, GPIO_IS_INPUT) | GPIO_BIT(GPIO_A1, 1, GPIO_IS_INPUT) \
     | GPIO_BIT(GPIO_A2, 2, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_A3, 3, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_A4, 4, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_A5, 5, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_A6, 6, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_A7, 7, GPIO_IS_INPUT))

#define TRISB_CONFIG                                                           \
    (GPIO_BIT(GPIO_B0, 0, GPIO_IS_INPUT) | GPIO_BIT(GPIO_B1, 1, GPIO_IS_INPUT) \
     | GPIO_BIT(GPIO_B2, 2, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_B3, 3, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_B4, 4, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_B5, 5, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_B6, 6, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_B7, 7, GPIO_IS_INPUT))

#define TRISC_CONFIG                                                           \
    (GPIO_BIT(GPIO_C0, 0, GPIO_IS_INPUT) | GPIO_BIT(GPIO_C1, 1, GPIO_IS_INPUT) \
     | GPIO_BIT(GPIO_C2, 2, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_C3, 3, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_C4, 4, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_C5, 5, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_C6, 6, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_C7, 7, GPIO_IS_INPUT))

#if defined(PIC18F47K42) || defined(PIC18F57K42)
#define TRISD_CONFIG                                                           \
    (GPIO_BIT(GPIO_D0, 0, GPIO_IS_INPUT) | GPIO_BIT(GPIO_D1, 1, GPIO_IS_INPUT) \
     | GPIO_BIT(GPIO_D2, 2, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_D3, 3, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_D4, 4, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_D5, 5, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_D6, 6, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_D7, 7, GPIO_IS_INPUT))

#define TRISE_CONFIG                                                           \
    (GPIO_BIT(GPIO_E0, 0, GPIO_IS_INPUT) | GPIO_BIT(GPIO_E1, 1, GPIO_IS_INPUT) \
     | GPIO_BIT(GPIO_E2, 2, GPIO_IS_INPUT))
#endif /* PIC18F47K42 || PIC18F57K42 */

#if defined(PIC18F57K42)
#define TRISF_CONFIG                                                           \
    (GPIO_BIT(GPIO_F0, 0, GPIO_IS_INPUT) | GPIO_BIT(GPIO_F1, 1, GPIO_IS_INPUT) \
     | GPIO_BIT(GPIO_F2, 2, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_F3, 3, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_F4, 4, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_F5, 5, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_F6, 6, GPIO_IS_INPUT)                                     \
     | GPIO_BIT(GPIO_F7, 7, GPIO_IS_INPUT))
#endif /* PIC18F57K42 */

/* ========================================================================== */

/**
 * @brief Macros to generate ODCON register values (1 = open drain output
 * enabled).
 *
 * These require GPIO_Ax, GPIO_Bx, etc. to be defined before use.
 */
#define ODCONA_CONFIG                           \
    (GPIO_BIT(GPIO_A0, 0, GPIO_IS_OPEN_DRAIN)   \
     | GPIO_BIT(GPIO_A1, 1, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_A2, 2, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_A3, 3, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_A4, 4, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_A5, 5, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_A6, 6, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_A7, 7, GPIO_IS_OPEN_DRAIN))

#define ODCONB_CONFIG                           \
    (GPIO_BIT(GPIO_B0, 0, GPIO_IS_OPEN_DRAIN)   \
     | GPIO_BIT(GPIO_B1, 1, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_B2, 2, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_B3, 3, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_B4, 4, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_B5, 5, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_B6, 6, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_B7, 7, GPIO_IS_OPEN_DRAIN))

#define ODCONC_CONFIG                           \
    (GPIO_BIT(GPIO_C0, 0, GPIO_IS_OPEN_DRAIN)   \
     | GPIO_BIT(GPIO_C1, 1, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_C2, 2, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_C3, 3, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_C4, 4, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_C5, 5, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_C6, 6, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_C7, 7, GPIO_IS_OPEN_DRAIN))

#if defined(PIC18F47K42) || defined(PIC18F57K42)
#define ODCOND_CONFIG                           \
    (GPIO_BIT(GPIO_D0, 0, GPIO_IS_OPEN_DRAIN)   \
     | GPIO_BIT(GPIO_D1, 1, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_D2, 2, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_D3, 3, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_D4, 4, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_D5, 5, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_D6, 6, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_D7, 7, GPIO_IS_OPEN_DRAIN))

#define ODCONE_CONFIG                           \
    (GPIO_BIT(GPIO_E0, 0, GPIO_IS_OPEN_DRAIN)   \
     | GPIO_BIT(GPIO_E1, 1, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_E2, 2, GPIO_IS_OPEN_DRAIN))
#endif /* PIC18F47K42 || PIC18F57K42 */

#if defined(PIC18F57K42)
#define ODCONF_CONFIG                           \
    (GPIO_BIT(GPIO_F0, 0, GPIO_IS_OPEN_DRAIN)   \
     | GPIO_BIT(GPIO_F1, 1, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_F2, 2, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_F3, 3, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_F4, 4, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_F5, 5, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_F6, 6, GPIO_IS_OPEN_DRAIN) \
     | GPIO_BIT(GPIO_F7, 7, GPIO_IS_OPEN_DRAIN))
#endif /* PIC18F57K42 */

/* ========================================================================== */

#endif /* PIC18_PINOUT_H */
