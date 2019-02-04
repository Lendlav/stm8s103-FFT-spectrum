//*****************************************************************************
//              SSD1306 library for STM8S
//*****************************************************************************


//----------------------------------------------------------------------------------
#define SLAVE_ADDRESS   0x78
//----------------------------------------------------------------------------------

extern unsigned char            lcd_buff[];
extern const unsigned char      font [][5];

//----------------------------------------------------------------------------------
void LCD_Init   (void);
void LCD_Off    (void);
void LCD_On     (void);
void LCD_Clear  (void);
void LCD_Update (void);
void LCD_Chr    (u8 ch);
void LCD_2xChr  (u8 ch);
void LCD_FStr   (const u8 *String);
void LCD_2xFStr (const u8 *String);
void LCD_GotoXY (u8 x,u8 y);
void LCD_Picture(const u8 *Picture);
void LCD_Logo   (const u8 *picture, u16 picture_size, u8 Inversion);
void LCD_command(u8 cmd);

//----------------------------------------------------------------------------------
