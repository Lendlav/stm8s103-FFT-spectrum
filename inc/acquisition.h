/* SPDX-License-Identifier: MIT */
#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <stdint.h>
#include "fft.h"

typedef enum {
  ACQUISITION_IDLE = 0,
  ACQUISITION_RUNNING,
  ACQUISITION_READY,
  ACQUISITION_OVERRUN
} AcquisitionStatus;

void Acquisition_Init(void);
void Acquisition_Start(void);
AcquisitionStatus Acquisition_GetStatus(void);
fft_complex_t *Acquisition_GetFftBuffer(void);
void Acquisition_TimerTickISR(void);
void Acquisition_AdcCompleteISR(void);
void Acquisition_DelayTicks(uint16_t ticks);

#endif
