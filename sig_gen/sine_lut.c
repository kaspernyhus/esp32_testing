#include "sine_lut.h"


static uint32_t sine_lut_1k[] = {
    0x00000000,0x0010b514,0x002120fb,0x0030fbc5,0x003fffff,0x004debe4,0x005a8279,0x00658c99,
    0x006ed9eb,0x007641ae,0x007ba374,0x007ee7a9,0x007fffff,0x007ee7a9,0x007ba374,0x007641ae,
    0x006ed9eb,0x00658c99,0x005a8279,0x004debe4,0x003fffff,0x0030fbc5,0x002120fb,0x0010b514,
    0x00000000,0x00ef4aec,0x00dedf05,0x00cf043b,0x00c00001,0x00b2141c,0x00a57d87,0x009a7367,
    0x00912615,0x0089be52,0x00845c8c,0x00811857,0x00800001,0x00811857,0x00845c8c,0x0089be52,
    0x00912615,0x009a7367,0x00a57d87,0x00b2141c,0x00c00001,0x00cf043b,0x00dedf05,0x00ef4aec,
};


static uint32_t sine_lut_1k3[] = {
    0x00000000,0x0015ad3e,0x002aba2e,0x003e8b23,0x00508d91,0x00603c49,0x006d234f,0x0076e33a,
    0x007d33f0,0x007fe6bc,0x007ee7a9,0x007a3e17,0x00720c80,0x00668f7b,0x00581c00,0x00471cec,
    0x00340ff2,0x001f81f3,0x000a0af2,0x00f449ad,0x00dedf05,0x00ca695c,0x00b78001,0x00a6aece,
    0x00987222,0x008d3347,0x00854566,0x0080e323,0x00802ce9,0x008327fd,0x0089be52,0x0093bf32,
    0x00a0e0a2,0x00b0c188,0x00c2ec77,0x00d6db13,0x00ebf9f5,
};

static uint32_t sine_lut_5k3[] = {
    0x00000000,0x005246dd,0x007e0e2e,0x006ed9eb,0x002bc751,0x00d438af,0x00912615,0x0081f1d2,
    0x00adb923,
};

static uint32_t sine_lut_440[] = {
    0x00000000,0x00075fd4,0x000eb963,0x0016066d,0x001d40bb,0x00246229,0x002b64a6,0x0032423c,
    0x0038f515,0x003f777e,0x0045c3ee,0x004bd50a,0x0051a5a9,0x005730d8,0x005c71e3,0x0061644f,
    0x006603e9,0x006a4cc2,0x006e3b35,0x0071cbea,0x0074fbd9,0x0077c84c,0x007a2ee2,0x007c2d91,
    0x007dc2a5,0x007eecc7,0x007faaf8,0x007ffc98,0x007fe161,0x007f596a,0x007e6526,0x007d0566,
    0x007b3b54,0x00790875,0x00766eaa,0x00737027,0x00700f79,0x006c4f7f,0x0068336a,0x0063beb7,
    0x005ef531,0x0059daeb,0x0054743b,0x004ec5b9,0x0048d43a,0x0042a4cc,0x003c3cb1,0x0035a15d,
    0x002ed86c,0x0027e7a6,0x0020d4f0,0x0019a64e,0x001261dc,0x000b0dc9,0x0003b04f,0x00fc4fb1,
    0x00f4f237,0x00ed9e24,0x00e659b2,0x00df2b10,0x00d8185a,0x00d12794,0x00ca5ea3,0x00c3c34f,
    0x00bd5b34,0x00b72bc6,0x00b13a47,0x00ab8bc5,0x00a62515,0x00a10acf,0x009c4149,0x0097cc96,
    0x0093b081,0x008ff087,0x008c8fd9,0x00899156,0x0086f78b,0x0084c4ac,0x0082fa9a,0x00819ada,
    0x0080a696,0x00801e9f,0x00800368,0x00805508,0x00811339,0x00823d5b,0x0083d26f,0x0085d11e,
    0x008837b4,0x008b0427,0x008e3416,0x0091c4cb,0x0095b33e,0x0099fc17,0x009e9bb1,0x00a38e1d,
    0x00a8cf28,0x00ae5a57,0x00b42af6,0x00ba3c12,0x00c08882,0x00c70aeb,0x00cdbdc4,0x00d49b5a,
    0x00db9dd7,0x00e2bf45,0x00e9f993,0x00f1469d,0x00f8a02c,
};


static uint32_t sine_lut_552[] = {
    0x00000000,0x00093c77,0x00126c9a,0x001b8427,0x002476f9,0x002d3921,0x0035beed,0x003dfcfd,
    0x0045e852,0x004d7659,0x00549cfd,0x005b52b3,0x00618e86,0x00674825,0x006c77eb,0x007116ec,
    0x00751efd,0x00788abd,0x007b559b,0x007d7bdc,0x007efaa3,0x007fcff1,0x007ffaa8,0x007f7a90,
    0x007e5054,0x007c7d82,0x007a0489,0x0076e8b5,0x00732e2d,0x006ed9eb,0x0069f1b6,0x00647c1b,
    0x005e8062,0x00580689,0x00511733,0x0049bba4,0x0041fdab,0x0039e79f,0x0031844a,0x0028dedf,
    0x002002e8,0x0016fc37,0x000dd6d8,0x00049f01,0x00fb60ff,0x00f22928,0x00e903c9,0x00dffd18,
    0x00d72121,0x00ce7bb6,0x00c61861,0x00be0255,0x00b6445c,0x00aee8cd,0x00a7f977,0x00a17f9e,
    0x009b83e5,0x00960e4a,0x00912615,0x008cd1d3,0x0089174b,0x0085fb77,0x0083827e,0x0081afac,
    0x00808570,0x00800558,0x0080300f,0x0081055d,0x00828424,0x0084aa65,0x00877543,0x008ae103,
    0x008ee914,0x00938815,0x0098b7db,0x009e717a,0x00a4ad4d,0x00ab6303,0x00b289a7,0x00ba17ae,
    0x00c20303,0x00ca4113,0x00d2c6df,0x00db8907,0x00e47bd9,0x00ed9366,0x00f6c389,
};

static uint32_t sine_lut_TEST_A[] = {
    0x000000,0x111111,0x222222,0x333333,0x444444,0x555555,0x666666,0x777777,
    0x888888,0x999999,0xaaaaaa,0xbbbbbb,0xcccccc,0xdddddd,0xeeeeee,0xffffff,
};

static uint32_t sine_lut_TEST_B[] = {
    0x123456,0x123456,0x123456,0x123456,0x123456,0x123456,0x123456,0x123456,
    0x123456,0x123456,0x123456,0x123456,0x123456,0x123456,0x123456,0x123456,
};


/*
Reference a sine LUT and get size
*/
void lut_gen_init(lut_gen_t *lsg, lut_freq_e freq)
{
    switch (freq) {
        case LUT_FREQ_1K:
            lsg->lut = sine_lut_1k;
            lsg->lut_size = sizeof(sine_lut_1k)/4;
            break;
        case LUT_FREQ_1K3:
            lsg->lut = sine_lut_1k3;
            lsg->lut_size = sizeof(sine_lut_1k3)/4;
            break;
        case LUT_FREQ_440:
            lsg->lut = sine_lut_440;
            lsg->lut_size = sizeof(sine_lut_440)/4;
            break;
        case LUT_FREQ_552:
            lsg->lut = sine_lut_552;
            lsg->lut_size = sizeof(sine_lut_552)/4;
            break;
        case LUT_FREQ_5K3:
            lsg->lut = sine_lut_5k3;
            lsg->lut_size = sizeof(sine_lut_5k3)/4;
            break;
        case LUT_TEST_A:
            lsg->lut = sine_lut_TEST_A;
            lsg->lut_size = sizeof(sine_lut_TEST_A)/4;
            break;
        case LUT_TEST_B:
            lsg->lut = sine_lut_TEST_B;
            lsg->lut_size = sizeof(sine_lut_TEST_B)/4;
            break;

        default:
            lsg->lut = sine_lut_1k;
            lsg->lut_size = sizeof(sine_lut_1k)/4;
            break;
    }
    lsg->pos = 0;
}


/*
Returns one sample
*/
uint32_t lut_gen_get_sample(lut_gen_t *lsg)
{
    uint32_t sample = lsg->lut[lsg->pos];
    if(lsg->pos < lsg->lut_size-1)
        lsg->pos++;
    else
        lsg->pos = 0;
    return sample;
}
