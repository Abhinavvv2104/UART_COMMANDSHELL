#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include<stdbool.h>
#define BUFFER_SIZE 128

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint8_t tail;

}RingBuffer;

void ring_buffer_init(RingBuffer *rb);
bool ring_buffer_write(RingBuffer *rb,uint8_t data);
bool ring_buffer_read(RingBuffer *rb,uint8_t *data);


#endif
