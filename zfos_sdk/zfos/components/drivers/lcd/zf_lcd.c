#include "../zf_driver.h"
#include "../zf_device.h"

#include "zf_lcd.h"
#include "lcd_font.h"

#include "zf_lcd_define.h"
#include "lcd_config.h"

#include "lcd_img.h"
static uint8_t lcd_index = 0;
static struct zf_lcd_t *g_zf_lcd[5];
struct zf_lcd_config_t *g_zf_lcd_config[5];


/**
 * @brief
 *
 * @return struct zf_lcd_t*
 */
struct zf_lcd_t *lcd_get_ops(uint8_t ch)
{
    return g_zf_lcd[ch];
}

/**
 * @brief
 *
 * @param x
 * @param y
 * @param color
 */
void lcd_draw_point(uint8_t ch, uint16_t x, uint16_t y, uint32_t color)
{
    g_zf_lcd[ch]->draw_point(0, x, y, color);
}

/**
 * @brief
 *
 * @param x
 * @param y
 * @param num
 * @param size
 * @param mode
 * @param font_color
 * @param back_color
 */
void lcd_show_char(uint8_t ch,uint16_t x, uint16_t y, uint16_t num, uint8_t size, uint8_t mode, uint16_t font_color, uint16_t back_color)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //得到字体一个字符对应点阵集所占的字节数
    num = num - ' ';                                                //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    for (t = 0; t < csize; t++)
    {
        if (size == 12)

            temp = asc2_1206[num][t]; //调用1206字体
        else if (size == 16)
            temp = asc2_1608[num][t]; //调用1608字体
        else if (size == 24)
            temp = asc2_2412[num][t]; //调用2412字体
        else
            return; //没有的字库

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
            {
                g_zf_lcd[ch]->draw_point(0, x, y, font_color);
            }
            else
            {

                if (mode)
                {
                    g_zf_lcd[ch]->draw_point(0, x, y, back_color);
                }
            }
            temp <<= 1;
            y++;
            if (y >= g_zf_lcd_config[ch]->pix_height)
                return; //超区域了
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                if (x >= g_zf_lcd_config[ch]->pix_width)
                    return; //超区域了
                break;
            }
        }
    }
}

void oled_show_char(uint8_t ch,uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode)
{
    uint8_t i, m, temp, size2, chr1;
    uint8_t x0 = x, y0 = y;

    size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //得到字体一个字符对应点阵集所占的字节数
    chr1 = chr - ' ';                                          //计算偏移后的值
    for (i = 0; i < size2; i++)
    {

        if (size1 == 12)
        {
            temp = asc2_1206_oled[chr1][i];
        } //调用1206字体
        else if (size1 == 24)
        {
            temp = asc2_2412_oled[chr1][i];
        } //调用2412字体
        else
            return;

        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                g_zf_lcd[ch]->draw_point(0, x, y, mode);
            else
                g_zf_lcd[ch]->draw_point(0, x, y, !mode);

            temp >>= 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

/**
 * @brief
 *
 * @param x
 * @param y
 * @param size
 * @param p
 * @param color
 */
void lcd_show_string(uint8_t ch,uint16_t x, uint16_t y, uint8_t size, uint8_t *p, uint16_t color)
{
    uint8_t x0 = x;
    uint16_t width = strlen((char *)p) * size;
    uint16_t height = size;

    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }
        if (y >= height)
            break; //退出
				if(g_zf_lcd_config[ch]->type == LCD_TYPE_OLED)
				{
					 oled_show_char(ch,x, y, *p, size, color);
				}else{
					 lcd_show_char(ch,x, y, *p, size, FONT_USE_BACK_COLOR, color, FONT_BACK_COLOR);
				}
        x += size / 2;
        p++;
    }
}

/**
 * @brief
 *
 * @return int
 */
int dev_lcd_init(void)
{



    zf_driver_get(ZF_DRIVER_TYPE_LCD_RGB, (void **)&g_zf_lcd[lcd_index]);
    if (check_driver(ZF_DRIVER_TYPE_LCD_RGB, &g_zf_lcd[lcd_index]))
    {
        return 1;
    }

    lcd_config(LCD_TYPE_RGB, g_zf_lcd[lcd_index], &g_zf_lcd_config[lcd_index]);
		lcd_index++;
		
//    // 8080
//    zf_driver_get(ZF_DRIVER_TYPE_LCD_8080_STD, (void **)&g_zf_lcd[lcd_index]);
//    lcd_config(LCD_TYPE_8080, g_zf_lcd[lcd_index], &g_zf_lcd_config[lcd_index]);
//		lcd_index++;
		
		
    zf_driver_get(ZF_DRIVER_TYPE_LCD_OLED_STD, (void **)&g_zf_lcd[lcd_index]);
    lcd_config(LCD_TYPE_OLED, g_zf_lcd[lcd_index], &g_zf_lcd_config[lcd_index]);
		lcd_index++;
		
		
    zf_driver_get(ZF_DRIVER_TYPE_LCD_SPI_STD, (void **)&g_zf_lcd[lcd_index]);
    lcd_config(LCD_TYPE_SPI, g_zf_lcd[lcd_index], &g_zf_lcd_config[lcd_index]);
		lcd_index++;
		
		
		uint16_t  color_temp = 0xF800;
		char mstr[100];
	
		for(int i = 0;i<3;i++)
		{
				
				sprintf(mstr,"zfos:%d",color_temp);
				g_zf_lcd[i]->fill(0, 0, 0,g_zf_lcd_config[i]->pix_width - 1,g_zf_lcd_config[i]->pix_height - 1,0xF800);
				lcd_show_string(i,10, 10, 24, mstr, 1);
				g_zf_lcd[i]->flush(0);
		}
		// color_temp++;

        // for(int i = 0;i<50;i++)
        // {
        //     for(int j = 0;j<50;j++)
        //     {
        //       lcd_draw_point(2,i,50+j,0xF800);
        //     }
             
        // }
    
    	for(int i = 0;i<3;i++)
		{
            int xxxx1 = 0;
            uint16_t *data_p =  (uint16_t*)&gImage_test;
            for(int h=0;h<240;h++){
                        for(int m=0;m<240;m++){
                          lcd_draw_point(i,m,h,*data_p++);
                }
            }
        }
    

    // lcd_show_string(2,10, 10, 24, "asdfasdf", 1);
		
    // lcd_show_string(10, 10, 24, "hello kirito zfos v1.2", BLUE);
   
    //
    return 0;
}
