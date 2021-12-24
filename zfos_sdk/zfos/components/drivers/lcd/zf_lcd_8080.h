#ifndef __ZF_LCD_8080_H__
#define __ZF_LCD_8080_H__

#include "zf_common.h"

int drv_lcd_8080_init(void);
void lcd_8080_write_reg(uint16_t value);
void lcd_8080_write_data(uint16_t value);
void lcd_8080_write_color(uint16_t value);
#endif
