/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_gpio.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_GPIO
#include "gpio.h"

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int gpio_hw_init(uint8_t ch)
{
  MX_GPIO_Init();
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @return int 
 */
static int gpio_hw_get(uint8_t ch, uint8_t cmd)
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
static int gpio_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int gpio_hw_close(uint8_t ch)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param pin_port 
 * @param pin 
 * @param mode 
 * @return int 
 */
static int gpio_config(void *pin_port, uint16_t pin, uint16_t mode)
{

  // input
  if (mode == 0)
  {
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;                    //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;                        //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;                   //高速
    HAL_GPIO_Init((GPIO_TypeDef *)pin_port, &GPIO_Initure); //初始化
  }
  else
  {
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;                        //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;                   //高速
    HAL_GPIO_Init((GPIO_TypeDef *)pin_port, &GPIO_Initure); //初始化
  }

  return 0;
}

/**
 * @brief 
 * 
 * @param pin_port 
 * @param pin 
 * @return int 
 */
static int gpio_read(void *pin_port, uint16_t pin)
{
  return HAL_GPIO_ReadPin(pin_port, pin);
}

/**
 * @brief 
 * 
 * @param pin_port 
 * @param pin 
 * @param value 
 * @return int 
 */
static int gpio_write(void *pin_port, uint16_t pin, uint16_t value)
{
  HAL_GPIO_WritePin((GPIO_TypeDef *)pin_port, pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
  return 0;
}


static struct zf_gpio_t zf_gpio =
    {
        .hw = {
            .init = gpio_hw_init,
            .get = gpio_hw_get,
            .set = gpio_hw_set,
            .close = gpio_hw_close},

        .config = gpio_config,
        .read = gpio_read,
        .write = gpio_write,
};

/**
 * @brief 
 * 
 */
void zf_hal_drv_gpio_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_GPIO, &zf_gpio);
}

#endif
