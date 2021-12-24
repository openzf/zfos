
#include "../zf_driver.h"
#include "../zf_device.h"
#include "zf_dts.h"
#include "zf_si2c.h"

#include "oled.h"


#define OLED_ADDRESS 0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78
#define OLED_CMD 0		  //写命令
#define OLED_DATA 1		  //写数据

uint8_t OLED_GRAM[144][8];

// 软件i2c
#define OLED_USE_SOFT 0
// 软件i2c模仿硬件i2c
#define OLED_USE_HSFOT 0
static struct zf_i2c_t *g_i2c;

#define OLED_USE_0_9_INCH  0

void oled_write_gram(int i)
{
#if OLED_USE_SOFT
	si2c_start();
	si2c_sent_byte(0x78);
	si2c_wait_ack();
	si2c_sent_byte(0x40);
	si2c_wait_ack();
	for (int n = 0; n < 128; n++)
	{
		si2c_sent_byte(OLED_GRAM[n][i]);
		si2c_wait_ack();
	}
	si2c_stop();
#else
	uint8_t mtemp[128];
	for (int n = 0; n < 128; n++)
	{
		mtemp[n] = OLED_GRAM[n][i];
	}
#if OLED_USE_HSFOT
	g_i2c->write(0, 0x78, 0x40, mtemp, 128);
#else
	si2c_write(0, 0x78, 0x40, mtemp, 128);
#endif
#endif
}

//发送一个字节
// mode:数据/命令标志 0,表示命令;1,表示数据;
void oled_write_byte(uint8_t dat, uint8_t mode)
{
#if OLED_USE_SOFT
	si2c_start();
	si2c_sent_byte(0x78);
	si2c_wait_ack();
	if (mode)
	{
		si2c_sent_byte(0x40);
	}
	else
	{
		si2c_sent_byte(0x00);
	}
	si2c_wait_ack();
	si2c_sent_byte(dat);
	si2c_wait_ack();
	si2c_stop();
#else
	if (mode)
	{
#if OLED_USE_HSFOT
		g_i2c->write(0, 0x78, 0x40, &dat, 1);
#else
		si2c_write(0, 0x78, 0x40, &dat, 1);
#endif
	}
	else
	{
#if OLED_USE_HSFOT
		g_i2c->write(0, 0x78, 0x00, &dat, 1);
#else
		si2c_write(0, 0x78, 0x00, &dat, 1);
#endif
	}

#endif
}

void oled_init(void)
{
	#if OLED_USE_0_9_INCH 
	oled_write_byte(0xAE, OLED_CMD); //--turn off oled panel
	oled_write_byte(0x00, OLED_CMD); //---set low column address
	oled_write_byte(0x10, OLED_CMD); //---set high column address
	oled_write_byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_write_byte(0x81, OLED_CMD); //--set contrast control register
	oled_write_byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
	oled_write_byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_byte(0xA6, OLED_CMD); //--set normal display
	oled_write_byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	oled_write_byte(0x3f, OLED_CMD); //--1/64 duty
	oled_write_byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oled_write_byte(0x00, OLED_CMD); //-not offset
	oled_write_byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
	oled_write_byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_byte(0xD9, OLED_CMD); //--set pre-charge period
	oled_write_byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_byte(0xDA, OLED_CMD); //--set com pins hardware configuration
	oled_write_byte(0x12, OLED_CMD);
	oled_write_byte(0xDB, OLED_CMD); //--set vcomh
	oled_write_byte(0x40, OLED_CMD); // Set VCOM Deselect Level
	oled_write_byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_write_byte(0x02, OLED_CMD); //
	oled_write_byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
	oled_write_byte(0x14, OLED_CMD); //--set(0x10) disable
	oled_write_byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
	oled_write_byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	oled_write_byte(0xAF, OLED_CMD);
	#else
	
	oled_write_byte(0xAE,OLED_CMD); /*display off*/ 
	oled_write_byte(0x02,OLED_CMD); /*set lower column address*/ 
	oled_write_byte(0x10,OLED_CMD); /*set higher column address*/ 
	oled_write_byte(0x40,OLED_CMD); /*set display start line*/ 
	oled_write_byte(0xB0,OLED_CMD); /*set page address*/
	oled_write_byte(0x81,OLED_CMD); /*contract control*/ 
	oled_write_byte(0xcf,OLED_CMD); /*128*/ 
	oled_write_byte(0xA1,OLED_CMD); /*set segment remap*/ 
	oled_write_byte(0xA6,OLED_CMD); /*normal / reverse*/ 
	oled_write_byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	oled_write_byte(0x3F,OLED_CMD); /*duty = 1/64*/ 
	oled_write_byte(0xad,OLED_CMD); /*set charge pump enable*/ 
	oled_write_byte(0x8b,OLED_CMD); /* 0x8B 内供 VCC */ 
	oled_write_byte(0x33,OLED_CMD); /*0X30---0X33 set VPP 9V */ 
	oled_write_byte(0xC8,OLED_CMD); /*Com scan direction*/ 
	oled_write_byte(0xD3,OLED_CMD); /*set display offset*/ 
	oled_write_byte(0x00,OLED_CMD); /* 0x20 */ 
	oled_write_byte(0xD5,OLED_CMD); /*set osc division*/ 
	oled_write_byte(0x80,OLED_CMD); 
	oled_write_byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	oled_write_byte(0x1f,OLED_CMD); /*0x22*/ 
	oled_write_byte(0xDA,OLED_CMD); /*set COM pins*/ 
	oled_write_byte(0x12,OLED_CMD); 
	oled_write_byte(0xdb,OLED_CMD); /*set vcomh*/ 
	oled_write_byte(0x40,OLED_CMD);
	
	oled_write_byte(0xAF,OLED_CMD); /*display ON*/
	#endif
}

/**
 * @brief
 *
 * @param ch
 * @return int
 */
static int lcd_oled_hw_init(uint8_t ch)
{
	oled_write_byte(0x8D, OLED_CMD); //电荷泵使能
	oled_write_byte(0x14, OLED_CMD); //开启电荷泵
	oled_write_byte(0xAF, OLED_CMD); //点亮屏幕
	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param cmd
 * @return int
 */
static int lcd_oled_hw_get(uint8_t ch, uint8_t cmd)
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
static int lcd_oled_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @return int
 */
static int lcd_oled_hw_close(uint8_t ch)
{
	oled_write_byte(0x8D, OLED_CMD); //电荷泵使能
	oled_write_byte(0x10, OLED_CMD); //关闭电荷泵
	oled_write_byte(0xAE, OLED_CMD); //关闭屏幕

	return 0;
}

/**
 * @brief
 *
 * @param ch
 * @param config
 * @return int
 */
static int lcd_oled_config(uint8_t ch, void *config)
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
static int lcd_oled_draw_point(uint8_t ch, uint16_t x, uint16_t y, uint32_t color)
{

	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	if (color)
	{
		OLED_GRAM[x][i] |= n;
	}
	else
	{
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
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
static int lcd_oled_fill(uint8_t ch, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{

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
static int lcd_oled_color_fill(uint8_t ch, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{

	return 0;
}

//更新显存到OLED
int lcd_oled_flush(uint8_t ch)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		oled_write_byte(0xb0 + i, OLED_CMD); //设置行起始地址
		#if OLED_USE_0_9_INCH 
		oled_write_byte(0x00, OLED_CMD);	 //设置低列起始地址
		#else
		oled_write_byte(0x02, OLED_CMD);	 //设置低列起始地址
		#endif
		oled_write_byte(0x10, OLED_CMD);	 //设置高列起始地址

		oled_write_gram(i);
	}
	return 0;
}

static struct zf_lcd_t zf_lcd_oled =
	{
		.hw = {
			.init = lcd_oled_hw_init,
			.get = lcd_oled_hw_get,
			.set = lcd_oled_hw_set,
			.close = lcd_oled_hw_close},
		.config = lcd_oled_config,
		.draw_point = lcd_oled_draw_point,
		.fill = lcd_oled_fill,
		.color_fill = lcd_oled_color_fill,
		.flush = lcd_oled_flush};

int dev_oled_init(void)
{

	zf_driver_get(ZF_DRIVER_TYPE_I2C, (void **)&g_i2c);
	if (check_driver(ZF_DRIVER_TYPE_I2C, g_i2c))
	{
		return 1;
	}

	oled_init();
	// 标准注册
	zf_driver_register(ZF_DRIVER_TYPE_LCD_OLED_STD, &zf_lcd_oled);

	return 0;
}
