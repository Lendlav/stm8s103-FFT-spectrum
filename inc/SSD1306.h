/* SPDX-License-Identifier: MIT */
#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include "main.h"

ErrorStatus LCD_Init(void);
ErrorStatus LCD_command(uint8_t command);
ErrorStatus LCD_SetWindow(uint8_t column_start, uint8_t column_end,
                          uint8_t page_start, uint8_t page_end);
/* BeginData/EndData bracket a stream; call AbortData if any byte write fails. */
ErrorStatus LCD_BeginData(void);
ErrorStatus LCD_WriteData(uint8_t data);
ErrorStatus LCD_EndData(void);
void LCD_AbortData(void);

#endif
