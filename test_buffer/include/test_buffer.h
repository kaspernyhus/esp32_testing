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
} test_buffer_t;


void test_buffer_init(test_buffer_t *tb, uint8_t *buffer, size_t size, size_t delayed_start);
void test_buffer_add(test_buffer_t *tb, void *data, size_t bytes);
