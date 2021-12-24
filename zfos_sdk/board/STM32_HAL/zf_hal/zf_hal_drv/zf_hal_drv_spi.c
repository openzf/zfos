/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_qspi.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_SPI
#include "spi.h"

static SPI_HandleTypeDef *g_spi_handle_group[ZF_SPI_MAX_NUM] =
    {
        &ZF_SPI0,
        &ZF_SPI1,
        &ZF_SPI2};

//--------- spi----
/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int spi_hw_init(uint8_t ch)
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
static int spi_hw_get(uint8_t ch, uint8_t cmd)
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
static int spi_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int spi_hw_close(uint8_t ch)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param speed 
 * @return int 
 */
static int spi_config(uint8_t ch, uint32_t speed)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param data 
 * @param length 
 * @return int 
 */
static int spi_write(uint8_t ch, uint8_t *data, uint16_t length)
{
  SPI_HandleTypeDef *mspi = g_spi_handle_group[ch];

  HAL_SPI_Transmit(mspi, data, length, 2000);
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param data 
 * @param length 
 * @return int 
 */
static int spi_read(uint8_t ch, uint8_t *data, uint16_t length)
{
  SPI_HandleTypeDef *mspi = g_spi_handle_group[ch];
  HAL_SPI_Receive(mspi, data, length, 2000);
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param send_data 
 * @param send_len 
 * @param read_data 
 * @param read_len 
 * @return int 
 */
static int spi_write_read(uint8_t ch, uint8_t *send_data, uint16_t send_len, uint8_t *read_data, uint16_t read_len)
{
  SPI_HandleTypeDef *mspi = g_spi_handle_group[ch];
  HAL_SPI_TransmitReceive(mspi, send_data, read_data, send_len, 2000);
  return 0;
}

// spi
struct zf_spi_t zf_spi =
    {
        .hw = {
            .init = spi_hw_init,
            .get = spi_hw_get,
            .set = spi_hw_set,
            .close = spi_hw_close},
        .config = spi_config,
        .read = spi_read,
        .write = spi_write,
        .write_read = spi_write_read};

/**
 * @brief 
 * 
 */
void zf_hal_drv_spi_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_SPI, &zf_spi);
}
#endif
