/* SPDX-License-Identifier: MIT */
#include "fft.h"

#include "fft_tables.h"

/* Divide signed intermediate values by two while rounding toward zero. */
static int16_t half_toward_zero(int32_t value)
{
  if (value < 0) {
    return (int16_t)-(int16_t)((uint32_t)(-value) >> 1);
  }
  return (int16_t)((uint32_t)value >> 1);
}

/* Convert a Q15 product to an integer with symmetric truncation at zero. */
static int16_t q15_to_int16(int32_t value)
{
  if (value < 0) {
    return (int16_t)-(int16_t)((uint32_t)(-value) >> 15);
  }
  return (int16_t)((uint32_t)value >> 15);
}

void FFT_Process(fft_complex_t samples[N_SAMPLE])
{
  uint16_t index;
  uint16_t reverse;
  uint16_t bit;
  uint16_t span;
  uint16_t half_span;
  uint16_t twiddle_step;
  uint16_t base;
  uint16_t offset;
  uint16_t twiddle_index;
  fft_complex_t swap;
  int16_t even_real;
  int16_t even_imag;
  int16_t odd_real;
  int16_t odd_imag;
  int16_t rotated_real;
  int16_t rotated_imag;
  int32_t product_real;
  int32_t product_imag;

  /* Windowing reduces leakage from tones that do not land exactly on a bin. */
  for (index = 0u; index < N_SAMPLE; ++index) {
    samples[index].real = q15_to_int16((int32_t)samples[index].real *
                                       hann_q15[index]);
    samples[index].imag = 0;
  }

  /* Iterative bit reversal puts the real samples in radix-2 DIT order. */
  reverse = 0u;
  for (index = 1u; index < N_SAMPLE; ++index) {
    bit = N_SAMPLE >> 1;
    while ((reverse & bit) != 0u) {
      reverse ^= bit;
      bit >>= 1;
    }
    reverse ^= bit;
    if (index < reverse) {
      swap = samples[index];
      samples[index] = samples[reverse];
      samples[reverse] = swap;
    }
  }

  /* Divide each butterfly by two at every stage to bound fixed-point growth. */
  for (span = 2u; span <= N_SAMPLE; span <<= 1) {
    half_span = span >> 1;
    twiddle_step = N_SAMPLE / span;
    for (base = 0u; base < N_SAMPLE; base += span) {
      for (offset = 0u; offset < half_span; ++offset) {
        twiddle_index = offset * twiddle_step;
        even_real = samples[base + offset].real;
        even_imag = samples[base + offset].imag;
        odd_real = samples[base + offset + half_span].real;
        odd_imag = samples[base + offset + half_span].imag;

        product_real = (int32_t)twiddle_real_q15[twiddle_index] * odd_real -
                       (int32_t)twiddle_imag_q15[twiddle_index] * odd_imag;
        product_imag = (int32_t)twiddle_real_q15[twiddle_index] * odd_imag +
                       (int32_t)twiddle_imag_q15[twiddle_index] * odd_real;
        rotated_real = q15_to_int16(product_real);
        rotated_imag = q15_to_int16(product_imag);

        samples[base + offset].real = half_toward_zero(
            (int32_t)even_real + rotated_real);
        samples[base + offset].imag = half_toward_zero(
            (int32_t)even_imag + rotated_imag);
        samples[base + offset + half_span].real = half_toward_zero(
            (int32_t)even_real - rotated_real);
        samples[base + offset + half_span].imag = half_toward_zero(
            (int32_t)even_imag - rotated_imag);
      }
    }
  }
}

uint16_t FFT_Magnitude(const fft_complex_t *value)
{
  uint32_t real = value->real;
  uint32_t imag = value->imag;
  uint32_t remainder;
  uint32_t root;
  uint32_t bit;

  if (value->real < 0) {
    real = (uint32_t)(-(int32_t)value->real);
  }
  if (value->imag < 0) {
    imag = (uint32_t)(-(int32_t)value->imag);
  }
  remainder = real * real + imag * imag;

  /* Restoring integer square root; no floating-point library is linked. */
  root = 0u;
  bit = 1UL << 30;
  while (bit > remainder) {
    bit >>= 2;
  }
  while (bit != 0u) {
    if (remainder >= root + bit) {
      remainder -= root + bit;
      root = (root >> 1) + bit;
    } else {
      root >>= 1;
    }
    bit >>= 2;
  }
  return (uint16_t)root;
}
