//*****************************************************************************
//
//*****************************************************************************

#include "stm8s.h"

//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
typedef union {
    struct {
        u8 LB;
        u8 HB;
    };
    u16 all;
} Word;

typedef union {
   struct {
      u8 LB;
      u8 MB;
      u8 HB;
      u8 HHB;
   };
   u32 all;
}Line;

extern __IO u32 count;

//----------------------------------------------------------------------------------

void Init_CLK   (void);
void Init_GPIO  (void);
void Init_ADC   (u8 channel);
void Init_I2C   (void);
void Init_ITC   (void);
void Init_EXTI  (void);
void TIM4_Config(void);
void TIM1_Config(void);
void Init_AWU   (void);
void Init_SWIM  (void);

void delay      (u32 nTime);

//----------------------------------------------------------------------------------



