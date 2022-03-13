#include "sine.h"


uint16_t b_sample_pos = 0;
uint16_t w_sample_pos = 0;
uint16_t l_sample_pos = 0;
uint16_t r_sample_pos = 0;
uint16_t ramp_val = 0;


// // 1kHz
// uint16_t sine_lut[] = {
//     0x0000,0x03d3,0x0795,0x0b36,
//     0x0ea6,0x11d6,0x14b7,0x173e,
//     0x195f,0x1b11,0x1c4c,0x1d0c,
//     0x1d4c,0x1d0c,0x1c4c,0x1b11,
//     0x195f,0x173e,0x14b7,0x11d6,
//     0x0ea6,0x0b36,0x0795,0x03d3,
//     0x0000,0xfc2d,0xf86b,0xf4ca,
//     0xf15a,0xee2a,0xeb49,0xe8c2,
//     0xe6a1,0xe4ef,0xe3b4,0xe2f4,
//     0xe2b4,0xe2f4,0xe3b4,0xe4ef,
//     0xe6a1,0xe8c2,0xeb49,0xee2a,
//     0xf15a,0xf4ca,0xf86b,0xfc2d,
//     };



// uint16_t sine_lut[] = {
//     0x0,0x10b5,0x2120,0x30fb,0x3fff,0x4deb,0x5a82,0x658c,
//     0x6ed9,0x7641,0x7ba2,0x7ee7,0x7fff,0x7ee7,0x7ba2,0x7641,
//     0x6ed9,0x658c,0x5a82,0x4deb,0x3fff,0x30fb,0x2120,0x10b5,
//     0x0,0xef4a,0xdedf,0xcf04,0xc000,0xb214,0xa57d,0x9a73,
//     0x9126,0x89be,0x845d,0x8118,0x8000,0x8118,0x845d,0x89be,
//     0x9126,0x9a73,0xa57d,0xb214,0xc000,0xcf04,0xdedf,0xef4a,
// };



// uint16_t sine_lut[] = {
//     0x0,0x36f,0x6de,0xa4c,0xdb8,0x1122,0x1488,0x17ea,
//     0x1b48,0x1ea1,0x21f5,0x2542,0x2888,0x2bc7,0x2efd,0x322b,
//     0x3550,0x386b,0x3b7b,0x3e81,0x417b,0x4469,0x474a,0x4a1e,
//     0x4ce4,0x4f9d,0x5246,0x54e0,0x576b,0x59e6,0x5c50,0x5ea8,
//     0x60f0,0x6325,0x6549,0x6759,0x6957,0x6b41,0x6d17,0x6ed9,
//     0x7087,0x7220,0x73a3,0x7512,0x766b,0x77ae,0x78db,0x79f1,
//     0x7af1,0x7bdb,0x7cad,0x7d69,0x7e0d,0x7e9a,0x7f10,0x7f6e,
//     0x7fb5,0x7fe4,0x7ffc,0x7ffc,0x7fe4,0x7fb5,0x7f6e,0x7f10,
//     0x7e9a,0x7e0d,0x7d69,0x7cad,0x7bdb,0x7af1,0x79f1,0x78db,
//     0x77ae,0x766b,0x7512,0x73a3,0x7220,0x7087,0x6ed9,0x6d17,
//     0x6b41,0x6957,0x6759,0x6549,0x6325,0x60f0,0x5ea8,0x5c50,
//     0x59e6,0x576b,0x54e0,0x5246,0x4f9d,0x4ce4,0x4a1e,0x474a,
//     0x4469,0x417b,0x3e81,0x3b7b,0x386b,0x3550,0x322b,0x2efd,
//     0x2bc7,0x2888,0x2542,0x21f5,0x1ea1,0x1b48,0x17ea,0x1488,
//     0x1122,0xdb8,0xa4c,0x6de,0x36f,0x0,0xfc90,0xf921,
//     0xf5b3,0xf247,0xeedd,0xeb77,0xe815,0xe4b7,0xe15e,0xde0a,
//     0xdabd,0xd777,0xd438,0xd102,0xcdd4,0xcaaf,0xc794,0xc484,
//     0xc17e,0xbe84,0xbb96,0xb8b5,0xb5e1,0xb31b,0xb062,0xadb9,
//     0xab1f,0xa894,0xa619,0xa3af,0xa157,0x9f0f,0x9cda,0x9ab6,
//     0x98a6,0x96a8,0x94be,0x92e8,0x9126,0x8f78,0x8ddf,0x8c5c,
//     0x8aed,0x8994,0x8851,0x8724,0x860e,0x850e,0x8424,0x8352,
//     0x8296,0x81f2,0x8165,0x80ef,0x8091,0x804a,0x801b,0x8003,
//     0x8003,0x801b,0x804a,0x8091,0x80ef,0x8165,0x81f2,0x8296,
//     0x8352,0x8424,0x850e,0x860e,0x8724,0x8851,0x8994,0x8aed,
//     0x8c5c,0x8ddf,0x8f78,0x9126,0x92e8,0x94be,0x96a8,0x98a6,
//     0x9ab6,0x9cda,0x9f0f,0xa157,0xa3af,0xa619,0xa894,0xab1f,
//     0xadb9,0xb062,0xb31b,0xb5e1,0xb8b5,0xbb96,0xbe84,0xc17e,
//     0xc484,0xc794,0xcaaf,0xcdd4,0xd102,0xd438,0xd777,0xdabd,
//     0xde0a,0xe15e,0xe4b7,0xe815,0xeb77,0xeedd,0xf247,0xf5b3,
//     0xf921,0xfc90,
// };



// // Ramp
// uint16_t sine_lut[] = {
//     0x0000,0x029b,0x0535,0x07d0,0x0a6b,0x0d05,0x0fa0,0x123b,
//     0x14d5,0x1770,0x1a0b,0x1ca5,0x1f40,0x21db,0x2475,0x2710,
//     0x29ab,0x2c45,0x2ee0,0x317b,0x3415,0x36b0,0x394b,0x3be5,
//     0x3e80,0x411b,0x43b5,0x4650,0x48eb,0x4b85,0x4e20,0x50bb,
//     0x5355,0x55f0,0x588b,0x5b25,0x5dc0,0x605b,0x62f5,0x6590,
//     0x682b,0x6ac5,0x6d60,0x6ffb,0x7295,0x7530,0x77cb,0x7a65,
// };


// TEST
uint16_t sine_lut[] = {
    0x2301,0x2301,0x2301,0x2301,0x2301,0x2301,0x2301,0x2301,
    0x6745,0x6745,0x6745,0x6745,0x6745,0x6745,0x6745,0x6745,
    0xab89,0xab89,0xab89,0xab89,0xab89,0xab89,0xab89,0xab89,
    0xefcd,0xefcd,0xefcd,0xefcd,0xefcd,0xefcd,0xefcd,0xefcd,
    0xadde,0xadde,0xadde,0xadde,0xadde,0xadde,0xadde,0xadde,
    0xdec0,0xdec0,0xdec0,0xdec0,0xdec0,0xdec0,0xdec0,0xdec0,
    0xefbe,0xefbe,0xefbe,0xefbe,0xefbe,0xefbe,0xefbe,0xefbe,
    0xbeba,0xbeba,0xbeba,0xbeba,0xbeba,0xbeba,0xbeba,0xbeba,
    0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,0x1234,
};




uint16_t left_sine_lut[] = {
    0x0,0x5ff,0xbfc,0x11f2,0x17dd,0x1dbc,0x2389,0x2943,
    0x2ee5,0x346d,0x39d7,0x3f21,0x4447,0x4947,0x4e1e,0x52c9,
    0x5745,0x5b90,0x5fa7,0x6389,0x6732,0x6aa2,0x6dd5,0x70cb,
    0x7381,0x75f7,0x782a,0x7a19,0x7bc3,0x7d28,0x7e47,0x7f1e,
    0x7fae,0x7ff6,0x7ff6,0x7fae,0x7f1e,0x7e47,0x7d28,0x7bc3,
    0x7a19,0x782a,0x75f7,0x7381,0x70cb,0x6dd5,0x6aa2,0x6732,
    0x6389,0x5fa7,0x5b90,0x5745,0x52c9,0x4e1e,0x4947,0x4447,
    0x3f21,0x39d7,0x346d,0x2ee5,0x2943,0x2389,0x1dbc,0x17dd,
    0x11f2,0xbfc,0x5ff,0x0,0xfa00,0xf403,0xee0d,0xe822,
    0xe243,0xdc76,0xd6bc,0xd11a,0xcb92,0xc628,0xc0de,0xbbb8,
    0xb6b8,0xb1e1,0xad36,0xa8ba,0xa46f,0xa058,0x9c76,0x98cd,
    0x955d,0x922a,0x8f34,0x8c7e,0x8a08,0x87d5,0x85e6,0x843c,
    0x82d7,0x81b8,0x80e1,0x8051,0x8009,0x8009,0x8051,0x80e1,
    0x81b8,0x82d7,0x843c,0x85e6,0x87d5,0x8a08,0x8c7e,0x8f34,
    0x922a,0x955d,0x98cd,0x9c76,0xa058,0xa46f,0xa8ba,0xad36,
    0xb1e1,0xb6b8,0xbbb8,0xc0de,0xc628,0xcb92,0xd11a,0xd6bc,
    0xdc76,0xe243,0xe822,0xee0d,0xf403,0xfa00,
};

uint16_t right_sine_lut[] = {
    0x0,0x10b5,0x2120,0x30fb,0x3fff,0x4deb,0x5a82,0x658c,
    0x6ed9,0x7641,0x7ba2,0x7ee7,0x7fff,0x7ee7,0x7ba2,0x7641,
    0x6ed9,0x658c,0x5a82,0x4deb,0x3fff,0x30fb,0x2120,0x10b5,
    0x0,0xef4a,0xdedf,0xcf04,0xc000,0xb214,0xa57d,0x9a73,
    0x9126,0x89be,0x845d,0x8118,0x8000,0x8118,0x845d,0x89be,
    0x9126,0x9a73,0xa57d,0xb214,0xc000,0xcf04,0xdedf,0xef4a,
};


void get_bytes_from_LUT(int8_t *buffer, size_t bytes)
{
    for(int i=0; i<bytes; i++) {
        
        buffer[i] = (int8_t)(sine_lut[b_sample_pos])&0x00FF;
        i++;
        buffer[i] = (int8_t)(sine_lut[b_sample_pos]>>8)&0x00FF;

        if(b_sample_pos < sizeof(sine_lut))
            b_sample_pos++;
        else
            b_sample_pos = 0;
    }
}

void get_samples_from_LUT(uint16_t *buffer, size_t samples)
{
    for(int i=0; i<samples; i++) {
        buffer[i] = sine_lut[w_sample_pos];
        if(w_sample_pos < sizeof(sine_lut)/sizeof(sine_lut[0])-1)
            w_sample_pos++;
        else
            w_sample_pos = 0;
    }
}

void get_stereo_samples_from_LUT(uint32_t *buffer, size_t samples)
{
    uint16_t l_buf;
    uint16_t r_buf;

    for(int i=0; i<samples; i++) {
        l_buf = left_sine_lut[l_sample_pos];
        // r_buf = ramp_val;
        r_buf = right_sine_lut[r_sample_pos];
        
        // Combine l & r
        buffer[i] = (r_buf<<16)|(l_buf & 0x0000ffff);
        
        if(l_sample_pos < sizeof(left_sine_lut)/sizeof(left_sine_lut[0])-1)
            l_sample_pos++;
        else
            l_sample_pos = 0;

        if(r_sample_pos < sizeof(right_sine_lut)/sizeof(right_sine_lut[0])-1)
            r_sample_pos++;
        else
            r_sample_pos = 0;

        ramp_val++;
    }
}