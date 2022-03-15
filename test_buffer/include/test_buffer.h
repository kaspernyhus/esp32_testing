#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef struct
{
    uint8_t *buffer;
    size_t size;
    size_t write;
    uint8_t is_printed;
    size_t delayed_start;
    char *tag;
} test_buffer_t;


/* Global test buffer */
test_buffer_t global_test_buf;


void test_buffer_init(test_buffer_t *tb, uint8_t *buffer, size_t size, size_t delayed_start, char *tag);
void test_buffer_add(test_buffer_t *tb, void *data, size_t bytes);
void test_buffer_add_byte(test_buffer_t *tb, uint8_t data);

void test_buffer_enable_global(uint8_t *buffer, size_t size, size_t delayed_start);

void test_buffer_print(test_buffer_t *tb);