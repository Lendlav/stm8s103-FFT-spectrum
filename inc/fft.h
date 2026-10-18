/* SPDX-License-Identifier: MIT */
#ifndef FFT_H
#define FFT_H

#include <stdint.h>

#include "app_config.h"

typedef struct {
  int16_t real;
  int16_t imag;
} fft_complex_t;

/* Apply a Hann window and transform N_SAMPLE real samples in place. */
void FFT_Process(fft_complex_t samples[N_SAMPLE]);

/* Return the integer magnitude of one transformed bin. */
uint16_t FFT_Magnitude(const fft_complex_t *value);

#endif
