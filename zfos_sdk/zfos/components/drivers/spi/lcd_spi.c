#include "../zf_device.h"
#include "../zf_driver.h"
#include "zf_lcd_define.h"

#include "zf_dts.h"
#include "lcd_spi.h"

#include "zf_sspi.h"

static struct zf_gpio_t *g_gpio;
static struct zf_spi_t *g_spi;

#define LCD_SPI_RES_L() g_gpio->write(LCD_SPI_RES_PORT, LCD_SPI_RES_PIN, 0)
#define LCD_SPI_RES_H() g_gpio->write(LCD_SPI_RES_PORT, LCD_SPI_RES_PIN, 1)

#define LCD_SPI_DC_L() g_gpio->write(LCD_SPI_DC_PORT, LCD_SPI_DC_PIN, 0)
#define LCD_SPI_DC_H() g_gpio->write(LCD_SPI_DC_PORT, LCD_SPI_DC_PIN, 1)

#define LCD_SPI_BLK_L() g_gpio->write(LCD_SPI_BK_PORT, LCD_SPI_BK_PIN, 0)
#define LCD_SPI_BLK_H() g_gpio->write(LCD_SPI_BK_PORT, LCD_SPI_BK_PIN, 1)

#define LCD_SPI_CS_L() g_gpio->write(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN, 0)
#define LCD_SPI_CS_H() g_gpio->write(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN, 1)

// spi配置需要空闲高电平, 2个edge采样
// 方向
#define LCD_SPI_DIR 0
// 用软件SPI
#define LCD_SPI_USE_SOFT 0

// 圆屏
#define USE_LCD_ROUND_SPI 0

static void mdelay(void)
{
	int i = 5000;
	while(i--)
	{
		
	}
}

void lcd_spi_write_data8(uint8_t dat)
{


	#if LCD_SPI_USE_SOFT
	sspi_write(dat);
	#else
		g_spi->write(1, &dat, 1);
	#endif
	// 

	mdelay();

}

/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
void lcd_spi_write_data(uint16_t dat)
{

	uint8_t data_temp[2];
	
	data_temp[0] = dat >> 8;
	data_temp[1] = dat;
	#if LCD_SPI_USE_SOFT
	sspi_write(dat >> 8);
	sspi_write(dat);
	#else
	
//	uint8_t temp = 0;
//	temp = dat >> 8;
//	g_spi->write(1, &temp, 1);

//	temp = dat;
//	g_spi->write(1, &temp, 1);
	g_spi->write(1, &data_temp[0], 2);
	#endif
	

}

/******************************************************************************
	  函数说明：LCD写入命令
	  入口数据：dat 写入的命令
	  返回值：  无
******************************************************************************/
void lcd_spi_write_reg(uint8_t dat)
{
	LCD_SPI_DC_L(); //写命令
	#if LCD_SPI_USE_SOFT
	sspi_write(dat);
	#else
		g_spi->write(1, &dat, 1);
	#endif
	// 

	LCD_SPI_DC_H(); //写数据
}

void lcd_round_spi_addr_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	lcd_spi_write_reg(0x2a); //列地址设置
	lcd_spi_write_data(x1);
	lcd_spi_write_data(x2);
	lcd_spi_write_reg(0x2b); //行地址设置
	lcd_spi_write_data(y1);
	lcd_spi_write_data(y2);
	lcd_spi_write_reg(0x2c); //储存器写
}

/******************************************************************************
	  函数说明：设置起始和结束地址
	  入口数据：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
	  返回值：  无
******************************************************************************/
void lcd_spi_addr_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	#if  USE_LCD_ROUND_SPI
	lcd_round_spi_addr_set(x1, y1, x2, y2);
	#else
	if (LCD_SPI_DIR == 0)
	{
		lcd_spi_write_reg(0x2a); //列地址设置
		lcd_spi_write_data(x1);
		lcd_spi_write_data(x2);
		lcd_spi_write_reg(0x2b); //行地址设置
		lcd_spi_write_data(y1);
		lcd_spi_write_data(y2);
		lcd_spi_write_reg(0x2c); //储存器写
	}
	else if (LCD_SPI_DIR == 1)
	{
		lcd_spi_write_reg(0x2a); //列地址设置
		lcd_spi_write_data(x1);
		lcd_spi_write_data(x2);
		lcd_spi_write_reg(0x2b); //行地址设置
		lcd_spi_write_data(y1 + 80);
		lcd_spi_write_data(y2 + 80);
		lcd_spi_write_reg(0x2c); //储存器写
	}
	else if (LCD_SPI_DIR == 2)
	{
		lcd_spi_write_reg(0x2a); //列地址设置
		lcd_spi_write_data(x1);
		lcd_spi_write_data(x2);
		lcd_spi_write_reg(0x2b); //行地址设置
		lcd_spi_write_data(y1);
		lcd_spi_write_data(y2);
		lcd_spi_write_reg(0x2c); //储存器写
	}
	else
	{
		lcd_spi_write_reg(0x2a); //列地址设置
		lcd_spi_write_data(x1 + 80);
		lcd_spi_write_data(x2 + 80);
		lcd_spi_write_reg(0x2b); //行地址设置
		lcd_spi_write_data(y1);
		lcd_spi_write_data(y2);
		lcd_spi_write_reg(0x2c); //储存器写
	}
	#endif
}

void lcd_round_spi_init(void)
{
	LCD_SPI_RES_L(); //复位
	delay_ms(100);
	LCD_SPI_RES_H();
    delay_ms(100);

	LCD_SPI_BLK_H(); //打开背光
	delay_ms(100);

    lcd_spi_write_reg(0xEF);
    lcd_spi_write_reg(0xEB);
    lcd_spi_write_data8(0x14);

    lcd_spi_write_reg(0xFE);
    lcd_spi_write_reg(0xEF);

    lcd_spi_write_reg(0xEB);
    lcd_spi_write_data8(0x14);

    lcd_spi_write_reg(0x84);
    lcd_spi_write_data8(0x40);

    lcd_spi_write_reg(0x85);
    lcd_spi_write_data8(0xFF);

    lcd_spi_write_reg(0x86);
    lcd_spi_write_data8(0xFF);

    lcd_spi_write_reg(0x87);
    lcd_spi_write_data8(0xFF);

    lcd_spi_write_reg(0x88);
    lcd_spi_write_data8(0x0A);

    lcd_spi_write_reg(0x89);
    lcd_spi_write_data8(0x21);

    lcd_spi_write_reg(0x8A);
    lcd_spi_write_data8(0x00);

    lcd_spi_write_reg(0x8B);
    lcd_spi_write_data8(0x80);

    lcd_spi_write_reg(0x8C);
    lcd_spi_write_data8(0x01);

    lcd_spi_write_reg(0x8D);
    lcd_spi_write_data8(0x01);

    lcd_spi_write_reg(0x8E);
    lcd_spi_write_data8(0xFF);

    lcd_spi_write_reg(0x8F);
    lcd_spi_write_data8(0xFF);

    lcd_spi_write_reg(0xB6);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x20);

    lcd_spi_write_reg(0x36);
    if (LCD_SPI_DIR == 0)
        lcd_spi_write_data8(0x08);
    else if (LCD_SPI_DIR == 1)
        lcd_spi_write_data8(0xC8);
    else if (LCD_SPI_DIR == 2)
        lcd_spi_write_data8(0x68);
    else
        lcd_spi_write_data8(0xA8);

    lcd_spi_write_reg(0x3A);
    lcd_spi_write_data8(0x05);

    lcd_spi_write_reg(0x90);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x08);

    lcd_spi_write_reg(0xBD);
    lcd_spi_write_data8(0x06);

    lcd_spi_write_reg(0xBC);
    lcd_spi_write_data8(0x00);

    lcd_spi_write_reg(0xFF);
    lcd_spi_write_data8(0x60);
    lcd_spi_write_data8(0x01);
    lcd_spi_write_data8(0x04);

    lcd_spi_write_reg(0xC3);
    lcd_spi_write_data8(0x13);
    lcd_spi_write_reg(0xC4);
    lcd_spi_write_data8(0x13);

    lcd_spi_write_reg(0xC9);
    lcd_spi_write_data8(0x22);

    lcd_spi_write_reg(0xBE);
    lcd_spi_write_data8(0x11);

    lcd_spi_write_reg(0xE1);
    lcd_spi_write_data8(0x10);
    lcd_spi_write_data8(0x0E);

    lcd_spi_write_reg(0xDF);
    lcd_spi_write_data8(0x21);
    lcd_spi_write_data8(0x0c);
    lcd_spi_write_data8(0x02);

    lcd_spi_write_reg(0xF0);
    lcd_spi_write_data8(0x45);
    lcd_spi_write_data8(0x09);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x26);
    lcd_spi_write_data8(0x2A);

    lcd_spi_write_reg(0xF1);
    lcd_spi_write_data8(0x43);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x72);
    lcd_spi_write_data8(0x36);
    lcd_spi_write_data8(0x37);
    lcd_spi_write_data8(0x6F);

    lcd_spi_write_reg(0xF2);
    lcd_spi_write_data8(0x45);
    lcd_spi_write_data8(0x09);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x26);
    lcd_spi_write_data8(0x2A);

    lcd_spi_write_reg(0xF3);
    lcd_spi_write_data8(0x43);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x72);
    lcd_spi_write_data8(0x36);
    lcd_spi_write_data8(0x37);
    lcd_spi_write_data8(0x6F);

    lcd_spi_write_reg(0xED);
    lcd_spi_write_data8(0x1B);
    lcd_spi_write_data8(0x0B);

    lcd_spi_write_reg(0xAE);
    lcd_spi_write_data8(0x77);

    lcd_spi_write_reg(0xCD);
    lcd_spi_write_data8(0x63);

    lcd_spi_write_reg(0x70);
    lcd_spi_write_data8(0x07);
    lcd_spi_write_data8(0x07);
    lcd_spi_write_data8(0x04);
    lcd_spi_write_data8(0x0E);
    lcd_spi_write_data8(0x0F);
    lcd_spi_write_data8(0x09);
    lcd_spi_write_data8(0x07);
    lcd_spi_write_data8(0x08);
    lcd_spi_write_data8(0x03);

    lcd_spi_write_reg(0xE8);
    lcd_spi_write_data8(0x34);

    lcd_spi_write_reg(0x62);
    lcd_spi_write_data8(0x18);
    lcd_spi_write_data8(0x0D);
    lcd_spi_write_data8(0x71);
    lcd_spi_write_data8(0xED);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x18);
    lcd_spi_write_data8(0x0F);
    lcd_spi_write_data8(0x71);
    lcd_spi_write_data8(0xEF);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x70);

    lcd_spi_write_reg(0x63);
    lcd_spi_write_data8(0x18);
    lcd_spi_write_data8(0x11);
    lcd_spi_write_data8(0x71);
    lcd_spi_write_data8(0xF1);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x18);
    lcd_spi_write_data8(0x13);
    lcd_spi_write_data8(0x71);
    lcd_spi_write_data8(0xF3);
    lcd_spi_write_data8(0x70);
    lcd_spi_write_data8(0x70);

    lcd_spi_write_reg(0x64);
    lcd_spi_write_data8(0x28);
    lcd_spi_write_data8(0x29);
    lcd_spi_write_data8(0xF1);
    lcd_spi_write_data8(0x01);
    lcd_spi_write_data8(0xF1);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x07);

    lcd_spi_write_reg(0x66);
    lcd_spi_write_data8(0x3C);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0xCD);
    lcd_spi_write_data8(0x67);
    lcd_spi_write_data8(0x45);
    lcd_spi_write_data8(0x45);
    lcd_spi_write_data8(0x10);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x00);

    lcd_spi_write_reg(0x67);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x3C);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x01);
    lcd_spi_write_data8(0x54);
    lcd_spi_write_data8(0x10);
    lcd_spi_write_data8(0x32);
    lcd_spi_write_data8(0x98);

    lcd_spi_write_reg(0x74);
    lcd_spi_write_data8(0x10);
    lcd_spi_write_data8(0x85);
    lcd_spi_write_data8(0x80);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x00);
    lcd_spi_write_data8(0x4E);
    lcd_spi_write_data8(0x00);

    lcd_spi_write_reg(0x98);
    lcd_spi_write_data8(0x3e);
    lcd_spi_write_data8(0x07);

    lcd_spi_write_reg(0x35);
    lcd_spi_write_reg(0x21);

    lcd_spi_write_reg(0x11);
    delay_ms(120);
    lcd_spi_write_reg(0x29);
    delay_ms(20);
}
void lcd_spi_init(void)
{
	LCD_SPI_RES_L(); //复位
	delay_ms(100);
	LCD_SPI_RES_H();
	delay_ms(100);

	LCD_SPI_BLK_H(); //打开背光
	delay_ms(100);

	//************* Start Initial Sequence **********//
	lcd_spi_write_reg(0x11); // Sleep out
	delay_ms(120);			 // Delay 120ms
	//************* Start Initial Sequence **********//
	lcd_spi_write_reg(0x36);
	if (LCD_SPI_DIR == 0)
		lcd_spi_write_data8(0x00);
	else if (LCD_SPI_DIR == 1)
		lcd_spi_write_data8(0xC0);
	else if (LCD_SPI_DIR == 2)
		lcd_spi_write_data8(0x70);
	else
		lcd_spi_write_data8(0xA0);

	lcd_spi_write_reg(0x3A);
	lcd_spi_write_data8(0x05);

	lcd_spi_write_reg(0xB2);
	lcd_spi_write_data8(0x0C);
	lcd_spi_write_data8(0x0C);
	lcd_spi_write_data8(0x00);
	lcd_spi_write_data8(0x33);
	lcd_spi_write_data8(0x33);

	lcd_spi_write_reg(0xB7);
	lcd_spi_write_data8(0x35);

	lcd_spi_write_reg(0xBB);
	lcd_spi_write_data8(0x19);

	lcd_spi_write_reg(0xC0);
	lcd_spi_write_data8(0x2C);

	lcd_spi_write_reg(0xC2);
	lcd_spi_write_data8(0x01);

	lcd_spi_write_reg(0xC3);
	lcd_spi_write_data8(0x12);

	lcd_spi_write_reg(0xC4);
	lcd_spi_write_data8(0x20);

	lcd_spi_write_reg(0xC6);
	lcd_spi_write_data8(0x0F);

	lcd_spi_write_reg(0xD0);
	lcd_spi_write_data8(0xA4);
	lcd_spi_write_data8(0xA1);

	lcd_spi_write_reg(0xE0);
	lcd_spi_write_data8(0xD0);
	lcd_spi_write_data8(0x04);
	lcd_spi_write_data8(0x0D);
	lcd_spi_write_data8(0x11);
	lcd_spi_write_data8(0x13);
	lcd_spi_write_data8(0x2B);
	lcd_spi_write_data8(0x3F);
	lcd_spi_write_data8(0x54);
	lcd_spi_write_data8(0x4C);
	lcd_spi_write_data8(0x18);
	lcd_spi_write_data8(0x0D);
	lcd_spi_write_data8(0x0B);
	lcd_spi_write_data8(0x1F);
	lcd_spi_write_data8(0x23);

	lcd_spi_write_reg(0xE1);
	lcd_spi_write_data8(0xD0);
	lcd_spi_write_data8(0x04);
	lcd_spi_write_data8(0x0C);
	lcd_spi_write_data8(0x11);
	lcd_spi_write_data8(0x13);
	lcd_spi_write_data8(0x2C);
	lcd_spi_write_data8(0x3F);
	lcd_spi_write_data8(0x44);
	lcd_spi_write_data8(0x51);
	lcd_spi_write_data8(0x2F);
	lcd_spi_write_data8(0x1F);
	lcd_spi_write_data8(0x1F);
	lcd_spi_write_data8(0x20);
	lcd_spi_write_data8(0x23);
	lcd_spi_write_reg(0x21);

	lcd_spi_write_reg(0x29);
}

/**
 * @brief
 *
 * @param ch
 * @return int
 */
static int lcd_spi_hw_init(uint8_t ch)
{

	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param cmd
 * @return int
 */
static int lcd_spi_hw_get(uint8_t ch, uint8_t cmd)
{
	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param cmd
 * @param data
 * @return int
 */
static int lcd_spi_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @return int
 */
static int lcd_spi_hw_close(uint8_t ch)
{
	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param config
 * @return int
 */
static int lcd_spi_config(uint8_t ch, void *config)
{

	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param x
 * @param y
 * @param color
 * @return int
 */
static int lcd_spi_draw_point(uint8_t ch, uint16_t x, uint16_t y, uint32_t color)
{
	lcd_spi_addr_set(x, y, x, y); //设置光标位置
	lcd_spi_write_data(color);
	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param sx
 * @param sy
 * @param ex
 * @param ey
 * @param color
 * @return int
 */

static int lcd_spi_fill(uint8_t ch, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{

	uint16_t i, j;
	lcd_spi_addr_set(sx, sy, ex - 1, ey - 1); //设置显示范围
	
	for (i = sy; i < ey; i++)
	{
		for (j = sx; j < ex; j++)
		{
			lcd_spi_write_data(color);
		}
	}
	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param sx
 * @param sy
 * @param ex
 * @param ey
 * @param color
 * @return int
 */
static int lcd_spi_color_fill(uint8_t ch, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{

	return 0;
}

int lcd_spi_flush(uint8_t ch)
{
return 0;
}

static struct zf_lcd_t zf_lcd_spi =
	{
		.hw = {
			.init = lcd_spi_hw_init,
			.get = lcd_spi_hw_get,
			.set = lcd_spi_hw_set,
			.close = lcd_spi_hw_close},
		.config = lcd_spi_config,
		.draw_point = lcd_spi_draw_point,
		.fill = lcd_spi_fill,
		.color_fill = lcd_spi_color_fill,
		.flush = lcd_spi_flush
};




int dev_lcd_spi_init(void)
{

	zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	zf_driver_get(ZF_DRIVER_TYPE_SPI, (void **)&g_spi);
	if (check_driver(ZF_DRIVER_TYPE_GPIO, g_gpio))
	{
		return 1;
	}
	else
	{

		if (check_driver(ZF_DRIVER_TYPE_SPI, g_spi))
		{
			return 1;
		}
	}

	#if  USE_LCD_ROUND_SPI
	lcd_round_spi_init();
	#else
	lcd_spi_init();
	#endif

	// 标准注册
	zf_driver_register(ZF_DRIVER_TYPE_LCD_SPI_STD, &zf_lcd_spi);
	return 0;
}
