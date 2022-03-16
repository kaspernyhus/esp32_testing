#include <stdio.h>
#include "math.h"
#include "sig_gen.h"
#include "esp_log.h"


void sig_gen_init(sig_gen_t *sg, const sig_gen_config_t *cfg)
{
    sg->gen_source = cfg->gen_source;
    sg->lut_freq = cfg->lut_freq;
    sg->bytes_per_sample = cfg->bytes_per_sample;
    sg->sample_rate = cfg->sample_rate;
    sg->_amplitude = ((pow(2,cfg->bytes_per_sample*8)/2)-1) * cfg->amplitude;
    sg->_freq = cfg->freq;
    sg->_deltaTime = (1.0 / cfg->sample_rate);
    sg->_phase = cfg->phase;
    sg->_time = 0.0;
    sg->_double_pi = 2.0*M_PI;

    if(sg->gen_source == LUT_GEN) {
        sg->lut_gen = (lut_gen_t *)malloc(sizeof(lut_gen_t));
        lut_gen_init(sg->lut_gen,sg->lut_freq);
        ESP_LOGI("sigGen","Signal Generator Initialized. LUT#%d. LUT size: %d", sg->lut_freq, sg->lut_gen->lut_size);
    }
    else ESP_LOGI("sigGen","Signal Generator Initialized. sampleRate:%d, ampl:%f, freq:%f, deltaT:%f, phase:%f", sg->sample_rate,sg->_amplitude,sg->_freq,sg->_deltaTime,sg->_phase);
}


// Outputs a mono sine
size_t sig_gen_output(sig_gen_t *sg, int32_t *out_data, size_t samples)
{
    for(int i=0;i<samples;i++) {
        out_data[i] = _sig_gen_get_sample(sg);
    }
    return samples * sg->bytes_per_sample;
}

// Takes two signal_gen objs and produce a stereo sine
size_t sig_gen_output_combine(sig_gen_t *sg_l, sig_gen_t *sg_r, int32_t *out_data, size_t samples)
{
    uint16_t l_sample;
    uint16_t r_sample;

    for(int i=0;i<samples;i++) {
        l_sample = (uint16_t)_sig_gen_get_sample(sg_l);
        r_sample = (uint16_t)_sig_gen_get_sample(sg_r);
        // Combine l & r
        out_data[i] = (r_sample<<16)|(l_sample & 0x0000ffff);
    }
    return samples * sg_l->bytes_per_sample * 2;
}


// Calculate sine sample
int32_t _sig_gen_get_sample(sig_gen_t *sg)
{   
    if(sg->gen_source == LUT_GEN) {
        return (int32_t)lut_gen_get_sample(sg->lut_gen);
    }
    if(sg->gen_source == CALC_GEN) {
        float angle = sg->_double_pi * sg->_freq * sg->_time + sg->_phase;
        float result = sg->_amplitude * sin(angle);
        sg->_time += sg->_deltaTime;
        return (int32_t)result;
    }
    return 0;
}