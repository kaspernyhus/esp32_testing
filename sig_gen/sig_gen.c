#include <stdio.h>
#include "math.h"
#include "sig_gen.h"
#include "esp_log.h"
#include "sine.h"


void sig_gen_init(sig_gen_t *sg, const sig_gen_config_t *cfg)
{
    sg->ch_fmt = cfg->ch_fmt;
    sg->bytes_per_sample = cfg->bytes_per_sample;
    sg->sample_rate = cfg->sample_rate;
    sg->_amplitude = 32767.0 * cfg->amplitude;
    sg->_freq = cfg->freq;
    sg->_deltaTime = 1.0 / sg->sample_rate;
    sg->_phase = cfg->phase;
    sg->_time = 0.0;
    sg->_double_pi = 2.0*M_PI;

    ESP_LOGI("sigGen","Signal Generator Initialized. sampleRate:%d, ampl:%f, freq:%f, deltaT:%f, phase:%f, dPI:%f", sg->sample_rate,sg->_amplitude,sg->_freq,sg->_deltaTime,sg->_phase,sg->_double_pi);
}


void sig_gen_output(const sig_gen_t *sg, int32_t *out_data, size_t samples)
{
    uint16_t l_sample;
    uint16_t r_sample;
    
    for(int i=0;i<samples;i++) {
        l_sample = get_next_sample();
        // l_sample = _sig_gen_get_sample(sg);
        r_sample = l_sample;
        // Combine l & r
        out_data[i] = (r_sample<<16)|(l_sample & 0x0000ffff);
    }
}


int32_t _sig_gen_get_sample(sig_gen_t *sg)
{
    float angle = sg->_double_pi * sg->_freq * sg->_time + sg->_phase;
    float result = sg->_amplitude * sin(angle);
    sg->_time += sg->_deltaTime;
    return (int16_t)result;
}