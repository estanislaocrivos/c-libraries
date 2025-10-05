#ifndef USART_DRIVER_H
#define USART_DRIVER_H

/* ============================================================================================== */

#include "config.h"

/* ============================================================================================== */

int8_t usart_initialize(struct serial_port* usart_port);

/* ============================================================================================== */

int8_t usart_transmit(struct serial_port* port, const uint8_t* buffer, size_t size);

/* ============================================================================================== */

int8_t usart_receive(struct serial_port* port, uint8_t* buffer, size_t size);

/* ============================================================================================== */

int8_t usart_set_rx_callback(struct serial_port*  port,
                             serial_rx_callback_t callback,
                             void*                callback_context);

/* ============================================================================================== */

void usart_enable_rx_interrupt(struct serial_port* port, bool enable);

/* ============================================================================================== */

void usart_clear_buffers(struct serial_port* port);

/* ============================================================================================== */

#endif  // USART_DRIVER_H
