#include "test_buffer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


const char *TB_TAG = "test_buffer";


void test_buffer_init(test_buffer_t *tb, uint8_t *buffer, size_t size, size_t delayed_start, char *tag)
{
    tb->buffer = buffer;
    tb->size = size;
    tb->write = 0;
    tb->is_printed = 0;
    tb->delayed_start = delayed_start;
    tb->tag = tag;
    memset(tb->buffer,0,size);
}

void test_buffer_add(test_buffer_t *tb, void *data, size_t bytes)
{   
    if(!tb->is_printed) {
        if(tb->delayed_start > 0)
            tb->delayed_start--;
        else {
            if(tb->write+bytes > tb->size) { // buffer will be overflown if a write happens
                test_buffer_print(tb);       
            }
            else {
                // Add data to test buffer
                memcpy(tb->buffer + tb->write,data,bytes);
                tb->write += bytes;
            }
        }
    }
}


void test_buffer_add_byte(test_buffer_t *tb, uint8_t data)
{
    if(!tb->is_printed) {
        if(tb->delayed_start > 0)
            tb->delayed_start--;
        else {
            if((tb->write+1) > tb->size) {
                test_buffer_print(tb);      
            }
            else {
                // Add data byte to test buffer
                tb->buffer[tb->write] = data;
                tb->write++;
            }
        }
    }
}


void test_buffer_enable_global(uint8_t *buffer, size_t size, size_t delayed_start)
{
    test_buffer_init(&global_test_buf,buffer,size,delayed_start,"Global test buffer");
    ESP_LOGI(TB_TAG,"Global test_buffer initialized");
}


/*
FreeRTOS task to print the contents of the object buffer
*/
void _test_buffer_print_task(void *arg)
{
    test_buffer_t *tb = (test_buffer_t *)arg; // Get test_buffer_t obj
    ESP_LOGI("","-------------------------------------------------");
    ESP_LOGI("","%s",tb->tag);
    ESP_LOGI("","-------------------------------------------------");
    ESP_LOG_BUFFER_HEX("test_buffer",tb->buffer,tb->size);
    vTaskDelete(NULL);
}


/*
Creates a freeRTOS task to print the buffer
*/
void test_buffer_print(test_buffer_t *tb)
{
    xTaskCreatePinnedToCore(_test_buffer_print_task,"test_buffer print",8192,tb,10,NULL,APP_CPU_NUM);
    tb->is_printed = 1;
}

