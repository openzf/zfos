#include "../zf_device.h"
#include "../zf_driver.h"
#include "zf_lcd_define.h"
#include "zf_lcd_8080.h"

#define USE_8080_16BIT  0
#if USE_8080_16BIT
// 16 bit  
//LCD地址结构体
typedef struct
{
	volatile uint16_t REG;
	volatile uint16_t RAM;
}FSMC_LCD_TypeDef;
// A19作为数据命令区分线 
#else
// 8bit
typedef struct
{
	volatile uint8_t REG;
	volatile uint8_t RAM;
} FSMC_LCD_TypeDef;
#endif

static FSMC_LCD_TypeDef *FSMC_LCD;
static struct zf_lcd_8080_t  *g_zf_lcd_8080;
static  struct zf_lcd_config_t g_lcd_config_t;

// -- api
/**
 * @brief 
 * 
 */
void lcd_8080_write_gram_pre(void)
{
    FSMC_LCD->REG = g_lcd_config_t.m8080_config.w_ram_cmd;
}

/**
 * @brief 
 * 
 * @param value 
 */
void lcd_8080_write_reg(uint16_t value)
{
    FSMC_LCD->REG = value;
}

/**
 * @brief 
 * 
 * @param value 
 */
void lcd_8080_write_data(uint16_t value)
{
    FSMC_LCD->RAM = value;
}

/**
 * @brief 
 * 
 * @param value 
 */
void lcd_8080_write_color(uint16_t value)
{
#if USE_8080_16BIT
    FSMC_LCD->RAM = value;
#else

    FSMC_LCD->RAM = value >> 8;
    FSMC_LCD->RAM = value & 0xff;
#endif
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 */
static void lcd_8080_cursor(uint16_t x,uint16_t y)
{

	uint16_t mx = 0;
	uint16_t my = 0;
	
	#if LCD_USE_HARD_DIR
		mx = x;
		my = y;
	#else

		if(g_lcd_config_t.dir == LCD_DIR_VERTICAL) {
				mx = y;
				my = g_lcd_config_t.pix_height - x -1;
		} else {
				mx = x;
				my = y;
		}
	#endif
	
	lcd_8080_write_reg(g_lcd_config_t.m8080_config.set_x_cmd);   
	lcd_8080_write_data(mx>>8);
	lcd_8080_write_data(mx&0xff);
	lcd_8080_write_data(mx>>8);
	lcd_8080_write_data(mx&0xff);

	lcd_8080_write_reg(g_lcd_config_t.m8080_config.set_y_cmd);   
	lcd_8080_write_data(my>>8);
	lcd_8080_write_data(my&0xff);
	lcd_8080_write_data(my>>8);
	lcd_8080_write_data(my&0xff);
	lcd_8080_write_reg(g_lcd_config_t.m8080_config.w_ram_cmd);  
}

/**
 * @brief 
 * 
 * @param start_x 
 * @param start_y 
 * @param end_x 
 * @param end_y 
 */
void lcd_8080_set_set_windows(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
    uint16_t msx = 0;
    uint16_t msy = 0;

    uint16_t mex = 0;
    uint16_t mey = 0;

#if LCD_USE_HARD_DIR
    msx = start_x;
    msy = start_y;
    mex = end_x;
    mey = end_y;
#else
    // 竖屏
    if (g_lcd_config_t.dir == LCD_DIR_VERTICAL)
    {
        // 原生分辨率
        msx = start_y;
        msy = g_lcd_config_t.pix_height - end_x - 1;
        mex = end_y,
        mey = g_lcd_config_t.pix_height - start_x - 1;
    }
    else
    { // 横屏
        msx = start_x;
        msy = start_y;
        mex = end_x;
        mey = end_y;
    }
#endif

    lcd_8080_write_reg(g_lcd_config_t.m8080_config.set_x_cmd);
    lcd_8080_write_data(msx >> 8);
    lcd_8080_write_data(msx & 0xff);
    lcd_8080_write_data(mex >> 8);
    lcd_8080_write_data(mex & 0xff);

    lcd_8080_write_reg(g_lcd_config_t.m8080_config.set_y_cmd);
    lcd_8080_write_data(msy >> 8);
    lcd_8080_write_data(msy & 0xff);
    lcd_8080_write_data(mey >> 8);
    lcd_8080_write_data(mey & 0xff);

    lcd_8080_write_reg(g_lcd_config_t.m8080_config.w_ram_cmd);
}
void lcd_8080_set_set_cursor(uint16_t x, uint16_t y)
{
    uint16_t mx = 0;
    uint16_t my = 0;

#if LCD_USE_HARD_DIR
    mx = x;
    my = y;
#else
    // 竖屏
    if (g_lcd_config_t.dir == LCD_DIR_VERTICAL)
    {
        mx = y;
        my = g_lcd_config_t.pix_height - x - 1;
    }
    else
    { // 横屏
        mx = x;
        my = y;
    }
#endif

    lcd_8080_write_reg(g_lcd_config_t.m8080_config.set_x_cmd);
    lcd_8080_write_data(mx >> 8);
    lcd_8080_write_data(mx & 0xff);
    lcd_8080_write_data(mx >> 8);
    lcd_8080_write_data(mx & 0xff);

    lcd_8080_write_reg(g_lcd_config_t.m8080_config.set_y_cmd);
    lcd_8080_write_data(my >> 8);
    lcd_8080_write_data(my & 0xff);
    lcd_8080_write_data(my >> 8);
    lcd_8080_write_data(my & 0xff);
    lcd_8080_write_reg(g_lcd_config_t.m8080_config.w_ram_cmd);
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int lcd_8080_hw_init(uint8_t ch)
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
static int lcd_8080_hw_get(uint8_t ch,uint8_t cmd)
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
static int lcd_8080_hw_set(uint8_t ch,uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int lcd_8080_hw_close(uint8_t ch)
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
static int lcd_8080_config(uint8_t ch,void *config)
{
    g_lcd_config_t = *((struct zf_lcd_config_t *)config);
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
static int lcd_8080_draw_point(uint8_t ch,uint16_t x, uint16_t y, uint32_t color)
{
    lcd_8080_cursor(x,y);
    lcd_8080_write_color(color);
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
static int lcd_8080_fill(uint8_t ch,uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{

    int xlen= ex-sx+1;
    int ylen = ey - sy +1;

    uint32_t all_pix = xlen * ylen;
    uint32_t i = 0;
        
    lcd_8080_set_set_windows(sx,sy,ex,ey);
    for(i = 0; i<all_pix; i++)
    {
        lcd_8080_write_color(color);
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
static int lcd_8080_color_fill(uint8_t ch,uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t height,width;

		
    width=ex-sx+1; 			
    height=ey-sy+1;			
    uint32_t total=width*height,i=0;
    lcd_8080_set_set_windows(sx,sy,ex,ey);
    while(i<=total)
    {
    lcd_8080_write_color(color[i++]);
    }
  return 0;
}

int lcd_8080_flush(uint8_t ch)
{
return 0;
}

static struct zf_lcd_t zf_lcd_8080 =
    {
       .hw={
            .init = lcd_8080_hw_init,
            .get = lcd_8080_hw_get,
            .set = lcd_8080_hw_set,
            .close =  lcd_8080_hw_close
        },
		.config = lcd_8080_config,
        .draw_point = lcd_8080_draw_point,
        .fill = lcd_8080_fill,
        .color_fill = lcd_8080_color_fill,
        .flush = lcd_8080_flush
};
		

/**
 * @brief 
 * 
 */
int drv_lcd_8080_init(void)
{

    uint32_t addr = 0;
    zf_driver_get(ZF_DRIVER_TYPE_LCD_8080, (void **)&g_zf_lcd_8080);
    if (check_driver(ZF_DRIVER_TYPE_LCD_8080,g_zf_lcd_8080))
    {
    return 1;
    }

    // 获取GPU addr
    g_zf_lcd_8080->get_gram_addr(0,&addr);
    FSMC_LCD = (FSMC_LCD_TypeDef *)addr; 

    // 标准注册
    zf_driver_register(ZF_DRIVER_TYPE_LCD_8080_STD,&zf_lcd_8080);
    return 0;
}



