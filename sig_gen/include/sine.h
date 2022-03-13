#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>



void get_samples_from_LUT(uint16_t *buffer, size_t samples);
void get_stereo_samples_from_LUT(uint32_t *buffer, size_t samples);
void get_bytes_from_LUT(int8_t *buffer, size_t bytes);