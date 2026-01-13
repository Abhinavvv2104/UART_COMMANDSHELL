#ifndef UART_MANAGER_H
#define UART_MANAGER_H

#include "ring_buffer.h"

// Setup the hardware (Pins, Baud Rate)
void uart_manager_init(RingBuffer *rb);

// Start the RTOS task (The listener)
void uart_manager_start(void);

#endif
