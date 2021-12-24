
#include "board.h"
// 设备树
#include "zf_dts.h"
// 杂项驱动
#include "user_misc_drv_def.h"
#include "../zf_hal/zf_hal_drv_manager.h"
// 依赖驱动
#include "gpio.h"




//---- i2c
static int si2c_gpio_init(uint32_t speed)
{

  GPIO_InitTypeDef GPIO_Initure;

  __HAL_RCC_GPIOH_CLK_ENABLE(); //开启GPIOH时钟
  __HAL_RCC_GPIOI_CLK_ENABLE(); //开启GPIOI时钟

  // SCL
  GPIO_Initure.Pin = SIIC_SCL_PIN;
  GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;     //推挽输出
  GPIO_Initure.Pull = GPIO_PULLUP;             //上拉
  GPIO_Initure.Speed = GPIO_SPEED_HIGH;        //高速
  HAL_GPIO_Init(SIIC_SCL_PORT, &GPIO_Initure); //初始化

  //SDA
  GPIO_Initure.Pin = SIIC_SDA_PIN;
  HAL_GPIO_Init(SIIC_SDA_PORT, &GPIO_Initure); //初始化
  return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int board_init(void)
{
   si2c_gpio_init(0);

    zf_hal_drv_manager_init();
   return 0;
}

