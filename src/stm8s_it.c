/**
******************************************************************************
* @file    stm8s_it.c
* @author  MCD Application Team
* @version V2.2.0
* @date    30-September-2014
* @brief   Main Interrupt Service Routines.
*          This file provides template for all peripherals interrupt service 
*          routine.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"

/** @addtogroup Template_Project
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO u32 count;
extern __IO u8  isr_flag;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
* @brief Dummy Interrupt routine
* @par Parameters:
* None
* @retval
* None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}
#endif /*_COSMIC_*/

/**
* @brief TRAP Interrupt routine
* @param  None
* @retval None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief Top Level Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief Auto Wake Up Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief Clock Controller Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief External Interrupt PORTA Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
   
}

/**
* @brief External Interrupt PORTB Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief External Interrupt PORTC Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief External Interrupt PORTD Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief External Interrupt PORTE Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}


/**
* @brief SPI Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief Timer1 Capture/Compare Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
/**
* @brief Timer2 Update/Overflow/Break Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief Timer2 Capture/Compare Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
   
}



#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
* @brief UART1 TX Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/**
* @brief UART1 RX Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}
#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

/**
* @brief I2C Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}


/* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */
/**
* @brief ADC1 interrupt routine.
* @par Parameters:
* None
* @retval 
* None
*/
INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

/* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
* @brief Timer4 Update/Overflow Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{   
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
   if(count)
   {
      count--;
   }
    
   isr_flag = SET;
   
    TIM4->SR1 = (uint8_t)(~TIM4_IT_UPDATE);//TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}

/**
* @brief Eeprom EEC Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
   /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
   */
}

