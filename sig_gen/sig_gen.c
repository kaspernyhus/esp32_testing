#include <stdio.h>
#include <stdint.h>
#include "math.h"
#include "sig_gen.h"
#include "esp_log.h"


static const char *SIG_TAG = "sigGen";


// Calculate or look up sine sample
uint32_t _sig_gen_get_sample(sig_gen_t *sg)
{
    switch (sg->gen_source) {
        case LUT_GEN:
            return lut_gen_get_sample(sg->lut_gen);

        case CALC_GEN: {
            double angle = sg->_double_pi * sg->_freq * sg->_time + sg->_phase;
            double result = sg->_amplitude * sin(angle);
            sg->_time += sg->_deltaTime;
            return result;
            }
        
        default:
            return 0;
    }   
}


void sig_gen_init(sig_gen_t *sg, const sig_gen_config_t *cfg)
{
    sg->gen_source = cfg->gen_source;
    sg->lut_freq = cfg->lut_freq;
    sg->bytes_per_sample = cfg->bytes_per_sample;
    sg->sample_rate = cfg->sample_rate;
    sg->endianess = cfg->endianess;
    sg->_amplitude = ((pow(2,cfg->bytes_per_sample*8-1))) * cfg->amplitude;
    sg->_freq = cfg->freq;
    sg->_deltaTime = (1.0 / cfg->sample_rate);
    sg->_phase = cfg->phase;
    sg->_time = 0.0;
    sg->_double_pi = 2.0*M_PI;
    sg->initialized = 1;

    // Generate signal from LUT - create obj on the heap
    if(sg->gen_source == LUT_GEN) {
        sg->lut_gen = (lut_gen_t *)malloc(sizeof(lut_gen_t));
        lut_gen_init(sg->lut_gen, sg->lut_freq);
        ESP_LOGI(SIG_TAG,"Signal Generator Initialized. LUT#%d. LUT size: %d. Bytes/sample: %d. Freq: %f", sg->lut_freq, sg->lut_gen->lut_size, sg->bytes_per_sample, (float)(sg->sample_rate)/(sg->lut_gen->lut_size));
    }
    // Generate signal from calculation
    else {
        ESP_LOGI(SIG_TAG,"Signal Generator Initialized. sampleRate:%d, ampl:%f, freq:%f, deltaT:%f, phase:%f", sg->sample_rate,sg->_amplitude,sg->_freq,sg->_deltaTime,sg->_phase);
    }
}

// Outputs a mono sine
// @return number of bytes
size_t sig_gen_output(sig_gen_t *sg, uint8_t *out_data, size_t samples)
{
    if(!sg->initialized) {
        ESP_LOGE(SIG_TAG, "ERROR: Signal generator not initialized!");
        return 0;
    }
    size_t out_index = 0;
    
    switch (sg->bytes_per_sample) {
        case 2: // 16bit LE
            if(sg->endianess == SIG_GEN_LE) {
                for(int i=0;i<samples;i++) {
                    uint16_t sample = (uint16_t)(_sig_gen_get_sample(sg)>>8);
                    out_data[out_index++] = (sample & 0xff);
                    out_data[out_index++] = (sample >> 8) & 0xff;
                }
            }
            else {  // BE
                for(int i=0;i<samples;i++) {
                    uint16_t sample = (uint16_t)(_sig_gen_get_sample(sg)>>8);
                    out_data[out_index++] = (sample >> 8) & 0xff;
                    out_data[out_index++] = (sample & 0xff);
                }
            }
            break;
        
        case 3: // 24bit LE
            if(sg->endianess == SIG_GEN_LE) {
                for(int i=0;i<samples;i++) {
                    uint32_t sample = _sig_gen_get_sample(sg);
                    out_data[out_index++] = (sample & 0xff);
                    out_data[out_index++] = (sample >> 8) & 0xff;
                    out_data[out_index++] = (sample >> 16) & 0xff;
                }
            }
            else {  // BE
                for(int i=0;i<samples;i++) {
                    uint32_t sample = _sig_gen_get_sample(sg);
                    out_data[out_index++] = (sample >> 16) & 0xff;
                    out_data[out_index++] = (sample >> 8) & 0xff;
                    out_data[out_index++] = (sample & 0xff);
                }
            }
            break;
        
        default:
            ESP_LOGE(SIG_TAG,"ERROR: bytes per sample %d", sg->bytes_per_sample);
            break;
        }
    return out_index;
}

// Takes two sig_gen objs and produce a stereo sine
size_t sig_gen_output_combine(sig_gen_t *sg_l, sig_gen_t *sg_r, uint8_t *out_data, size_t samples)
{
    if((!sg_l->initialized) | (!sg_r->initialized)) {
        ESP_LOGE(SIG_TAG, "ERROR: Signal generator not initialized!");
        return 0;
    }

    uint8_t bps_combined = sg_l->bytes_per_sample | sg_r->bytes_per_sample;
    uint8_t end_combined = sg_l->endianess | sg_r->endianess;
    // TODO test for valid (equal) bps and end settings in both channels
    size_t out_index = 0;

    switch (bps_combined) {
        case 2: // 16bit LE
            if(end_combined == SIG_GEN_LE) {
                uint16_t l_sample;
                uint16_t r_sample;

                for(int i=0;i<samples;i++) {
                    l_sample = (uint16_t)(_sig_gen_get_sample(sg_l)>>8);
                    r_sample = (uint16_t)(_sig_gen_get_sample(sg_r)>>8);
                    // Combine l & r
                    uint32_t lr_combined = (r_sample<<16)|(l_sample & 0xffff);

                    out_data[out_index++] = (lr_combined >> 16) & 0xff;
                    out_data[out_index++] = (lr_combined >> 24) & 0xff;
                    out_data[out_index++] = (lr_combined & 0xff);
                    out_data[out_index++] = (lr_combined >> 8) & 0xff;
                }
            }
            else { // BE
                uint16_t l_sample;
                uint16_t r_sample;

                for(int i=0;i<samples;i++) {
                    l_sample = (uint16_t)(_sig_gen_get_sample(sg_l)>>8);
                    r_sample = (uint16_t)(_sig_gen_get_sample(sg_r)>>8);
                    // Combine l & r
                    uint32_t lr_combined = (r_sample<<16)|(l_sample & 0xffff);

                    out_data[out_index++] = (lr_combined >> 24) & 0xff;
                    out_data[out_index++] = (lr_combined >> 16) & 0xff;
                    out_data[out_index++] = (lr_combined >> 8) & 0xff;
                    out_data[out_index++] = (lr_combined & 0xff);
                }
            }
            break;
        
        case 3: // 24bit LE
            if(end_combined == SIG_GEN_LE) {
                uint32_t l_sample;
                uint32_t r_sample;

                for(int i=0;i<samples;i++) {
                    l_sample = _sig_gen_get_sample(sg_l);
                    r_sample = _sig_gen_get_sample(sg_r);
                    
                    // l
                    out_data[out_index++] = (l_sample & 0xff);
                    out_data[out_index++] = (l_sample >> 8) & 0xff;
                    out_data[out_index++] = (l_sample >> 16) & 0xff;

                    // r
                    out_data[out_index++] = (r_sample & 0xff);
                    out_data[out_index++] = (r_sample >> 8) & 0xff;
                    out_data[out_index++] = (r_sample >> 16) & 0xff;
                }
            }
            else { // BE
                uint32_t l_sample;
                uint32_t r_sample;

                for(int i=0;i<samples;i++) {
                    l_sample = _sig_gen_get_sample(sg_l);
                    r_sample = _sig_gen_get_sample(sg_r);
                    
                    // l
                    out_data[out_index++] = (l_sample >> 16) & 0xff;
                    out_data[out_index++] = (l_sample >> 8) & 0xff;
                    out_data[out_index++] = (l_sample & 0xff);

                    // r
                    out_data[out_index++] = (r_sample >> 16) & 0xff;
                    out_data[out_index++] = (r_sample >> 8) & 0xff;
                    out_data[out_index++] = (r_sample & 0xff);
                }
            }
            break;

        default:
            ESP_LOGE(SIG_TAG,"bytes per sample L; %d // bytes per sample R; %d", sg_l->bytes_per_sample, sg_r->bytes_per_sample);
            break;
    }

    return out_index;
}


