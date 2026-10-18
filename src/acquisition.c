/* SPDX-License-Identifier: MIT */
#include "main.h"
#include "app_config.h"
#include "acquisition.h"

/* Static storage leaves the small STM8 stack available for ISR call frames. */
static fft_complex_t fft_buffer[N_SAMPLE];
static volatile uint8_t capture_state;
static volatile uint8_t adc_busy;
static volatile uint8_t timer_tick;
static uint16_t sample_index;

void Acquisition_Init(void)
{
  capture_state = ACQUISITION_IDLE;
  adc_busy = 0u;
  timer_tick = 0u;
  sample_index = 0u;
}

void Acquisition_Start(void)
{
  uint16_t timeout = ADC_BUSY_TIMEOUT;

  /* If a previous conversion did not finish, reset the ADC before recapturing. */
  while ((adc_busy != 0u) && (timeout != 0u)) {
    --timeout;
  }
  if (adc_busy != 0u) {
    ADC_Reinitialize();
    adc_busy = 0u;
  }

  sample_index = 0u;
  capture_state = ACQUISITION_RUNNING;
}

AcquisitionStatus Acquisition_GetStatus(void)
{
  return (AcquisitionStatus)capture_state;
}

fft_complex_t *Acquisition_GetFftBuffer(void)
{
  return fft_buffer;
}

void Acquisition_TimerTickISR(void)
{
  ++timer_tick;
  if (capture_state != ACQUISITION_RUNNING) {
    return;
  }

  if (adc_busy != 0u) {
    /* Missing a sample changes the FFT timing, so discard the whole frame. */
    capture_state = ACQUISITION_OVERRUN;
    return;
  }

  adc_busy = 1u;
  ADC_StartConversion();
}

void Acquisition_AdcCompleteISR(void)
{
  uint16_t raw = ADC_ReadConversionValue();

  ADC_ClearEndOfConversion();
  adc_busy = 0u;

  if (capture_state != ACQUISITION_RUNNING) {
    return;
  }

  /* Keep samples in time order; FFT_Process applies the window and permutation. */
  fft_buffer[sample_index].real = (int16_t)raw - ADC_CHANNEL_MIDPOINT;
  fft_buffer[sample_index].imag = 0;
  ++sample_index;
  if (sample_index >= N_SAMPLE) {
    capture_state = ACQUISITION_READY;
  }
}

void Acquisition_DelayTicks(uint16_t ticks)
{
  uint8_t previous;

  while (ticks != 0u) {
    previous = timer_tick;
    while (timer_tick == previous) {
      /* TIM4 advances the wrapping counter at the configured sample rate. */
    }
    --ticks;
  }
}
