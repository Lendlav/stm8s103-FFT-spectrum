/* SPDX-License-Identifier: MIT */
#ifndef STM8_REGS_H
#define STM8_REGS_H

#include <stdint.h>

/*
 * Only the registers used by this application are named here. The addresses
 * and bit fields come from ST reference manual RM0016 and the STM8S103F3
 * datasheet; this is an independently written register map, not an SPL copy.
 */
#define REG8(address) (*(volatile uint8_t *)(address))

/* Clock control. */
#define CLK_CKDIVR       REG8(0x50C6u)
#define CLK_PCKENR1      REG8(0x50C7u)
#define CLK_PCKENR2      REG8(0x50CAu)

/* GPIOB: ODR, IDR, DDR, CR1, CR2 are consecutive registers. */
#define GPIOB_ODR        REG8(0x5005u)
#define GPIOB_DDR        REG8(0x5007u)
#define GPIOB_CR1        REG8(0x5008u)
#define GPIOB_CR2        REG8(0x5009u)

/* I2C master registers. */
#define I2C_CR1          REG8(0x5210u)
#define I2C_CR2          REG8(0x5211u)
#define I2C_FREQR        REG8(0x5212u)
#define I2C_OARL         REG8(0x5213u)
#define I2C_OARH         REG8(0x5214u)
#define I2C_DR           REG8(0x5216u)
#define I2C_SR1          REG8(0x5217u)
#define I2C_SR2          REG8(0x5218u)
#define I2C_SR3          REG8(0x5219u)
#define I2C_CCRL         REG8(0x521Bu)
#define I2C_CCRH         REG8(0x521Cu)
#define I2C_TRISER       REG8(0x521Du)

/* TIM4, the 8-bit periodic sampling timer. */
#define TIM4_CR1         REG8(0x5340u)
#define TIM4_IER         REG8(0x5343u)
#define TIM4_SR          REG8(0x5344u)
#define TIM4_EGR         REG8(0x5345u)
#define TIM4_CNTR        REG8(0x5346u)
#define TIM4_PSCR        REG8(0x5347u)
#define TIM4_ARR         REG8(0x5348u)

/* ADC1 control and data registers. Right-aligned results are read DRL first. */
#define ADC1_CSR         REG8(0x5400u)
#define ADC1_CR1         REG8(0x5401u)
#define ADC1_CR2         REG8(0x5402u)
#define ADC1_CR3         REG8(0x5403u)
#define ADC1_DRH         REG8(0x5404u)
#define ADC1_DRL         REG8(0x5405u)
#define ADC1_TDRL        REG8(0x5407u)

#define GPIO_I2C_PINS    0x30u  /* PB4=SCL and PB5=SDA on STM8S103F3. */

#define I2C_CR1_PE       0x01u
#define I2C_CR2_START    0x01u
#define I2C_CR2_STOP     0x02u
#define I2C_SR1_SB       0x01u
#define I2C_SR1_ADDR     0x02u
#define I2C_SR1_BTF      0x04u
#define I2C_SR1_TXE      0x80u
#define I2C_SR2_ERROR    0x0Fu

#define ADC_CSR_EOC      0x80u
#define ADC_CSR_EOCIE    0x20u
#define ADC_CSR_CHANNEL  0x0Fu
#define ADC_CR1_ADON     0x01u
#define ADC_CR1_PRESCALE_12 0x60u
#define TIM4_SR_UIF      0x01u
#define TIM4_IER_UIE     0x01u
#define TIM4_CR1_CEN     0x01u

#endif
