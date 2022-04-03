# Signal generator

Can generate a sine either from a lookup table or a calculation. Lookup is more precise.

## Configuration
`gen_source` selects generator source: `LUT_GEN` or `CALC_GEN`



### LUT frequencies
    LUT_FREQ_1K,
    LUT_FREQ_1K3,
    LUT_FREQ_5K3,
    LUT_FREQ_440,
    LUT_FREQ_552,
    LUT_FREQ_60,
    LUT_TEST_A,
    LUT_TEST_B,
    LUT_TEST_A_24,


## Example (calculating samples)
```
sig_gen_t sineL;
sig_gen_t sineR;

sig_gen_config_t cfg = {
    .gen_source = CALC_GEN,
    .bytes_per_sample = 2,
    .sample_rate = SAMPLE_RATE,
    .amplitude = 1.0,
    .freq = 1000,
    .phase = 0,
};
sig_gen_init(&sineL, &cfg);
cfg.freq = 500;
sig_gen_init(&sineR, &cfg);


// Get samples (stereo)
sig_gen_output_combine(&sineL, &sineR, samples, 480);
```

## Example (lookup samples)
```
sig_gen_t sineL;
sig_gen_t sineR;

sig_gen_config_t cfg = {
    .gen_source = LUT_GEN,
    .lut_freq = LUT_FREQ_1K,
    .bytes_per_sample = 2,
    .sample_rate = SAMPLE_RATE,
};
sig_gen_init(&sineL, &cfg);
cfg.lut_freq = LUT_FREQ_440;
sig_gen_init(&sineR, &cfg);


// Get samples (stereo)
sig_gen_output_combine(&sineL, &sineR, samples, 480);
```