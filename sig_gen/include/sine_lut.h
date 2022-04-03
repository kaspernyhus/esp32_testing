#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef enum {
    LUT_FREQ_1K,
    LUT_FREQ_1K3,
    LUT_FREQ_5K3,
    LUT_FREQ_440,
    LUT_FREQ_552,
    LUT_FREQ_60,
    LUT_TEST_A,
    LUT_TEST_B,
    LUT_TEST_A_24,
} lut_freq_e;


typedef struct {
    size_t pos;         // lookup position
    uint32_t *lut;      // pointer to lookup table
    size_t lut_size;    // table size
} lut_gen_t;


void lut_gen_init(lut_gen_t *lsg, lut_freq_e freq);
uint32_t lut_gen_get_sample(lut_gen_t *lsg);
void lut_gen_get_samples(lut_gen_t *lsg, uint32_t *out_buffer, size_t samples);
