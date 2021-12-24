#ifndef __ZF_HAL_BOARD_CONF_H__
#define __ZF_HAL_BOARD_CONF_H__

#include "stm32h7xx_hal.h"

//  已经使用的驱动
// basic
#define USE_BSP_GPIO  1
#define USE_BSP_UART  1
#define USE_BSP_DELAY 1
#define USE_DRIVER_SD 1

#define USE_BSP_SPI   1
#define USE_BSP_QSPI  1
#define USE_BSP_I2C   1
#define USE_BSP_SDMMC 1
#define USE_BSP_LCD_RGB 1
#define USE_BSP_LCD_8080 0

// uart
#define ZF_UART0 huart1
#define ZF_UART1 huart1
#define ZF_UART2 huart1
#define ZF_UART_MAX_NUM  4

// spi
#define ZF_SPI0 hspi2
#define ZF_SPI1 hspi4
#define ZF_SPI2 hspi2
#define ZF_SPI_MAX_NUM  4

// i2c
#define ZF_I2C0 hi2c1
#define ZF_I2C1 hi2c1
#define ZF_I2C2 hi2c1
#define ZF_I2C_MAX_NUM  4

// qspi
#define ZF_QSPI0 hqspi
#define ZF_QSPI1 hqspi
#define ZF_QSPI2 hqspi
#define ZF_QSPI_MAX_NUM  4

#endif
