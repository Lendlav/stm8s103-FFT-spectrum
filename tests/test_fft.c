/* SPDX-License-Identifier: MIT */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "fft.h"

static fft_complex_t samples[N_SAMPLE];

static void fail(const char *message, uint16_t bin, uint16_t magnitude)
{
  fprintf(stderr, "N=%u: %s (bin=%u magnitude=%u)\n",
          (unsigned)N_SAMPLE, message, (unsigned)bin, (unsigned)magnitude);
  exit(EXIT_FAILURE);
}

static uint16_t peak_bin(uint16_t *peak_magnitude)
{
  uint16_t bin;
  uint16_t peak = 0u;
  uint16_t peak_index = 0u;
  uint16_t magnitude;

  for (bin = 0u; bin <= N_SAMPLE / 2u; ++bin) {
    magnitude = FFT_Magnitude(&samples[bin]);
    if (magnitude > peak) {
      peak = magnitude;
      peak_index = bin;
    }
  }
  *peak_magnitude = peak;
  return peak_index;
}

static void load_tone(uint16_t bin, int16_t amplitude)
{
  uint16_t index;
  double angle;

  for (index = 0u; index < N_SAMPLE; ++index) {
    angle = (2.0 * 3.14159265358979323846 * bin * index) / N_SAMPLE;
    samples[index].real = (int16_t)lround(amplitude * sin(angle));
    samples[index].imag = 0;
  }
}

static void check_tone(uint16_t expected_bin)
{
  uint16_t measured;
  uint16_t magnitude;

  load_tone(expected_bin, 200);
  FFT_Process(samples);
  measured = peak_bin(&magnitude);
  if (measured != expected_bin) {
    fail("sine peak moved to the wrong bin", measured, magnitude);
  }
  /* Hann coherent gain plus per-stage scaling predicts about A/4 here. */
  if ((magnitude < 35u) || (magnitude > 60u)) {
    fail("sine magnitude outside the expected fixed-point range", measured, magnitude);
  }
}

int main(void)
{
  uint16_t index;
  uint16_t magnitude;
  uint16_t measured;
  fft_complex_t five = { 3, 4 };

  if (FFT_Magnitude(&five) != 5u) {
    fail("3-4-5 magnitude check failed", 0u, FFT_Magnitude(&five));
  }

  check_tone(2u);
  check_tone((N_SAMPLE / 4u) - 1u);
  check_tone((N_SAMPLE / 2u) - 2u);

  /* A constant input is a DC signal and must peak at bin zero. */
  for (index = 0u; index < N_SAMPLE; ++index) {
    samples[index].real = 200;
    samples[index].imag = 0;
  }
  FFT_Process(samples);
  measured = peak_bin(&magnitude);
  if ((measured != 0u) || (magnitude < 70u)) {
    fail("DC signal was not strongest at bin zero", measured, magnitude);
  }

  printf("FFT N=%u: DC and three bin-centred sine signals passed\n",
         (unsigned)N_SAMPLE);
  return EXIT_SUCCESS;
}
