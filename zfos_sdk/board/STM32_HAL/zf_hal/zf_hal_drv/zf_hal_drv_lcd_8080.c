/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_lcd_8080.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_LCD_8080
//--lcd mcu8080
//配置MPU的region(SRAM区域为透写模式)

#if USE_8080_16BIT
// 16 bit
#define FSMC_LCD_BASE_ADDR ((uint32_t)(0x60000000 | ((1 << 20) - 2)))
// A19作为数据命令区分线
#else
#define FSMC_LCD_BASE_ADDR ((uint32_t)(0x60000000 | ((1 << 20) - 2) / 2))
#endif

/**
 * @brief 
 * 
 */
void LCD_MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_Initure;

  HAL_MPU_Disable(); //配置MPU之前先关闭MPU,配置完成以后在使能MPU
  //外部SRAM为region0，大小为2MB，此区域可读写
  MPU_Initure.Enable = MPU_REGION_ENABLE;   //使能region
  MPU_Initure.Number = MPU_REGION_NUMBER0;  //设置region，外部SRAM使用的region0
  MPU_Initure.BaseAddress = 0X60000000;     //region基地址
  MPU_Initure.Size = MPU_REGION_SIZE_256MB; //region大小
  MPU_Initure.SubRegionDisable = 0X00;
  MPU_Initure.TypeExtField = MPU_TEX_LEVEL0;
  MPU_Initure.AccessPermission = MPU_REGION_FULL_ACCESS;   //此region可读写
  MPU_Initure.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE; //允许读取此区域中的指令
  MPU_Initure.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_Initure.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_Initure.IsBufferable = MPU_ACCESS_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_Initure);
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT); //开启MPU
}

/**
 * @brief 
 * 
 */
void dev_lcd_reset(void)
{

  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(50);
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int lcd_8080_hw_init(uint8_t ch)
{
  LCD_MPU_Config();
  dev_lcd_reset();
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @return int 
 */
static int lcd_8080_hw_get(uint8_t ch, uint8_t cmd)
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
static int lcd_8080_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
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
static int lcd_8080_config(uint8_t ch, void *config)
{
  return 0;
}
/**
 * @brief 
 * 
 * @param ch 
 * @param addr 
 * @return int 
 */
static int lcd_8080_get_addr(uint8_t ch, uint32_t *addr)
{
  *addr = FSMC_LCD_BASE_ADDR;
  return 0;
}

struct zf_lcd_8080_t zf_lcd_8080 =
    {
        .hw = {
            .init = lcd_8080_hw_init,
            .get = lcd_8080_hw_get,
            .set = lcd_8080_hw_set,
            .close = lcd_8080_hw_close},
        .config = lcd_8080_config,
        .get_gram_addr = lcd_8080_get_addr

};

/**
 * @brief 
 * 
 */
void zf_hal_drv_lcd_8080_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_LCD_8080, &zf_lcd_8080);
}

#endif
