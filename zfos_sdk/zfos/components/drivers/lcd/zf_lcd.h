#ifndef __ZF_LCD_H__
#define  __ZF_LCD_H__

#include "zf_common.h"

//画笔颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //棕色
#define BRRED 0XFC07 //棕红色
#define GRAY 0X8430  //灰色

#define FONT_USE_BACK_COLOR 0 // 字体是否使用背景色
#define FONT_BACK_COLOR WHITE //字体背景色


int dev_lcd_init(void);

struct zf_lcd_t * lcd_get_ops(uint8_t ch);
void lcd_draw_point(uint8_t ch,uint16_t x, uint16_t y, uint32_t color);
void lcd_show_string(uint8_t ch,uint16_t x, uint16_t y, uint8_t size, uint8_t *p, uint16_t color);
#endif
