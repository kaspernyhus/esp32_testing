#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MONO 1
#define STEREO 2


typedef struct {
    uint8_t ch_fmt;
    uint8_t bytes_per_sample;
    uint16_t sample_rate;

    float _amplitude;
    float _freq;
    float _time;
    float _deltaTime;
    float _phase;
    float _double_pi;
} sig_gen_t;


typedef struct {
    uint8_t ch_fmt;
    uint8_t bytes_per_sample;
    uint16_t sample_rate;
    float amplitude;
    float freq;
    float phase;
} sig_gen_config_t;



void sig_gen_init(sig_gen_t *sg, const sig_gen_config_t *cfg);
void sig_gen_output(const sig_gen_t *sg, int32_t *out_data, size_t samples);


int32_t _sig_gen_get_sample(sig_gen_t *sg);

