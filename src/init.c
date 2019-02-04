//*****************************************************************************
//
//*****************************************************************************
#include "main.h"

//---------------------------------------------
#define TIM4_PERIOD       155
//---------------------------------------------

//---------------------------------------------
void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_16, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}
//---------------------------------------------
void TIM1_Config(void)
{
  TIM1_DeInit();
  /* Time base configuration */
  /*
  TIM1_Period = INIT_TIM1_ARR
  TIM1_Prescaler = 0
  TIM1_RepetitionCounter=0
  */
  TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 2000, 0);
  
  /* PWM configuration */
  /*
  TIM1_OCMode      = TIM1_OCMODE_PWM1
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_Pulse       = INIT_TIM1_CCR1
  TIM1_OCPolarity  = TIM1_OCPOLARITY_LOW
  */
  TIM1_OC1Init(TIM1_OCMODE_PWM1, 
               TIM1_OUTPUTSTATE_ENABLE, 
               TIM1_OUTPUTNSTATE_DISABLE,
               1000, 
               TIM1_OCPOLARITY_LOW, 
               TIM1_OCNPOLARITY_LOW, 
               TIM1_OCIDLESTATE_RESET, 
               TIM1_OCNIDLESTATE_RESET);
  
//  /* Update Interrupt Enable */
//  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
//  
  /* ARRPreload Enable */
  TIM1_ARRPreloadConfig(ENABLE);
  
  /* Enable TIM1 */
  TIM1_Cmd(ENABLE);
  
  /* Enable CC1 output*/
  TIM1_CtrlPWMOutputs(ENABLE);
}
//---------------------------------------------
void Init_CLK(void)
{
   CLK_DeInit();
   CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);  // 1..8
   CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1); // 1..128   
   CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
   
    //   CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_LSI, DISABLE, CLK_CURRENTCloCKSTATE_DISABLE); 
    //   CLK_CCOConfig(CLK_OUTPUT_CPU);   
    //   CLK_CCOCmd(ENABLE);  
}
//---------------------------------------------

void Init_EXTI(void)
{
   //   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_ONLY);
   //   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
   //   EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_RISE_ONLY);
}

//---------------------------------------------
void Init_ITC(void)
{ 
  //  ITC_SetSoftwarePriority(ITC_IRQ_PORTA, ITC_PRIORITYLEVEL_1);
  //   ITC_SetSoftwarePriority(ITC_IRQ_AWU, ITC_PRIORITYLEVEL_1);
  //   ITC_SetSoftwarePriority(ITC_IRQ_PORTC, ITC_PRIORITYLEVEL_2);
  //   ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_1);
  //   ITC_SetSoftwarePriority(ITC_IRQ_TLI, ITC_PRIORITYLEVEL_1);
  
   enableInterrupts();
}
//---------------------------------------------
void Init_GPIO(void)
{  
   GPIO_DeInit(GPIOA);
   GPIO_DeInit(GPIOB);
   GPIO_DeInit(GPIOC);
   GPIO_DeInit(GPIOD);

   GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);
}
//---------------------------------------------
void Init_ADC(u8 channel)
{ 
   ADC1_DeInit();
   ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
             channel, 
             ADC1_PRESSEL_FCPU_D12, 
             ADC1_EXTTRIG_TIM, DISABLE, 
             ADC1_ALIGN_RIGHT, 
             ADC1_SCHMITTTRIG_CHANNEL6,DISABLE
                );
   ADC1_Cmd(ENABLE);
}
//---------------------------------------------
#define OWN_ADDRESS     0x70
void Init_I2C(void)
{   
   I2C_DeInit();
   I2C_Init(
            ((uint32_t)750000), //I2C_MAX_FAST_FREQ, 
            OWN_ADDRESS, 
            I2C_DUTYCYCLE_2, 
            I2C_ACK_CURR, 
            I2C_ADDMODE_7BIT, 
            16 
               );  
}
//---------------------------------------------

