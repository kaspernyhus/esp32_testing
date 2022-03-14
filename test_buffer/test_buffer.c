#include "test_buffer.h"
#include "esp_log.h"


void test_buffer_init(test_buffer_t *tb, uint8_t *buffer, size_t size, size_t delayed_start)
{
    tb->buffer = buffer;
    tb->size = size;
    tb->write = 0;
    tb->is_printed = 0;
    tb->delayed_start = delayed_start;
    memset(tb->buffer,0,size);
}

void test_buffer_add(test_buffer_t *tb, void *data, size_t bytes)
{   
    if(!tb->is_printed) {
        if(tb->delayed_start > 0)
            tb->delayed_start--;
        else {
            if(tb->write+bytes > tb->size) { // buffer will be overflown if a write happens
                ESP_LOGI("test_buffer","---------------------------------------");
                ESP_LOG_BUFFER_HEX("test_buffer",tb->buffer,tb->size);
                tb->is_printed = 1;        
            }
            else {
                // Add data to test buffer
                memcpy(tb->buffer + tb->write,data,bytes);
                tb->write += bytes;
            }
        }
    }
}