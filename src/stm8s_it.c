/* SPDX-License-Identifier: MIT */
#include "acquisition.h"
#include "interrupts.h"
#include "main.h"

/* SDCC emits these handlers into the STM8 vector table at IRQ 22 and 23. */
void TRAP_IRQHandler(void) __trap
{
  /* An unexpected CPU trap stops execution where a debugger can inspect it. */
  while (1) {
  }
}

void ADC1_IRQHandler(void) __interrupt(22)
{
  Acquisition_AdcCompleteISR();
}

void TIM4_UPD_OVF_IRQHandler(void) __interrupt(23)
{
  TIM4_ClearUpdateFlag();
  Acquisition_TimerTickISR();
}

/* Keep the device's FLASH vector occupied and fail safely if it ever fires. */
void FLASH_IRQHandler(void) __interrupt(24)
{
  while (1) {
  }
}

/* Extend SDCC's table through IRQ 29 so code cannot overlap reserved vectors. */
void Reserved_IRQHandler(void) __interrupt(29)
{
  while (1) {
  }
}
