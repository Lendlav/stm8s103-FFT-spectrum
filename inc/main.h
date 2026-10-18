/* SPDX-License-Identifier: MIT */
#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

/* A small status type used by the OLED routines and hardware setup. */
typedef enum {
  ERROR = 0,
  SUCCESS = 1
} ErrorStatus;

void Init_CLK(void);
void Init_GPIO(void);
void Init_ADC(uint8_t channel);
void ADC_Reinitialize(void);
void Init_I2C(void);
void TIM4_Config(void);

uint16_t ADC_ReadConversionValue(void);
void ADC_ClearEndOfConversion(void);
void ADC_StartConversion(void);
void TIM4_ClearUpdateFlag(void);
void CPU_EnableInterrupts(void);

#endif
