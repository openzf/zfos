#ifndef __ZF_HAL_DTS_H__
#define __ZF_HAL_DTS_H__


#include "stm32h743xx.h"


#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOB
#define LED_ON 0
#define LED_OFF 1

#define KEY0_Pin GPIO_PIN_0
#define KEY0_GPIO_Port GPIOA

#define FLASH_CS_Pin GPIO_PIN_12
#define FLASH_CS_GPIO_Port GPIOB

#define SIIC_SDA_PORT GPIOI
#define SIIC_SDA_PIN GPIO_PIN_3

#define SIIC_SCL_PORT GPIOH
#define SIIC_SCL_PIN GPIO_PIN_6

#define TOUCH_RESET_Pin GPIO_PIN_8
#define TOUCH_RESET_GPIO_Port GPIOI


#define LCD_SPI_RES_PORT GPIOG
#define LCD_SPI_RES_PIN GPIO_PIN_12

#define LCD_SPI_DC_PORT  GPIOG
#define LCD_SPI_DC_PIN  GPIO_PIN_13

#define LCD_SPI_BK_PORT  GPIOG
#define LCD_SPI_BK_PIN  GPIO_PIN_14

#define LCD_SPI_CS_PORT  GPIOG
#define LCD_SPI_CS_PIN  GPIO_PIN_14


// 软件spi
#define SSPI_SCL_PORT  GPIOG
#define SSPI_SCL_PIN  GPIO_PIN_10

#define SSPI_MOSI_PORT  GPIOG
#define SSPI_MOSI_PIN  GPIO_PIN_11

#define SSPI_MISO_PORT  GPIOG
#define SSPI_MISO_PIN  GPIO_PIN_12


#endif
