#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"


typedef struct {
    uint8_t *buffer;
    size_t size;
    size_t write;
    size_t read;
} ringbuf_t;



void ringbuf_init(ringbuf_t *rb, uint8_t *buffer, size_t size);
void ringbuf_reset(ringbuf_t *rb);
uint8_t ringbuf_full(ringbuf_t *rb);

void ringbuf_write(ringbuf_t *rb, void *data, size_t size);
void ringbuf_read(ringbuf_t *rb, void *data, size_t bytes);



void ringbuf_get_test_samples(uint8_t *buffer, uint16_t bytes);