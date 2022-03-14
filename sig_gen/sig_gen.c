#include <stdio.h>
#include "sig_gen.h"
#include "sine.h"


void sig_gen_init(sig_gen_t *sg, uint8_t ch_fmt)
{
    sg->ch_fmt = ch_fmt;
    sg->bytes_per_sample = 2;
}


void sig_gen_output(sig_gen_t *sg, void *out_data, size_t samples)
{
    if(sg->ch_fmt > 1) {
        get_stereo_samples_from_LUT(out_data,samples);
    }
    else {
        get_samples_from_LUT(out_data,samples);
    }
}


void sig_gen_get_sample(sig_gen_t *sg, void *out_data, size_t samples)
{
    
}