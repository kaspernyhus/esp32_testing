#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef enum {
    FREQ_1K,
    FREQ_1K3,
    FREQ_5K3,
    FREQ_440,
    FREQ_552,
    FREQ_60
} lut_freq_e;


typedef struct {
    size_t pos;
    uint16_t *lut;
    size_t lut_size;
} lut_gen_t;

void lut_gen_init(lut_gen_t *lsg, lut_freq_e freq);

uint16_t lut_gen_get_sample(lut_gen_t *lsg);
void lut_gen_get_samples(lut_gen_t *lsg, uint16_t *out_buffer, size_t samples);
