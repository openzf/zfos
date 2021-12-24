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
#if USE_BSP_QSPI
#include "quadspi.h"

static QSPI_HandleTypeDef *g_qspi_handle_group[ZF_QSPI_MAX_NUM] =
    {
        &ZF_QSPI0,
        &ZF_QSPI1,
        &ZF_QSPI2};

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int qspi_hw_init(uint8_t ch)
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
static int qspi_hw_get(uint8_t ch, uint8_t cmd)
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
static int qspi_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int qspi_hw_close(uint8_t ch)
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
static int qspi_config(uint8_t ch, uint32_t speed)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param buff 
 * @param datalen 
 * @return int 
 */
static int qspi_read(uint8_t ch, uint8_t *buff, uint32_t datalen)
{
  QSPI_HandleTypeDef *mqspi = g_qspi_handle_group[ch];
  mqspi->Instance->DLR = datalen - 1; //配置数据长度
  if (HAL_QSPI_Receive(mqspi, buff, 5000) == HAL_OK)
  {
    return 0; //接收数据
  }

  return 1;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param buff 
 * @param datalen 
 * @return int 
 */
static int qspi_write(uint8_t ch, uint8_t *buff, uint32_t datalen)
{
  QSPI_HandleTypeDef *mqspi = g_qspi_handle_group[ch];
  mqspi->Instance->DLR = datalen - 1; //配置数据长度
  if (HAL_QSPI_Transmit(mqspi, buff, 5000) == HAL_OK)
  {
    return 0; //发送数据
  }
  return 1;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param instruction 
 * @param address 
 * @param dummyCycles 
 * @param instructionMode 
 * @param addressMode 
 * @param addressSize 
 * @param dataMode 
 * @return int 
 */
static int qspi_write_cmd(uint8_t ch, uint32_t instruction, uint32_t address, uint32_t dummyCycles, uint32_t instructionMode, uint32_t addressMode, uint32_t addressSize, uint32_t dataMode)
{
  QSPI_CommandTypeDef Cmdhandler;

  uint32_t minstructionMode = instructionMode;
  uint32_t maddressMode = addressMode;
  uint32_t maddressSize = addressSize;
  uint32_t mdataMode = dataMode;
  QSPI_HandleTypeDef *mqspi = g_qspi_handle_group[ch];

  switch (instructionMode)
  {
  case 0:
    minstructionMode = QSPI_INSTRUCTION_NONE;
    break;
  case 1:
    minstructionMode = QSPI_INSTRUCTION_1_LINE;
    break;
  case 2:
    minstructionMode = QSPI_INSTRUCTION_2_LINES;
    break;
  case 3:
    minstructionMode = QSPI_INSTRUCTION_4_LINES;
    break;
  default:
    break;
  }

  switch (addressMode)
  {
  case 0:
    maddressMode = QSPI_ADDRESS_NONE;
    break;
  case 1:
    maddressMode = QSPI_ADDRESS_1_LINE;
    break;
  case 2:
    maddressMode = QSPI_ADDRESS_2_LINES;
    break;
  case 3:
    maddressMode = QSPI_ADDRESS_4_LINES;
    break;
  default:
    break;
  }

  switch (addressSize)
  {
  case 0:
    maddressSize = QSPI_ADDRESS_8_BITS;
    break;
  case 1:
    maddressSize = QSPI_ADDRESS_16_BITS;
    break;
  case 2:
    maddressSize = QSPI_ADDRESS_24_BITS;
    break;
  case 3:
    maddressSize = QSPI_ADDRESS_32_BITS;
    break;
  default:
    break;
  }

  switch (dataMode)
  {
  case 0:
    mdataMode = QSPI_DATA_NONE;
    break;
  case 1:
    mdataMode = QSPI_DATA_1_LINE;
    break;
  case 2:
    mdataMode = QSPI_DATA_2_LINES;
    break;
  case 3:
    mdataMode = QSPI_DATA_4_LINES;
    break;
  default:
    break;
  }

  Cmdhandler.Instruction = instruction; //指令
  Cmdhandler.Address = address;         //地址
  Cmdhandler.DummyCycles = dummyCycles; //设置空指令周期数

  Cmdhandler.InstructionMode = minstructionMode; //指令模式
  Cmdhandler.AddressMode = maddressMode;         //地址模式
  Cmdhandler.AddressSize = maddressSize;         //地址长度
  Cmdhandler.DataMode = mdataMode;               //数据模式

  Cmdhandler.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;           //每次都发送指令
  Cmdhandler.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; //无交替字节
  Cmdhandler.DdrMode = QSPI_DDR_MODE_DISABLE;               //关闭DDR模式
  Cmdhandler.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;

  HAL_QSPI_Command(mqspi, &Cmdhandler, 5000);
  return 0;
}

// spi
struct zf_qspi_t zf_qspi =
    {
        .hw = {
            .init = qspi_hw_init,
            .get = qspi_hw_get,
            .set = qspi_hw_set,
            .close = qspi_hw_close},
        .config = qspi_config,
        .read = qspi_read,
        .write = qspi_write,
        .write_cmd = qspi_write_cmd};

/**
 * @brief 
 * 
 */
void zf_hal_drv_qspi_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_QSPI, &zf_qspi);
}

#endif
