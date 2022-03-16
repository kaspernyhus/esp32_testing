#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"


/*
public:
    CircularBuffer(size_t dataCapacity);

    size_t size() const { return dataSize; }
    size_t capacity() const { return dataCapacity; }
    size_t write(const uint8_t *data, size_t bytes);
    size_t read(uint8_t *data, size_t bytes);
    void emptyBuffer();

private:
    std::mutex bufferMutex;
    size_t begIndex = 0;
    size_t endIndex = 0;
    size_t dataSize = 0;
    size_t dataCapacity = 0;
    std::vector<uint8_t> buffer;
*/


typedef struct {
    uint8_t *buffer;    // Statically allocated user provided buffer
    size_t size;        // Buffer size in bytes
    size_t write;       // Write index
    size_t read;        // Read index
} ringbuf_t;



void ringbuf_init(ringbuf_t *rb, uint8_t *buffer, size_t size);
void ringbuf_reset(ringbuf_t *rb);
uint8_t ringbuf_full(ringbuf_t *rb);

size_t ringbuf_write(ringbuf_t *rb, void *data, size_t size);
size_t ringbuf_read(ringbuf_t *rb, void *data, size_t bytes);

void ringbuf_print_info(const ringbuf_t *rb);
void ringbuf_print(const ringbuf_t *rb);