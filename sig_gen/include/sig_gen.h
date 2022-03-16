#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sine.h"


typedef enum {
    LUT_GEN,
    CALC_GEN
} gen_source_e;


typedef struct {
    gen_source_e gen_source;
    lut_gen_t *lut_gen;
    lut_freq_e lut_freq;

    uint8_t bytes_per_sample;
    uint16_t sample_rate;

    float _amplitude;
    float _freq;
    float _time;
    double _deltaTime;
    float _phase;
    double _double_pi;
} sig_gen_t;


typedef struct {
    gen_source_e gen_source;
    lut_freq_e lut_freq;
    uint8_t bytes_per_sample;
    uint16_t sample_rate;
    float amplitude;
    float freq;
    float phase;
} sig_gen_config_t;


void sig_gen_init(sig_gen_t *sg, const sig_gen_config_t *cfg);
size_t sig_gen_output( sig_gen_t *sg, int32_t *out_data, size_t samples);
size_t sig_gen_output_combine(sig_gen_t *sg_l, sig_gen_t *sg_r, int32_t *out_data, size_t samples);

int32_t _sig_gen_get_sample(sig_gen_t *sg);

