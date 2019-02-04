//******************************************************************************
//
//  simple spectrum whit using stm8s103 and 128x32 OLED I2C
//
//******************************************************************************

#include "main.h"
#include "fft__.h"
#include "SSD1306.h"
//----------------------------------------------------------------------------------
typedef union {
  struct {
    u8 HHB;
    u8 HB;
    u8 MB;
    u8 LB;
  };  
  u32 all;
  u8 arr[4];
} line;
//------------------------------------------------------------------------------

__IO u32 count;         // delay counter
__IO u8  isr_flag = 0;  // sample flag
line my_line;           // screen line

int16_t    capture    [N_SAMPLE]; /* Wave captureing buffer */
complex_t  bfly_buff  [N_SAMPLE]; /* FFT buffer */
 
//------------------------------------------------------------------------------
void capture_wave (uint16_t count);  
void convert(int value);
//------------------------------------------------------------------------------

void main( void )
{
   Init_CLK();
   Init_GPIO();
   Init_I2C();
   Init_ADC(ADC1_CHANNEL_6);
   TIM4_Config();
   TIM1_Config();
   Init_ITC();
   delay(500);  //dalay must be here before the lcd init call
   LCD_Init();
   
   uint16_t m, n, s;

   while(1)
   {
     capture_wave(N_SAMPLE);            //collect and prepare samples
     fix_fft(capture, bfly_buff);       //transform samples                  
     fft_out(bfly_buff, capture);       //prepare spectrum values 

     // ********show spectrum values********      
     disableInterrupts();
        
#define DATA_MODE     0x40
#define COMMAND_MODE  0x00   // bit Co = 0, D/C# = 0
#define column_start  0
#define column_end    127
#define page_start    0
#define page_end      3
     
     LCD_command(0x21);                 // SSD1306_COLUMNADDR
     LCD_command(column_start);         // column start
     LCD_command(column_end);           // column end
     LCD_command(0x22);                 // SSD1306_PAGEADDR
     LCD_command(page_start);           // page start
     LCD_command(page_end);             // page end (4 pages for 32 rows OLED)
   
     I2C_GenerateSTART(ENABLE);
     while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
     I2C_Send7bitAddress(SLAVE_ADDRESS, I2C_DIRECTION_TX);
     while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
     I2C_SendData(DATA_MODE);
     while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 
     
     for (n = 0; n < N_SAMPLE / 2; n++) 
     {
       s  = capture[n];
       s /= 100;
       convert(s);
       for (u8 mm = 0; mm < (256/N_SAMPLE); mm++)
       {
         if(mm > ((256/N_SAMPLE)/3) )
         {
            my_line.all = 0;  //skeep some line
         }
         else
         {
           if((n%1)==0)my_line.LB |= 0x01;
           if((n%5)==0)my_line.LB |= 0x05;  //add the dots above spectrum
           if((n%10)==0)my_line.LB |= 0x15;
         }
         for (m = 0; m < 4; m++)
         {
           I2C->DR = my_line.arr[3-m];
           while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));          
         }
       }
     }
     
     I2C_GenerateSTOP(ENABLE);      
     enableInterrupts();
   }   
}
/*------------------------------------------------*/
//  convert int to line
/*------------------------------------------------*/
void convert(int value)
{ 
  my_line.all &= 0x80000000; 
  u32 mask     = 0x80000000;
  
  while(value>0)
  {
    mask >>= 1;
    mask  |= 0x80000000;
    value -= 1;     
  }  
  my_line.all |=mask; 
}
/*------------------------------------------------*/
/*                  Capture waveform              */
/*------------------------------------------------*/
void capture_wave (uint16_t cnt)
{ 
  u8 i     = 0;
  u16 adc  = 0;
  isr_flag = RESET;

  do 
  {
    if(isr_flag)
    {
      isr_flag = RESET;     
 
      ADC1->CR1 |= ADC1_CR1_ADON;  
      while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET); 
      adc = ADC1_GetConversionValue() - 511;
      
      //prepare a complex value
      bfly_buff[position[i]].real = FIX(adc, ham[i]);
      bfly_buff[i].image = 0;

      i++;
      cnt--;
    }
  } while(cnt); 
}
//---------------------------------------------
void delay(u32 nTime)
{         
  count = nTime;
  while(count)
  {
    continue;
  }
}
