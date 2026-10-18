/* SPDX-License-Identifier: MIT */
#include "main.h"
#include "app_config.h"
#include "stm8_regs.h"
#include "SSD1306.h"

#define LCD_DATA_MODE       0x40u
#define LCD_COMMAND_MODE    0x00u

static uint8_t data_transfer_active;

static ErrorStatus wait_i2c_status(uint8_t mask)
{
  uint16_t timeout = I2C_EVENT_TIMEOUT;

  while (timeout != 0u) {
    if ((I2C_SR2 & I2C_SR2_ERROR) != 0u) {
      return ERROR;
    }
    if ((I2C_SR1 & mask) == mask) {
      return SUCCESS;
    }
    --timeout;
  }
  return ERROR;
}

static void recover_i2c(void)
{
  data_transfer_active = 0u;
  I2C_CR2 |= I2C_CR2_STOP;
  I2C_CR1 = 0u;
  I2C_CR2 = 0u;
  I2C_SR2 = 0u; /* The I2C error flags are cleared by writing zero. */
  Init_I2C();
}

static ErrorStatus begin_transaction(uint8_t control)
{
  volatile uint8_t status_read;

  I2C_CR2 |= I2C_CR2_START;
  if (wait_i2c_status(I2C_SR1_SB) != SUCCESS) {
    return ERROR;
  }

  I2C_DR = SSD1306_I2C_ADDRESS; /* Write direction is zero in the address byte. */
  if (wait_i2c_status(I2C_SR1_ADDR) != SUCCESS) {
    return ERROR;
  }

  /* Reading SR1 then SR3 acknowledges ADDR and selects master transmitter mode. */
  status_read = I2C_SR1;
  status_read = I2C_SR3;
  (void)status_read;

  I2C_DR = control;
  return wait_i2c_status(I2C_SR1_TXE);
}

static ErrorStatus write_i2c_byte(uint8_t value)
{
  if (wait_i2c_status(I2C_SR1_TXE) != SUCCESS) {
    return ERROR;
  }
  I2C_DR = value;
  return SUCCESS;
}

ErrorStatus LCD_command(uint8_t command)
{
  if (begin_transaction(LCD_COMMAND_MODE) != SUCCESS ||
      write_i2c_byte(command) != SUCCESS ||
      wait_i2c_status(I2C_SR1_BTF) != SUCCESS) {
    recover_i2c();
    return ERROR;
  }

  I2C_CR2 |= I2C_CR2_STOP;
  return SUCCESS;
}

ErrorStatus LCD_Init(void)
{
  static const uint8_t commands[] = {
    /* Display off, clock, 32-row multiplex, zero offset and start line. */
    0xAEu, 0xD5u, 0x80u, 0xA8u, 0x1Fu, 0xD3u, 0x00u, 0x40u,
    /* Charge pump, vertical addressing, remap columns, scan direction, pins. */
    0x8Du, 0x14u, 0x20u, 0x01u, 0xA1u, 0xC8u, 0xDAu, 0x02u,
    /* Contrast, precharge, VCOM, RAM display, normal polarity and display on. */
    0x81u, 0x0Au, 0xD9u, 0xF1u, 0xDBu, 0x40u, 0xA4u, 0xA6u, 0xAFu
  };
  uint8_t index;

  for (index = 0u; index < (uint8_t)sizeof(commands); ++index) {
    if (LCD_command(commands[index]) != SUCCESS) {
      return ERROR;
    }
  }
  return SUCCESS;
}

ErrorStatus LCD_SetWindow(uint8_t column_start, uint8_t column_end,
                          uint8_t page_start, uint8_t page_end)
{
  if ((column_start > column_end) ||
      (column_end >= SSD1306_COLUMN_COUNT) ||
      (page_start > page_end) ||
      (page_end >= SSD1306_PAGE_COUNT)) {
    return ERROR;
  }

  /* SSD1306 command parameters are sent as ordinary one-byte commands. */
  if (LCD_command(0x21u) != SUCCESS ||
      LCD_command(column_start) != SUCCESS ||
      LCD_command(column_end) != SUCCESS ||
      LCD_command(0x22u) != SUCCESS ||
      LCD_command(page_start) != SUCCESS ||
      LCD_command(page_end) != SUCCESS) {
    return ERROR;
  }
  return SUCCESS;
}

ErrorStatus LCD_BeginData(void)
{
  if ((data_transfer_active != 0u) ||
      (begin_transaction(LCD_DATA_MODE) != SUCCESS)) {
    recover_i2c();
    return ERROR;
  }
  data_transfer_active = 1u;
  return SUCCESS;
}

ErrorStatus LCD_WriteData(uint8_t data)
{
  if ((data_transfer_active == 0u) ||
      (write_i2c_byte(data) != SUCCESS) ||
      (wait_i2c_status(I2C_SR1_TXE) != SUCCESS)) {
    recover_i2c();
    return ERROR;
  }
  return SUCCESS;
}

ErrorStatus LCD_EndData(void)
{
  if ((data_transfer_active == 0u) ||
      (wait_i2c_status(I2C_SR1_BTF) != SUCCESS)) {
    recover_i2c();
    return ERROR;
  }
  I2C_CR2 |= I2C_CR2_STOP;
  data_transfer_active = 0u;
  return SUCCESS;
}

void LCD_AbortData(void)
{
  if (data_transfer_active != 0u) {
    recover_i2c();
  }
}
