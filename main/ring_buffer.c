#include"ring_buffer.h"

void ring_buffer_init(RingBuffer *rb){
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_write(RingBuffer *rb,uint8_t data){
    uint16_t next_head;
    next_head=(rb->head +1) % BUFFER_SIZE;
    // check if full ( next_head hits tail)
    if(next_head == rb->tail ){
        return false;
    }
    rb->buffer[rb->head]=data;
    rb->head=next_head;
    return true;

}

bool ring_buffer_read(RingBuffer *rb,uint8_t *data){
    //check if empty(head equals tail)
    if(rb->tail== rb->head){
        return false;
    }
    *data= rb->buffer[rb->tail];
    rb->tail=(rb->tail + 1) % BUFFER_SIZE;
    return true;
}