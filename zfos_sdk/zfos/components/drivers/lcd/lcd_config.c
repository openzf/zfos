#include "lcd_config.h"
#include "zf_driver.h"
#include "zf_lcd_8080.h"

static void lcd_init_param_ili9481(void)
{
    //TEST  cmi35mva 20180510 ok优秀

    lcd_8080_write_reg(0x01); //Soft_rese
    delay_ms(220);

    //Exit_sleep_mode
    lcd_8080_write_reg(0x11);
    delay_ms(280);

    lcd_8080_write_reg(0xd0);  //Power_Setting
    lcd_8080_write_data(0x07); //07  VC[2:0] Sets the ratio factor of Vci to generate the reference voltages Vci1
    lcd_8080_write_data(0x44); //41  BT[2:0] Sets the Step up factor and output voltage level from the reference voltages Vci1
    lcd_8080_write_data(0x1E); //1f  17   1C  VRH[3:0]: Sets the factor to generate VREG1OUT from VCILVL
    delay_ms(220);

    lcd_8080_write_reg(0xd1);  //VCOM Control
    lcd_8080_write_data(0x00); //00
    lcd_8080_write_data(0x0C); //1A   VCM [6:0] is used to set factor to generate VCOMH voltage from the reference voltage VREG1OUT  15    09
    lcd_8080_write_data(0x1A); //1F   VDV[4:0] is used to set the VCOM alternating amplitude in the range of VREG1OUT x 0.70 to VREG1OUT   1F   18

    lcd_8080_write_reg(0xC5);  //Frame Rate
    lcd_8080_write_data(0x03); // 03   02

    lcd_8080_write_reg(0xd2);  //Power_Setting for Normal Mode
    lcd_8080_write_data(0x01); //01
    lcd_8080_write_data(0x11); //11

    lcd_8080_write_reg(0xE4); //?
    lcd_8080_write_data(0xa0);
    lcd_8080_write_reg(0xf3);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x2a);

    //1  Gamma Setting OK
    lcd_8080_write_reg(0xc8);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x26);
    lcd_8080_write_data(0x21);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x1f);
    lcd_8080_write_data(0x65);
    lcd_8080_write_data(0x23);
    lcd_8080_write_data(0x77);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x0f);
    lcd_8080_write_data(0x00);

    //Panel Driving Setting
    lcd_8080_write_reg(0xC0);
    lcd_8080_write_data(0x00); //1//00  REV  SM  GS
    lcd_8080_write_data(0x3B); //2//NL[5:0]: Sets the number of lines to drive the lcd at an interval of 8 lines.
    lcd_8080_write_data(0x00); //3//SCN[6:0]
    lcd_8080_write_data(0x02); //4//PTV: Sets the Vcom output in non-display area drive period
    lcd_8080_write_data(0x11); //5//NDL: Sets the source output level in non-display area.  PTG: Sets the scan mode in non-display area.

    //Interface Control
    lcd_8080_write_reg(0xc6);
    lcd_8080_write_data(0x83);
    //GAMMA SETTING

    lcd_8080_write_reg(0xf0); //?
    lcd_8080_write_data(0x01);

    lcd_8080_write_reg(0xE4); //?
    lcd_8080_write_data(0xa0);

    //////倒装设置   NG
    // Set_address_mode  ---- 刷新方向

    lcd_8080_write_reg(0x36);
#if LCD_USE_HARD_DIR
    // 竖屏
    if (g_mlcd->dir == LCD_DIR_VERTICAL)
    {
        lcd_8080_write_data(0x0A);
    }
    else
    { // 横屏
        lcd_8080_write_data(0x28);
    }
#else
    lcd_8080_write_data(0x28);
#endif

    lcd_8080_write_reg(0x3a);
    lcd_8080_write_data(0x55);

    lcd_8080_write_reg(0xb4); //Display Mode and Frame Memory Write Mode Setting
    lcd_8080_write_data(0x02);
    lcd_8080_write_data(0x00); //?
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x01);

    delay_ms(280);

    lcd_8080_write_reg(0x2a);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x01);
    lcd_8080_write_data(0x3F); //3F

    lcd_8080_write_reg(0x2b);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x00);
    lcd_8080_write_data(0x01);
    lcd_8080_write_data(0xDf); //DF

    //lcd_8080_write_reg(0x21);
    lcd_8080_write_reg(0x29);
    lcd_8080_write_reg(0x2c);
}

int lcd_init_param_5inch(void)
{

    unsigned int W, L;
    lcd_8080_write_reg(0xFF);
    lcd_8080_write_data(0xFF);
    lcd_8080_write_data(0x98);
    lcd_8080_write_data(0x06);

    lcd_8080_write_reg(0xBC);  //
    lcd_8080_write_data(0x03); //
    lcd_8080_write_data(0x0E); //
    lcd_8080_write_data(0x63); //
    lcd_8080_write_data(0x69); //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0x1B); //
    lcd_8080_write_data(0x10); //
    lcd_8080_write_data(0x6F); //
    lcd_8080_write_data(0x63); //
    lcd_8080_write_data(0xFF); //
    lcd_8080_write_data(0xFF); //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0xFF); //
    lcd_8080_write_data(0xF2); //
    lcd_8080_write_data(0xC1); //

    lcd_8080_write_reg(0xBD);  //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0x23); //
    lcd_8080_write_data(0x45); //
    lcd_8080_write_data(0x67); //
    lcd_8080_write_data(0x01); //
    lcd_8080_write_data(0x23); //
    lcd_8080_write_data(0x45); //
    lcd_8080_write_data(0x67); //

    lcd_8080_write_reg(0xBE);  //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x22); //
    lcd_8080_write_data(0x27); //
    lcd_8080_write_data(0x6A); //
    lcd_8080_write_data(0xBC); //
    lcd_8080_write_data(0xD8); //
    lcd_8080_write_data(0x92); //
    lcd_8080_write_data(0x22); //
    lcd_8080_write_data(0x22); //

    lcd_8080_write_reg(0xC7);  //	C7		 VCOM Control 1 (C7h) 	 ????????
    lcd_8080_write_data(0x25); //	25

    lcd_8080_write_reg(0xED);  //
    lcd_8080_write_data(0x7F); //
    lcd_8080_write_data(0x0F); //
    lcd_8080_write_data(0x00); //

    lcd_8080_write_reg(0xC0);  //
    lcd_8080_write_data(0x03); //
    lcd_8080_write_data(0x0B); //
    lcd_8080_write_data(0x02); //

    lcd_8080_write_reg(0xFC);  //
    lcd_8080_write_data(0x08); //

    lcd_8080_write_reg(0xDF);  //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x20); //

    lcd_8080_write_reg(0xF3);  //
    lcd_8080_write_data(0x74); //

    lcd_8080_write_reg(0xB4);  //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x00); //
    lcd_8080_write_data(0x00); //

    lcd_8080_write_reg(0xF7);  //
    lcd_8080_write_data(0x81); //	81

    lcd_8080_write_reg(0xB1);  //	B1
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x13); //	13
    lcd_8080_write_data(0x16); //	16

    lcd_8080_write_reg(0xF1);  //	F1	 Panel Timing Control 1
    lcd_8080_write_data(0x29); //	29
    lcd_8080_write_data(0x8A); //	8A
    lcd_8080_write_data(0x07); //	07

    lcd_8080_write_reg(0xF2);  //	F2	Panel Timing Control 2 (F2h)
    lcd_8080_write_data(0x40); //40
    lcd_8080_write_data(0xD2); //	D2
    lcd_8080_write_data(0x50); //	50
    lcd_8080_write_data(0x28); //	28

    lcd_8080_write_reg(0xC1);  //	C1	Power Control 2 (C1h)
    lcd_8080_write_data(0x17); //	17
    lcd_8080_write_data(0x50); //	50
    lcd_8080_write_data(0x50); //	50

    lcd_8080_write_reg(0x20); //	20		Display Inversion Off (20h)

    lcd_8080_write_reg(0xE0);  //	E0
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x0E); //	0E	p2
    lcd_8080_write_data(0x14); //	14	p3
    lcd_8080_write_data(0x0C); //	0C	p4
    lcd_8080_write_data(0x0E); //	0E	p5
    lcd_8080_write_data(0x0A); //	0A	p6
    lcd_8080_write_data(0x06); //	06	p7
    lcd_8080_write_data(0x03); //	03	p8
    lcd_8080_write_data(0x09); //	09	p9
    lcd_8080_write_data(0x0C); //	0C	p10
    lcd_8080_write_data(0x13); //	13	p11
    lcd_8080_write_data(0x10); //	10	p12
    lcd_8080_write_data(0x0F); //	0F	p13
    lcd_8080_write_data(0x14); //	14	p14
    lcd_8080_write_data(0x0B); //	0B	p15
    lcd_8080_write_data(0x00); //	00	p16

    lcd_8080_write_reg(0xE1);  //	E1
    lcd_8080_write_data(0x00); //	00	p1
    lcd_8080_write_data(0x08); //	08	p2
    lcd_8080_write_data(0x10); //	10	p3
    lcd_8080_write_data(0x0E); //	0E	p4
    lcd_8080_write_data(0x0F); //	0F	p5
    lcd_8080_write_data(0x0C); //	0C	p6
    lcd_8080_write_data(0x08); //	08	p7
    lcd_8080_write_data(0x05); //	05	p8
    lcd_8080_write_data(0x07); //	07	p9
    lcd_8080_write_data(0x0B); //	0B	p10
    lcd_8080_write_data(0x12); //	12	p11
    lcd_8080_write_data(0x10); //	10	p12
    lcd_8080_write_data(0x0E); //	0E	p13
    lcd_8080_write_data(0x17); //	17	p14
    lcd_8080_write_data(0x0F); //	0F	p15
    lcd_8080_write_data(0x00); //	00	p16

    lcd_8080_write_reg(0x3A);  //	3A		//????
    lcd_8080_write_data(0x55); //	55		16bit

    //lcd_8080_write_reg(0x36);//	36		Memory Access Control (36h)
    //lcd_8080_write_data(0x00);//	00

    //////倒装设置   NG
    // Set_address_mode  ---- 刷新方向

    lcd_8080_write_reg(0x36);
#if LCD_USE_HARD_DIR
    // 竖屏
    if (g_mlcd->dir == LCD_DIR_VERTICAL)
    {
        lcd_8080_write_data(0x0A);
    }
    else
    { // 横屏
        lcd_8080_write_data(0x28);
    }
#else
    lcd_8080_write_data(0x0A);
#endif

    lcd_8080_write_reg(0x35);  //	35		//Tearing Effect Line ON (35h)
    lcd_8080_write_data(0x00); //	00

    //while(1); //
    lcd_8080_write_reg(0x11); //	11   Sleep Out (11h).../Sleep In (10h) .
    delay_ms(100);            //
    //while(1); //
    lcd_8080_write_reg(0x29); //	29		//???
    delay_ms(100);            //

    //
    lcd_8080_write_reg(0x36); //
    lcd_8080_write_data(0x00 | (6 << 5));

    lcd_8080_write_reg(0x2A);  //	ILI9806G_Write_Cmd ( CMD_SetCoordinateX );
    lcd_8080_write_data(0x00); //	ILI9806G_Write_Data ( 0x00 );		/* x ?????8? */
    lcd_8080_write_data(0x00); //	ILI9806G_Write_Data ( 0x00 );		/* x ?????8? */
    lcd_8080_write_data(0x01); //	ILI9806G_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x ?????8? */
    lcd_8080_write_data(0xDF); //	ILI9806G_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x ?????8? */

    lcd_8080_write_reg(0x2B);  //	2B	//		Y????
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x03); //	03			Y????
    lcd_8080_write_data(0x55); //	55		0x0355?853 ?854??

    //	/* write gram start */
    lcd_8080_write_reg(0x2C); //	2C//	ILI9806G_Write_Cmd ( CMD_SetPixel );

    //????????
    lcd_8080_write_reg(0x2A);  //	2A	//Column Address Set (2Ah)
    lcd_8080_write_data(0x00); //	00				X????
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x01); //	01		   X????
    lcd_8080_write_data(0xDF); //	DF		//0x01DF=479 ?480??

    lcd_8080_write_reg(0x2B);  //	2B	//		Y????
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x03); //	03			Y????
    lcd_8080_write_data(0x55); //	55		0x0355?853 ?854??
    //................................................................................
    lcd_8080_write_reg(0x2C); //	2C		?LCD??,

    for (L = 0; L < 856; L++)
    { //??
        for (W = 0; W < 480; W++)
        {
            lcd_8080_write_data((0)); //	???8bit??,?8?
            lcd_8080_write_data(0);   //	00
        }
    }

    lcd_8080_write_reg(0x2A);  //	2A	//Column Address Set (2Ah)
    lcd_8080_write_data(0x00); //	00				X????
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x01); //	01		   X????
    lcd_8080_write_data(0xDF); //	DF		//0x01DF=479 ?480??

    lcd_8080_write_reg(0x2B);  //	2B	//		Y????
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x00); //	00
    lcd_8080_write_data(0x03); //	03			Y????
    lcd_8080_write_data(0x55); //	55		0x0355?853 ?854??
    //................................................................................
    lcd_8080_write_reg(0x2C); //	2C		?LCD??,
    return 0;
}

// mcu-------
// struct lcd_8080_config_t mcu_5inch =
//     {
//         0,
//         0,
//         856,
//         480,
//         0, //开始写gram指令
//         0, //设置x坐标指令
//         0, //设置y坐标指令
//         lcd_init_param_5inch
//     };

// rgb ------
// 4.3寸 800x480


struct zf_lcd_config_t lcd_rgb_4_3_inch_800x480 =
    {
        .type = LCD_TYPE_RGB,
        .id = 0x4384,
        .dir = 0,
        .pix_width = 800,
        .pix_height = 480,

        .rgb_config =
            {
                .hsw = 88,
                .hfp = 40,
                .hbp = 48,
                .vsw = 32,
                .vbp = 13,
                .vfp = 5}};

// 4.3寸 480x272
struct zf_lcd_config_t lcd_rgb_4_3_inch_480x272 =
    {
        .type =LCD_TYPE_RGB,
        .id = 0x4384,
        .dir = 0,
        .pix_width = 800,
        .pix_height = 480,

        .rgb_config =
            {
                .hsw = 1,
                .hfp = 1,
                .hbp = 40,
                .vsw = 8,
                .vbp = 5,
                .vfp = 8}};

// 5寸8080
struct zf_lcd_config_t lcd_8080_5inch_800x480 =
    {
        .type = LCD_TYPE_8080,
        .id = 0x4384,
        .dir = 1,
        .pix_width = 480 ,
        .pix_height =854 ,
        .m8080_config={
            .set_x_cmd=0x2a,
            .set_y_cmd=0x2b,
            .w_ram_cmd=0x2c
            }
        };


// oled
struct zf_lcd_config_t lcd_oled_128x64 =
    {
        .type = LCD_TYPE_OLED,
        .id = 0x4384,
        .dir = 0,
        .pix_width = 128,
        .pix_height =64,
        };


// spi
struct zf_lcd_config_t lcd_spi_1_3_240x240 =
    {
        .type = LCD_TYPE_SPI,
        .id = 0x4384,
        .dir = 0,
        .pix_width = 240,
        .pix_height = 240,
        };



/**
 * @brief 
 * 
 * @param type 
 * @param lcd 
 */
void lcd_config(uint16_t type, struct zf_lcd_t *lcd, struct zf_lcd_config_t **lcd_config)
{
    // rgb
    if (type == LCD_TYPE_RGB)
    {
        lcd->config(0, (void *)&lcd_rgb_4_3_inch_800x480);
        *lcd_config = &lcd_rgb_4_3_inch_800x480;
    }
    else if (type == LCD_TYPE_8080) // mcu8080
    {

        lcd->config(0, (void *)&lcd_8080_5inch_800x480);
        *lcd_config = &lcd_8080_5inch_800x480;
        lcd_init_param_5inch();
    }
    else if (type == LCD_TYPE_OLED) // spi lcd
    {
        lcd->config(0, (void *)&lcd_oled_128x64);
        *lcd_config = &lcd_oled_128x64;
    }
    else if (type == LCD_TYPE_SPI) // oled
    {
        lcd->config(0, (void *)&lcd_spi_1_3_240x240);
        *lcd_config = &lcd_spi_1_3_240x240;
    }
    else
    {
    }
}
