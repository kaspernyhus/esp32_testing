#include "circular_buffer.h"
#include "esp_log.h"

const char *RB_TAG = "ringbuffer";
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

/*
Initialize ringbuffer
@rb: ringbuffer obj
@buffer: user provided buffer
@size: size of user buffer in bytes
*/
void ringbuf_init(ringbuf_t *rb, uint8_t *buffer, size_t size)
{
    rb->buffer = buffer;
    rb->size = size;
    ringbuf_reset(rb);
    ESP_LOGI(RB_TAG,"Ringbuffer created. Size: %d bytes", rb->size);
}


void ringbuf_reset(ringbuf_t *rb)
{
    memset(rb->buffer,0,rb->size);
    rb->write = 0;
    rb->read = 0;
}


/*
Writes to a ringbuffer obj
@data: data to be written
@bytes: number of bytes to be written
return: number of actual bytes written
*/
size_t ringbuf_write(ringbuf_t *rb, void *data, size_t bytes)
{
    size_t bytes_written = 0;
    if(bytes > rb->size) {
        ESP_LOGE(RB_TAG,"ERROR: Trying to write more bytes than size of buffer");
        return bytes_written;
    }
    portENTER_CRITICAL(&spinlock);
    // too big to fit, only write available
    if(rb->write + bytes > rb->size) {
        size_t first_block = rb->size - rb->write;
        size_t second_block = bytes - first_block;
        memcpy(rb->buffer + rb->write,data,first_block);
        memcpy(rb->buffer,data + first_block,second_block);
        rb->write = second_block;
        bytes_written = first_block + second_block;
    }
    // write all data
    else {
        memcpy(rb->buffer + rb->write,data,bytes);
        rb->write = (rb->write + bytes) % rb->size;
        bytes_written = bytes;
    }
    portEXIT_CRITICAL(&spinlock);
    
    return bytes_written;
}


/*
Reads from a ringbuffer obj
@data: data to be read to
@bytes: number of bytes to be read
return: number of actual bytes read
*/
size_t ringbuf_read(ringbuf_t *rb, void *data, size_t bytes)
{
    size_t bytes_read = 0;
    if(bytes > rb->size) {
        ESP_LOGE(RB_TAG,"ERROR: Trying to read more bytes than size of buffer");
        return bytes_read;
    }
    portENTER_CRITICAL(&spinlock);
    // wrap around read
    if(rb->read + bytes > rb->size) {
        size_t first_block = rb->size - rb->read;
        size_t second_block = bytes - first_block;
        memcpy(data,rb->buffer + rb->read,first_block);
        memcpy(data + first_block,rb->buffer,second_block);
        rb->read = second_block;
        bytes_read = first_block + second_block;
    }
    // read all data out in one go
    else {
        memcpy(data,rb->buffer + rb->read,bytes);
        rb->read = (rb->read + bytes) % rb->size;
        bytes_read = bytes;
    }
    portEXIT_CRITICAL(&spinlock);
    return bytes_read;
}

/*
Returns true if ringbuffer is full (or empty...)
*/
uint8_t ringbuf_full(ringbuf_t *rb)
{
    return rb->read == rb->write;
}

 