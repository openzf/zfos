/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_uart.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_UART
#include "usart.h"

static UART_HandleTypeDef *g_uart_handle_group[ZF_UART_MAX_NUM] =
    {
        &ZF_UART0,
        &ZF_UART1,
        &ZF_UART2,
};

//-------- uart--------
static int uart_set_rec_callback(uint8_t ch, int (*arg_callback)(uint8_t *data, uint16_t len))
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int uart_hw_init(uint8_t ch)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @return int 
 */
static int uart_hw_get(uint8_t ch, uint8_t cmd)
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
static int uart_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int uart_hw_close(uint8_t ch)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param baud 
 * @param data 
 * @param stop 
 * @return int 
 */
static int uart_config(uint8_t ch, uint32_t baud, uint8_t data, uint8_t stop)
{
  UART_HandleTypeDef *muart = g_uart_handle_group[ch];
  muart->Init.BaudRate = baud;
  HAL_UART_Init(muart);
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param data 
 * @param len 
 * @return int 
 */
static int uart_write(uint8_t ch, uint8_t *data, uint16_t len)
{
  UART_HandleTypeDef *muart = g_uart_handle_group[ch];
  HAL_UART_Transmit(muart, data, len, 100);
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param data 
 * @param len 
 * @return int 
 */
static int uart_read(uint8_t ch, uint8_t *data, uint16_t len)
{
  UART_HandleTypeDef *muart = g_uart_handle_group[ch];
  if (HAL_UART_Receive(muart, data, len, 100) == HAL_OK)
  {
    return 0;
  }

  return -1;
}

// uart
struct zf_uart_t zf_uart =
    {

        .hw = {
            .init = uart_hw_init,
            .get = uart_hw_get,
            .set = uart_hw_set,
            .close = uart_hw_close},

        .config = uart_config,
        .read = uart_read,
        .write = uart_write,
};

/**
 * @brief 
 * 
 */
void zf_hal_drv_uart_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_UART, &zf_uart);
}
#endif
