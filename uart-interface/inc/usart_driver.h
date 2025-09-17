/**
 * @file
 * @brief This is a mock header file for the USART1 peripheral driver. This file contains the
 * function prototypes for the USART1 driver. Used for testing.
 */

/* ============================================================================================== */

#ifndef USART_DRIVER_H
#define USART_DRIVER_H

/* ============================================================================================== */

#include <stdint.h>
#include <stddef.h>

/* ============================================================================================== */

#include "uart.h"
#include "errno.h"

/* ============================================================================================== */

static void* usart1_callback_context;

uart_rx_callback_t usart1_callback_t;

/* ============================================================================================== */

int8_t usart1_initialize(struct uart_port* usart1_port);

/* ============================================================================================== */

int8_t usart1_transmit(const uint8_t* tx_buffer, size_t length);

/* ============================================================================================== */

int8_t usart1_receive(uint8_t* rx_buffer, size_t length);

/* ============================================================================================== */

int8_t usart1_set_rx_callback(usart1_callback_t callback);

/* ============================================================================================== */

void usart1_clear_buffers(void);

/* ============================================================================================== */

#endif  // USART_DRIVER_H
