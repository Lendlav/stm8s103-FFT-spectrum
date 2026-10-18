/* SPDX-License-Identifier: MIT */
#include "main.h"
#include "app_config.h"
#include "acquisition.h"
#include "fft.h"
#include "interrupts.h"
#include "SSD1306.h"

/* Magnitudes and sample buffers stay static to protect the 1 KiB RAM budget. */
static uint16_t magnitudes[N_SAMPLE / 2u];

/* Half-open bin ranges make each band a compact interval and omit DC. */
#if N_SAMPLE == 32u
#define DISPLAY_BAR_COUNT 8u
static const uint8_t display_bin_edges[DISPLAY_BAR_COUNT + 1u] = {
  1u, 2u, 3u, 4u, 6u, 8u, 10u, 13u, 16u
};
#elif N_SAMPLE == 64u
#define DISPLAY_BAR_COUNT 16u
static const uint8_t display_bin_edges[DISPLAY_BAR_COUNT + 1u] = {
  1u, 2u, 3u, 4u, 5u, 6u, 8u, 10u, 12u,
  14u, 16u, 18u, 20u, 23u, 26u, 29u, 32u
};
#else
#define DISPLAY_BAR_COUNT 24u
static const uint8_t display_bin_edges[DISPLAY_BAR_COUNT + 1u] = {
  1u, 2u, 3u, 4u, 5u, 6u, 7u, 9u, 11u, 13u,
  15u, 17u, 20u, 23u, 26u, 29u, 32u, 36u, 40u,
  44u, 48u, 52u, 56u, 60u, 64u
};
#endif

static uint32_t make_bar(uint16_t height)
{
  if (height >= 31u) {
    return 0xFFFFFFFFUL;
  }
  return (1UL << (height + 1u)) - 1u;
}

static uint16_t get_band_peak(uint8_t band)
{
  uint8_t bin;
  uint16_t peak = 0u;

  for (bin = display_bin_edges[band]; bin < display_bin_edges[band + 1u]; ++bin) {
    if (magnitudes[bin] > peak) {
      peak = magnitudes[bin];
    }
  }
  return peak;
}

static ErrorStatus draw_spectrum(void)
{
  uint16_t band;
  uint16_t page;
  uint16_t x;
  uint16_t x_start;
  uint16_t x_end;
  uint16_t height;
  uint16_t peak;
  uint32_t level;
  uint32_t column;

  if (LCD_SetWindow(0u, SSD1306_COLUMN_COUNT - 1u,
                    0u, SSD1306_PAGE_COUNT - 1u) != SUCCESS ||
      LCD_BeginData() != SUCCESS) {
    return ERROR;
  }

  /* Magnitudes are normalized by the FFT stages, so no extra N division is needed. */
  for (band = 0u; band < DISPLAY_BAR_COUNT; ++band) {
    peak = get_band_peak((uint8_t)band);
    level = peak;
    height = (uint16_t)((level * 32u) / (level + DISPLAY_LEVEL_REFERENCE));
    if (height > 31u) {
      height = 31u;
    }

    x_start = (uint16_t)(((uint32_t)band * SSD1306_COLUMN_COUNT) /
                         DISPLAY_BAR_COUNT);
    x_end = (uint16_t)(((uint32_t)(band + 1u) * SSD1306_COLUMN_COUNT) /
                       DISPLAY_BAR_COUNT);
    for (x = x_start; x < x_end; ++x) {
      column = (x + 1u == x_end) ? 0u : make_bar(height);
      /* Vertical addressing advances one 8-pixel page at a time. */
      for (page = 0u; page < SSD1306_PAGE_COUNT; ++page) {
        if (LCD_WriteData((uint8_t)(column >> (page * 8u))) != SUCCESS) {
          LCD_AbortData();
          return ERROR;
        }
      }
    }
  }
  return LCD_EndData();
}

void main(void)
{
  fft_complex_t *samples;
  uint16_t bin;

  Init_CLK();
  Init_GPIO();
  Init_I2C();
  Init_ADC(6u);
  TIM4_Config();
  Acquisition_Init();
  CPU_EnableInterrupts();

  /* Let the OLED supply and bus settle before its first command sequence. */
  Acquisition_DelayTicks(500u);
  (void)LCD_Init();
  samples = Acquisition_GetFftBuffer();

  while (1) {
    Acquisition_Start();
    while (Acquisition_GetStatus() == ACQUISITION_RUNNING) {
      /* TIM4 spaces samples and ADC1 completes them from interrupt handlers. */
    }
    if (Acquisition_GetStatus() != ACQUISITION_READY) {
      continue; /* A missed sample invalidates the time spacing for this frame. */
    }

    FFT_Process(samples);
    for (bin = 0u; bin < N_SAMPLE / 2u; ++bin) {
      magnitudes[bin] = FFT_Magnitude(&samples[bin]);
    }
    (void)draw_spectrum();
  }
}
