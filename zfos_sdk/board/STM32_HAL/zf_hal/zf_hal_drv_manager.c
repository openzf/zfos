#include "zf_hal_drv_manager.h"

#include "./zf_hal_drv/zf_hal_drv_def.h"
#include "./user_misc_drv/user_misc_drv_def.h"

static void test(void)
{
        
}

static void (*zf_hal_drv_init_groups[])(void) =
    {
        test,
#if USE_BSP_GPIO
        zf_hal_drv_gpio_init,
#endif

#if USE_BSP_DELAY
        zf_hal_drv_delay_init,
#endif
#if USE_BSP_UART
        zf_hal_drv_uart_init,
#endif
#if USE_BSP_I2C
        zf_hal_drv_i2c_init,
#endif
#if USE_BSP_SPI
        zf_hal_drv_spi_init,
#endif
#if USE_BSP_QSPI
        zf_hal_drv_qspi_init,
#endif
#if USE_BSP_SDMMC
        zf_hal_drv_sdmmc_init,
#endif
#if USE_BSP_LCD_RGB
        zf_hal_drv_lcd_rgb_init,
#endif

#if USE_BSP_LCD_8080
    zf_hal_drv_lcd_8080_init
#endif

};


static void (*misc_drv_init_groups[])(void) =
{
    test
};

/**
 * @brief 
 * 
 */
void user_misc_init(void)
{
  int i = 0;
  for (i = 0; i < sizeof(misc_drv_init_groups) / 4; i++)
  {
    (
        misc_drv_init_groups[i])();
  }
}

/**
 * @brief 
 * 
 */
void zf_hal_drv_manager_init(void)
{

  int i = 0;
  for (i = 0; i < sizeof(zf_hal_drv_init_groups) / 4; i++)
  {
    (zf_hal_drv_init_groups[i])();
  }
}
