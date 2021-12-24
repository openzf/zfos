/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_rgb.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_LCD_RGB
#include "ltdc.h"
//定义最大屏分辨率时,LCD所需的帧缓存数组大小
#define RGB_LCD_BASE_ADDR 0XC0000000
#define RGB_LCD_PIXELS_BYTE 2
#define LCD_DIR_HORIZONTAL 0
#define LCD_DIR_VERTICAL 1



struct zf_lcd_config_t g_lcd_rgb_config;
uint16_t g_ltdc_framebuf[1280][800] __attribute__((at(RGB_LCD_BASE_ADDR)));

/**
 * @brief 
 * 
 * @param m_lcd_rgb_config 
 */
static void lcd_config_sequence(struct zf_lcd_config_t m_lcd_rgb_config)
{
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;                                                                                   //水平同步极性
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;                                                                                   //垂直同步极性
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;                                                                                   //数据使能极性
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;                                                                                  //像素时钟极性
  hltdc.Init.HorizontalSync = m_lcd_rgb_config.rgb_config.hsw - 1;                                                                         //水平同步宽度
  hltdc.Init.VerticalSync = m_lcd_rgb_config.rgb_config.vsw - 1;                                                                           //垂直同步宽度
  hltdc.Init.AccumulatedHBP = m_lcd_rgb_config.rgb_config.hsw + m_lcd_rgb_config.rgb_config.hbp - 1;                                                  //水平同步后沿宽度
  hltdc.Init.AccumulatedVBP = m_lcd_rgb_config.rgb_config.vsw + m_lcd_rgb_config.rgb_config.vbp - 1;                                                  //垂直同步后沿高度
  hltdc.Init.AccumulatedActiveW = m_lcd_rgb_config.rgb_config.hsw + m_lcd_rgb_config.rgb_config.hbp +  m_lcd_rgb_config.pix_width - 1;                 //有效宽度
  hltdc.Init.AccumulatedActiveH = m_lcd_rgb_config.rgb_config.vsw + m_lcd_rgb_config.rgb_config.vbp +  m_lcd_rgb_config.pix_height - 1;                //有效高度
  hltdc.Init.TotalWidth = m_lcd_rgb_config.rgb_config.hsw + m_lcd_rgb_config.rgb_config.hbp +  m_lcd_rgb_config.pix_width + m_lcd_rgb_config.rgb_config.hfp - 1;  //总宽度
  hltdc.Init.TotalHeigh = m_lcd_rgb_config.rgb_config.vsw + m_lcd_rgb_config.rgb_config.vbp +  m_lcd_rgb_config.pix_height + m_lcd_rgb_config.rgb_config.vfp - 1; //总高度
  hltdc.Init.Backcolor.Red = 0;                                                                                                 //屏幕背景层红色部分
  hltdc.Init.Backcolor.Green = 0;                                                                                               //屏幕背景层绿色部分
  hltdc.Init.Backcolor.Blue = 0;                                                                                                //屏幕背景色蓝色部分
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief 
 * 
 * @param m_lcd_rgb_config 
 * @param layer_addr 
 * @param layer 
 */
static void lcd_config_layer(struct zf_lcd_config_t m_lcd_rgb_config, uint32_t layer_addr, uint8_t layer)
{
  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  // LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 =  m_lcd_rgb_config.pix_width;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 =  m_lcd_rgb_config.pix_height;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = layer_addr;
  pLayerCfg.ImageWidth =  m_lcd_rgb_config.pix_width;
  pLayerCfg.ImageHeight =  m_lcd_rgb_config.pix_height;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, layer) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int lcd_rgb_hw_init(uint8_t ch)
{
  HAL_GPIO_WritePin(LCD_BK_GPIO_Port, LCD_BK_Pin, 1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @return int 
 */
static int lcd_rgb_hw_get(uint8_t ch, uint8_t cmd)
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
static int lcd_rgb_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int lcd_rgb_hw_close(uint8_t ch)
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
static int lcd_rgb_config(uint8_t ch, void *config)
{
  g_lcd_rgb_config = *((struct zf_lcd_config_t *)config);

  printf("config init :%d \r\n", g_lcd_rgb_config.pix_height);
  // 时序
  lcd_config_sequence(g_lcd_rgb_config);
  // 层
  lcd_config_layer(g_lcd_rgb_config, RGB_LCD_BASE_ADDR, 0);
  lcd_config_layer(g_lcd_rgb_config, RGB_LCD_BASE_ADDR, 1);

  //使能DMA2D时钟
  __HAL_RCC_DMA2D_CLK_ENABLE();
  __HAL_LTDC_LAYER_ENABLE(&hltdc, 0);
  __HAL_LTDC_LAYER_DISABLE(&hltdc, 1);
  __HAL_LTDC_RELOAD_CONFIG(&hltdc);
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
static int lcd_rgb_draw_point(uint8_t ch, uint16_t x, uint16_t y, uint32_t color)
{

  //坐标系转换
  if (g_lcd_rgb_config.dir == LCD_DIR_HORIZONTAL) //横屏
  {
    *(uint16_t *)((uint32_t)RGB_LCD_BASE_ADDR + RGB_LCD_PIXELS_BYTE * (g_lcd_rgb_config.pix_width * y + x)) = color;
  }
  else //竖屏
  {
    *(uint16_t *)((uint32_t)RGB_LCD_BASE_ADDR + RGB_LCD_PIXELS_BYTE * (g_lcd_rgb_config.pix_width * (g_lcd_rgb_config.pix_height - x - 1) + y)) = color;
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
static int lcd_rgb_fill(uint8_t ch, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
  uint32_t psx, psy, pex, pey; //以LCD面板为基准的坐标系,不随横竖屏变化而变化

  uint16_t offline;
  uint32_t addr;

  //坐标系转换
  if (g_lcd_rgb_config.dir == LCD_DIR_HORIZONTAL) //横屏
  {
    psx = sx;
    psy = sy;
    pex = ex;
    pey = ey;
  }
  else //竖屏
  {
    psx = sy;
    psy = g_lcd_rgb_config.pix_height - ex - 1;
    pex = ey;
    pey = g_lcd_rgb_config.pix_height - sx - 1;
  }
  offline = g_lcd_rgb_config.pix_width - (pex - psx + 1);
  addr = (RGB_LCD_BASE_ADDR + RGB_LCD_PIXELS_BYTE * (g_lcd_rgb_config.pix_width * psy + psx));

  __HAL_RCC_DMA2D_CLK_ENABLE(); //使能DM2D时钟
  //DMA2D->CR&=~(DMA2D_CR_START);	//先停止DMA2D

  DMA2D->CR = DMA2D_R2M; //寄存器到存储器模式
  // 设置 DMA2D 采用寄存器往存储器传输数据模式，即 DMA2D 将 OCOLR 寄存器设置颜色值填充到存储器里面。
  DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_RGB565; //设置颜色格式

  DMA2D->OOR = offline; //设置行偏移

  DMA2D->OMAR = addr;                                     //输出存储器地址
  DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); //设定行数寄存器
  DMA2D->OCOLR = color;                                   //设定输出颜色寄存器

  DMA2D->CR |= DMA2D_CR_START;                //启动DMA2D
  while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0) //等待传输完成
  {
  }
  DMA2D->IFCR |= DMA2D_FLAG_TC; //清除传输完成标志
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
static int lcd_rgb_color_fill(uint8_t ch, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
  uint32_t psx, psy, pex, pey; //以LCD面板为基准的坐标系,不随横竖屏变化而变化

  uint16_t offline;
  uint32_t addr;

  //坐标系转换
  if (g_lcd_rgb_config.dir == LCD_DIR_HORIZONTAL) //横屏
  {
    psx = sx;
    psy = sy;
    pex = ex;
    pey = ey;
  }
  else //竖屏
  {
    psx = sy;
    psy = g_lcd_rgb_config.pix_height - ex - 1;
    pex = ey;
    pey = g_lcd_rgb_config.pix_height - sx - 1;
  }

  offline = g_lcd_rgb_config.pix_width - (pex - psx + 1);
  addr = (RGB_LCD_BASE_ADDR + RGB_LCD_PIXELS_BYTE * (g_lcd_rgb_config.pix_width * psy + psx));

  __HAL_RCC_DMA2D_CLK_ENABLE(); //使能DM2D时钟
  //DMA2D->CR&=~(DMA2D_CR_START);	//先停止DMA2D

  DMA2D->CR = DMA2D_M2M; //存储器到存储器模式

  DMA2D->FGMAR = (uint32_t)color; //源地址
  DMA2D->OMAR = addr;             //输出存储器地址

  DMA2D->FGOR = 0;      //前景层行偏移为0
  DMA2D->OOR = offline; //设置行偏移

  DMA2D->FGPFCCR = 0X02; //设置颜色格式
  DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_RGB565;

  DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); //设定行数寄存器

  DMA2D->CR |= DMA2D_CR_START;                //启动DMA2D
  while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0) //等待传输完成
  {
  }
  DMA2D->IFCR |= DMA2D_FLAG_TC; //清除传输完成标志
  return 0;
}

  int lcd_rgb_flush(uint8_t ch)
  {
    return 0;
  }

static struct zf_lcd_t zf_lcd_rgb =
    {
        .hw = {
            .init = lcd_rgb_hw_init,
            .get = lcd_rgb_hw_get,
            .set = lcd_rgb_hw_set,
            .close = lcd_rgb_hw_close},
        .config = lcd_rgb_config,
        .draw_point = lcd_rgb_draw_point,
        .fill = lcd_rgb_fill,
        .color_fill = lcd_rgb_color_fill,
        .flush =  lcd_rgb_flush
};

/**
 * @brief 
 * 
 */
void zf_hal_drv_lcd_rgb_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_LCD_RGB, &zf_lcd_rgb);
}

#endif
