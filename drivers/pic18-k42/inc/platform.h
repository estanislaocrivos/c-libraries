#ifndef PLATFORM_H
#define PLATFORM_H

/* ========================================================================== */

#include "pic18_adcc.h"
#include "pic18_critical.h"
#include "pic18_delay.h"
#include "pic18_gpio.h"
#include "pic18_i2c1.h"
#include "pic18_i2c2.h"
#include "pic18_interrupts.h"
#include "pic18_mcu.h"
#include "pic18_nvm.h"
#include "pic18_pinout.h"
#include "pic18_spi1.h"
#include "pic18_uart1.h"
#include "pic18_uart2.h"

/* ========================================================================== */

#define PLATFORM_MCU_OPS       &pic18_mcu_ops
#define PLATFORM_GPIO_OPS      &pic18_gpio_ops
#define PLATFORM_SERIAL1_OPS   &pic18_uart1_ops
#define PLATFORM_SERIAL2_OPS   &pic18_uart2_ops
#define PLATFORM_I2C1_OPS      &pic18_i2c1_ops
#define PLATFORM_I2C2_OPS      &pic18_i2c2_ops
#define PLATFORM_SPI1_OPS      &pic18_spi1_ops
#define PLATFORM_ADC_OPS       &pic18_adcc_ops
#define PLATFORM_NVM_OPS       &pic18_nvm_ops
#define PLATFORM_INTERRUPT_OPS &pic18_interrupt_ops

/* ========================================================================== */

#endif  // PLATFORM_H
