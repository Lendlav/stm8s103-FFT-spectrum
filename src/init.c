/* SPDX-License-Identifier: MIT */
#include "main.h"
#include "app_config.h"
#include "stm8_regs.h"

static uint8_t adc_channel = 6u;

void Init_CLK(void)
{
  /* Select the reset HSI source without division: fCPU = fMASTER = 16 MHz. */
  CLK_CKDIVR = 0x00u;

  /* The reset state enables all peripheral clocks; explicitly keep used ones on. */
  CLK_PCKENR1 |= 0x10u; /* PCKEN14: TIM4 on STM8S103. */
  CLK_PCKENR1 |= 0x01u; /* PCKEN10: I2C. */
  CLK_PCKENR2 |= 0x08u; /* PCKEN23: ADC. */
}

void Init_GPIO(void)
{
  /* I2C is open-drain; external pull-ups on PB4/PB5 are required by the bus. */
  GPIOB_ODR &= (uint8_t)~GPIO_I2C_PINS;
  GPIOB_CR1 &= (uint8_t)~GPIO_I2C_PINS;
  GPIOB_CR2 |= GPIO_I2C_PINS;
  GPIOB_DDR |= GPIO_I2C_PINS;
}

void Init_I2C(void)
{
  /* Configure fast-mode I2C at 16 MHz: 16 MHz / (3 * 14) = 380.95 kHz. */
  I2C_CR1 = 0u;
  I2C_CR2 = 0u;
  I2C_SR2 = 0u;
  I2C_FREQR = 16u;
  I2C_CCRH = 0x80u; /* Fast mode, duty ratio 2:1, high CCR bits are zero. */
  I2C_CCRL = 14u;
  I2C_TRISER = 6u;  /* Fast-mode maximum rise time at a 16 MHz peripheral clock. */
  I2C_CR1 = I2C_CR1_PE;
}

void Init_ADC(uint8_t channel)
{
  adc_channel = channel & ADC_CSR_CHANNEL;

  /* Power down before changing the ADC prescaler or channel configuration. */
  ADC1_CR1 = 0u;
  ADC1_CR2 = 0u; /* Single conversion, right alignment, no external trigger. */
  ADC1_CR3 = 0u;
  ADC1_CSR = (uint8_t)(ADC_CSR_EOCIE | adc_channel);

  /* AIN6 is in TDRL bit 6; disabling its Schmitt trigger selects analog input. */
  ADC1_TDRL |= (uint8_t)(1u << adc_channel);
  ADC1_CR1 = ADC_CR1_PRESCALE_12;
  ADC1_CR1 |= ADC_CR1_ADON; /* First ADON write wakes the converter. */
}

void ADC_Reinitialize(void)
{
  /* Recover from a wedged conversion without relying on a vendor driver. */
  Init_ADC(adc_channel);
}

void ADC_StartConversion(void)
{
  /* EOC is cleared by writing zero; the second ADON=1 write starts a sample. */
  ADC1_CSR = (uint8_t)(ADC1_CSR & (uint8_t)~ADC_CSR_EOC);
  ADC1_CR1 |= ADC_CR1_ADON;
}

uint16_t ADC_ReadConversionValue(void)
{
  uint8_t low;
  uint8_t high;

  /* Right-aligned ADC data must be read LSB first to keep the pair coherent. */
  low = ADC1_DRL;
  high = ADC1_DRH;
  return (uint16_t)(((uint16_t)high << 8) | low);
}

void ADC_ClearEndOfConversion(void)
{
  ADC1_CSR = (uint8_t)(ADC1_CSR & (uint8_t)~ADC_CSR_EOC);
}

void TIM4_Config(void)
{
  TIM4_CR1 = 0u;
  TIM4_IER = 0u;
  TIM4_SR = 0u;
  TIM4_CNTR = 0u;
  TIM4_PSCR = SAMPLE_TIMER_PRESCALER;
  TIM4_ARR = SAMPLE_TIMER_RELOAD;
  TIM4_EGR = 0x01u; /* Load the prescaler and auto-reload value immediately. */
  TIM4_SR = 0u;     /* Clear the update flag caused by the software update. */
  TIM4_IER = TIM4_IER_UIE;
  TIM4_CR1 = TIM4_CR1_CEN;
}

void TIM4_ClearUpdateFlag(void)
{
  /* TIM4 has a single status flag, so writing zero clears the update event. */
  TIM4_SR = 0u;
}

void CPU_EnableInterrupts(void)
{
  __asm
    rim
  __endasm;
}
