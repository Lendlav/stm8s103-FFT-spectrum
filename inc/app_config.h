/* SPDX-License-Identifier: MIT */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Hardware and signal-processing settings for STM8S103F3. */
/* Supported FFT lengths have generated Hann-window and twiddle tables. */
#ifndef N_SAMPLE
#define N_SAMPLE                    64u
#endif

/* Must match the clock setup in Init_CLK(). */
#define CPU_CLOCK_HZ                16000000UL
#define SAMPLE_TIMER_PRESCALER      4u /* TIM4 divides by 2^PSCR: 2^4 = 16. */
#define SAMPLE_TIMER_DIVIDER        16UL
#define SAMPLE_TIMER_RELOAD         155u
/* TIM4 update rate: 16 MHz / 16 / (155 + 1) = about 6.41 ksample/s. */
#define SAMPLE_RATE_HZ              \
  (CPU_CLOCK_HZ / (SAMPLE_TIMER_DIVIDER * (SAMPLE_TIMER_RELOAD + 1UL)))

/* Center a 10-bit ADC reading around zero before applying the FFT window. */
#define ADC_CHANNEL_MIDPOINT        512
/* Normalized FFT level at which a bar reaches roughly half its height. */
#define DISPLAY_LEVEL_REFERENCE     32u
/* These are loop limits, not elapsed time values. */
#define I2C_EVENT_TIMEOUT           60000u
#define ADC_BUSY_TIMEOUT            60000u
/* The 7-bit SSD1306 address is shifted when written to the STM8 I2C data reg. */
#define SSD1306_I2C_ADDRESS          0x78u
#define SSD1306_COLUMN_COUNT         128u
#define SSD1306_PAGE_COUNT           4u

#if (N_SAMPLE != 32u) && (N_SAMPLE != 64u) && (N_SAMPLE != 128u)
#error N_SAMPLE must be 32, 64, or 128
#endif

#endif
