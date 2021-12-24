/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_delay.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_DELAY
#include "main.h"
// 主频
uint16_t SYSCLK = 400;
static uint32_t fac_us = 0; //us延时倍乘数

/**
 * @brief hw_init
 * 
 * @param ch 
 * @return int 
 */
static int delay_hw_init(uint8_t ch)
{
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTick频率为HCLK
  fac_us = SYSCLK;
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @return int 
 */
static int delay_hw_get(uint8_t ch, uint8_t cmd)
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
static int delay_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int delay_hw_close(uint8_t ch)
{
  return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
static int mdelay_config(void)
{

  return 0;
}

/**
 * @brief 
 * 
 * @param time 
 */
static void mdelay_us(uint32_t time)
{
  uint32_t ticks;
  uint32_t told, tnow, tcnt = 0;
  uint32_t reload = SysTick->LOAD; //LOAD的值
  ticks = time * fac_us;           //需要的节拍数
  told = SysTick->VAL;             //刚进入时的计数器值
  while (1)
  {
    tnow = SysTick->VAL;
    if (tnow != told)
    {
      if (tnow < told)
        tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
      else
        tcnt += reload - tnow + told;
      told = tnow;
      if (tcnt >= ticks)
        break; //时间超过/等于要延迟的时间,则退出.
    }
  };
}

/**
 * @brief 
 * 
 * @param time 
 */
static void mdelay_ms(uint32_t time)
{

  uint32_t i;
  for (i = 0; i < time; i++)
    mdelay_us(1000);
}

/**
 * @brief 
 * 
 * @param time 
 */
static void mdelay_s(uint32_t time)
{

}

/**
 * @brief 
 * 
 */
static struct zf_delay_t zf_delay =
    {
        .hw = {
            .init = delay_hw_init,
            .get = delay_hw_get,
            .set = delay_hw_set,
            .close = delay_hw_close},
        .config = mdelay_config,
        .delay_us = mdelay_us,
        .delay_ms = mdelay_ms,
        .delay_s = mdelay_s};

/**
 * @brief 
 * 
 */
void zf_hal_drv_delay_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_DELAY, &zf_delay);
}
#endif
