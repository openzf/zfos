#ifndef __ZF_LCD_DEFINE_H__
#define __ZF_LCD_DEFINE_H__

#include "zf_common.h"


struct lcd_rbg_config_t
{
	uint8_t hsw;
	uint8_t hfp;
	uint8_t hbp;
	uint8_t vsw;
	uint8_t vbp;
	uint8_t vfp;
};

struct lcd_8080_config_t
{
    uint16_t w_ram_cmd;		//开始写gram指令
	uint16_t set_x_cmd;		//设置x坐标指令
	uint16_t set_y_cmd;		//设置y坐标指令 
};

 struct zf_lcd_config_t
{
    uint8_t type; // rgb mcu oled spi
	uint16_t id;
	uint8_t dir;
	uint16_t pix_width;
	uint16_t pix_height;
    struct lcd_rbg_config_t rgb_config;
	struct lcd_8080_config_t  m8080_config;
};


typedef enum
{
    LCD_DIR_HORIZONTAL = 0,
    LCD_DIR_VERTICAL
} LCD_DIR_ENUM;

#define LCD_TYPE_RGB 0
#define LCD_TYPE_8080 1
#define LCD_TYPE_OLED 2
#define LCD_TYPE_SPI 3

#endif
