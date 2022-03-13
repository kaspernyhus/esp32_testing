#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MONO 1
#define STEREO 2


typedef struct
{
    uint8_t ch_fmt;
    uint8_t bytes_per_sample;
} sig_gen_t;


void sig_gen_init(sig_gen_t *sg, uint8_t ch_fmt);
void sig_gen_output(sig_gen_t *sg, void *out_data, size_t samples);

