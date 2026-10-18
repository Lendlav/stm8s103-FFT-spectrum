/* SPDX-License-Identifier: MIT */
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/* SDCC needs ISR prototypes visible in the translation unit containing main. */
void TRAP_IRQHandler(void) __trap;
void ADC1_IRQHandler(void) __interrupt(22);
void TIM4_UPD_OVF_IRQHandler(void) __interrupt(23);
void FLASH_IRQHandler(void) __interrupt(24);
void Reserved_IRQHandler(void) __interrupt(29);

#endif
