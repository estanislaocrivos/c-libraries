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

static uart_rx_callback_t usart1_callback;

/* ============================================================================================== */

int8_t usart1_initialize(struct uart_port* usart1_port);

/* ============================================================================================== */

int8_t usart1_transmit(struct uart_port* port, const uint8_t* buffer, size_t size);

/* ============================================================================================== */

int8_t usart1_receive(struct uart_port* port, uint8_t* buffer, size_t size);

/* ============================================================================================== */

int8_t usart1_set_rx_callback(struct uart_port*  port,
                              uart_rx_callback_t callback,
                              void*              callback_context);

/* ============================================================================================== */

void usart1_enable_rx_interrupt(struct uart_port* port, bool enable);

/* ============================================================================================== */

void usart1_clear_buffers(struct uart_port* port);

/* ============================================================================================== */

#endif  // USART_DRIVER_H
