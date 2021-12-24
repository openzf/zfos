#ifndef __ZF_HAL_DRV_DEF_H_
#define __ZF_HAL_DRV_DEF_H__

#include "zf_driver.h"

#include "zf_hal_conf.h"
#include "zf_dts.h"

// 每个驱动模块提供的入口函数
void zf_hal_drv_gpio_init(void);
void zf_hal_drv_delay_init(void);
void zf_hal_drv_uart_init(void);
void zf_hal_drv_i2c_init(void);
void zf_hal_drv_spi_init(void);
void zf_hal_drv_qspi_init(void);
void zf_hal_drv_sdmmc_init(void);
void zf_hal_drv_lcd_rgb_init(void);
void zf_hal_drv_lcd_8080_init(void);
#endif
