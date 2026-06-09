#ifndef PIC18_PINOUT_CONFIG_H
#define PIC18_PINOUT_CONFIG_H

/* ========================================================================== */

#include "pic18_pinout.h"

/* ========================================================================== */

/* PORT A */
#define GPIO_A0 PIN_NC
#define GPIO_A1 PIN_NC
#define GPIO_A2 PIN_NC
#define GPIO_A3 PIN_NC
#define GPIO_A4 PIN_NC
#define GPIO_A5 PIN_NC
#define GPIO_A6 PIN_NC
#define GPIO_A7 PIN_NC

/* PORT B */
#define GPIO_B0 PIN_NC
#define GPIO_B1 PIN_NC
#define GPIO_B2 PIN_NC
#define GPIO_B3 PIN_NC
#define GPIO_B4 PIN_NC
#define GPIO_B5 PIN_NC
#define GPIO_B6 PIN_NC
#define GPIO_B7 PIN_NC

/* PORT C */
#define GPIO_C0 PIN_NC
#define GPIO_C1 PIN_NC
#define GPIO_C2 PIN_DO
#define GPIO_C3 PIN_DI
#define GPIO_C4 PIN_NC
#define GPIO_C5 PIN_NC
#define GPIO_C6 PIN_NC
#define GPIO_C7 PIN_NC

#if defined(MCU_18F47K42) || defined(MCU_18F57K42)
/* PORT D */
#define GPIO_D0 PIN_NC
#define GPIO_D1 PIN_NC
#define GPIO_D2 PIN_NC
#define GPIO_D3 PIN_NC
#define GPIO_D4 PIN_NC
#define GPIO_D5 PIN_NC
#define GPIO_D6 PIN_NC
#define GPIO_D7 PIN_NC

/* PORT E */
#define GPIO_E0 PIN_NC
#define GPIO_E1 PIN_NC
#define GPIO_E2 PIN_NC
#endif /* MCU_18F47K42 || MCU_18F57K42 */

#if defined(MCU_18F57K42)
/* PORT F */
#define GPIO_F0 PIN_NC
#define GPIO_F1 PIN_NC
#define GPIO_F2 PIN_NC
#define GPIO_F3 PIN_NC
#define GPIO_F4 PIN_NC
#define GPIO_F5 PIN_NC
#define GPIO_F6 PIN_NC
#define GPIO_F7 PIN_NC
#endif /* MCU_18F57K42 */

/* ========================================================================== */

/* OUTPUTS Peripheral Pin Select. Add your output PPS settings here, according
 * to ...OUT_PPS_SETTING macros on
 * external/libraries/c/drivers/pic18_k42/inc/pic18_pinout.h. For example, you
 * may set pin C0 as the UART1 TX output like this:
 *
 * #define GPIO_C0_OUT_PPS_SETTING UART1_TX_OUT_PPS_SETTING
 */

/* PORT A */
#define GPIO_A1_OUT_PPS_SETTING T1GOUT_PPS_SETTING

/* PORT B */
#define GPIO_B3_OUT_PPS_SETTING CCP1_OUT_PPS_SETTING
#define GPIO_B4_OUT_PPS_SETTING CCP2_OUT_PPS_SETTING

/* PORT C */
#define GPIO_C2_OUT_PPS_SETTING UART1_TX_OUT_PPS_SETTING

#if defined(MCU_18F47K42) || defined(MCU_18F57K42)
/* PORT D */
#define GPIO_D3_OUT_PPS_SETTING CCP3_OUT_PPS_SETTING

/* PORT E */
#define GPIO_E1_OUT_PPS_SETTING ADGRDA_OUT_PPS_SETTING
#endif /* MCU_18F47K42 || MCU_18F57K42 */

#if defined(MCU_18F57K42)
/* PORT F */
#define GPIO_F1_OUT_PPS_SETTING C1_OUT_OUT_PPS_SETTING
#endif /* MCU_18F57K42 */

/* ========================================================================== */

/* INPUTS Peripheral Pin Select. Add your input PPS settings here, according to
 * ...IN_PPS_REG macros on
 * external/libraries/c/drivers/pic18_k42/inc/pic18_pinout.h. For example, you
 * may set pin C1 as UART1 RX input like this:
 *
 * #define GPIO_C1_IN_PPS_REG U1RXPPS_IN_PPS_REG
 */

/* PORT A */
#define GPIO_A1_IN_PPS_REG T1CKIPPS_IN_PPS_REG

/* PORT B */
#define GPIO_B0_IN_PPS_REG CCP1PPS_IN_PPS_REG
#define GPIO_B1_IN_PPS_REG CCP2PPS_IN_PPS_REG

/* PORT C */
#define GPIO_C3_IN_PPS_REG U1RXPPS_IN_PPS_REG

#if defined(MCU_18F47K42) || defined(MCU_18F57K42)
/* PORT D */
#define GPIO_D2_IN_PPS_REG CCP3PPS_IN_PPS_REG

/* PORT E */
#define GPIO_E0_IN_PPS_REG ADCPPS_IN_PPS_REG
#endif /* MCU_18F47K42 || MCU_18F57K42 */

#if defined(MCU_18F57K42)
/* PORT F */
#define GPIO_F0_IN_PPS_REG COG1PPS_IN_PPS_REG
#endif /* MCU_18F57K42 */

/* ========================================================================== */

#endif /* PIC18_PINOUT_CONFIG_H */
