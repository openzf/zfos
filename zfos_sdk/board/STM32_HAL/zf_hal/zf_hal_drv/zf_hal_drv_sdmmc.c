/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_sdmmc.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_SDMMC
#include "sdmmc.h"
#include "../user_misc_drv/user_driver_sd.h"

#define SD_TIMEOUT 30 * 1000
#define SD_DEFAULT_BLOCK_SIZE 512
static volatile uint32_t WriteStatus = 0, ReadStatus = 0;

void BSP_SD_WriteCpltCallback(void)
{

  WriteStatus = 1;
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void BSP_SD_ReadCpltCallback(void)
{
  ReadStatus = 1;
}

// -- api
/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int sdmmc_hw_init(uint8_t ch)
{
  BSP_SD_Init();
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @return int 
 */
static int sdmmc_hw_get(uint8_t ch, uint8_t cmd)
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
static int sdmmc_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int sdmmc_hw_close(uint8_t ch)
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
static int sdmmc_config(uint8_t ch, uint32_t speed)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param card_type 
 * @param block_size 
 * @param bloack_num 
 * @param size 
 * @return int 
 */
static int sdmmc_get_info(uint8_t ch, uint8_t *card_type, uint16_t *block_size, uint32_t *bloack_num, unsigned long long *size)
{

  BSP_SD_CardInfo CardInfo;
  BSP_SD_GetCardInfo(&CardInfo);

  *size = ((unsigned long long)hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr);
  *bloack_num = CardInfo.LogBlockNbr;
  *block_size = CardInfo.LogBlockSize;
  *card_type = hsd1.SdCard.CardType;

  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int sdmmc_erase_all(uint8_t ch)
{

  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param addr 
 * @param size 
 * @return int 
 */
static int sdmmc_erase_range(uint8_t ch, uint32_t addr, uint32_t size)
{

  return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param sector 
 * @param buff 
 * @param count 
 * @return int 
 */
static int sdmmc_read(uint8_t ch, uint32_t sector, uint8_t *buff, uint32_t count)
{

  int res = 0;
  uint32_t timeout;

  if (BSP_SD_ReadBlocks_DMA((uint32_t *)buff,
                            (uint32_t)(sector),
                            count) == MSD_OK)
  {
    ReadStatus = 0;
    /* Wait that the reading process is completed or a timeout occurs */
    timeout = HAL_GetTick();
    while ((ReadStatus == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
    {
    }
    /* incase of a timeout return error */
    if (ReadStatus == 0)
    {
      res = 1;
    }
    else
    {
      ReadStatus = 0;
      timeout = HAL_GetTick();

      while ((HAL_GetTick() - timeout) < SD_TIMEOUT)
      {
        if (BSP_SD_GetCardState() == SD_TRANSFER_OK)
        {
          res = 0;
          break;
        }
      }
    }
  }

  return res;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param sector 
 * @param buff 
 * @param count 
 * @return int 
 */
static int sdmmc_write(uint8_t ch, uint32_t sector, uint8_t *buff, uint32_t count)
{
  int res = 0;
  uint32_t timeout;
#if defined(ENABLE_SCRATCH_BUFFER)
  uint8_t ret;
  int i;
#endif

  WriteStatus = 0;
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
  uint32_t alignedAddr;
#endif

  //  if (SD_CheckStatusWithTimeout(SD_TIMEOUT) < 0)
  //  {
  //    return res;
  //  }

#if defined(ENABLE_SCRATCH_BUFFER)
  if (!((uint32_t)buff & 0x3))
  {
#endif
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)

    /*
    the SCB_CleanDCache_by_Addr() requires a 32-Byte aligned address
    adjust the address and the D-Cache size to clean accordingly.
    */
    alignedAddr = (uint32_t)buff & ~0x1F;
    SCB_CleanDCache_by_Addr((uint32_t *)alignedAddr, count * BLOCKSIZE + ((uint32_t)buff - alignedAddr));
#endif

    if (BSP_SD_WriteBlocks_DMA((uint32_t *)buff,
                               (uint32_t)(sector),
                               count) == MSD_OK)
    {
      /* Wait that writing process is completed or a timeout occurs */

      timeout = HAL_GetTick();
      while ((WriteStatus == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
      {
      }
      /* incase of a timeout return error */
      if (WriteStatus == 0)
      {
        res = 1;
      }
      else
      {
        WriteStatus = 0;
        timeout = HAL_GetTick();

        while ((HAL_GetTick() - timeout) < SD_TIMEOUT)
        {
          if (BSP_SD_GetCardState() == SD_TRANSFER_OK)
          {
            res = 0;
            break;
          }
        }
      }
    }
#if defined(ENABLE_SCRATCH_BUFFER)
  }
  else
  {
    /* Slow path, fetch each sector a part and memcpy to destination buffer */
#if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
    /*
      * invalidate the scratch buffer before the next write to get the actual data instead of the cached one
      */
    SCB_InvalidateDCache_by_Addr((uint32_t *)scratch, BLOCKSIZE);
#endif

    for (i = 0; i < count; i++)
    {
      WriteStatus = 0;

      memcpy((void *)scratch, (void *)buff, BLOCKSIZE);
      buff += BLOCKSIZE;

      ret = BSP_SD_WriteBlocks_DMA((uint32_t *)scratch, (uint32_t)sector++, 1);
      if (ret == MSD_OK)
      {
        /* wait for a message from the queue or a timeout */
        timeout = HAL_GetTick();
        while ((WriteStatus == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
        {
        }
        if (WriteStatus == 0)
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
    if ((i == count) && (ret == MSD_OK))
      res = RES_OK;
  }
#endif
  return res;
}

struct zf_sdmmc_t zf_sdmmc =
    {
        .hw = {
            .init = sdmmc_hw_init,
            .get = sdmmc_hw_get,
            .set = sdmmc_hw_set,
            .close = sdmmc_hw_close},
        .config = sdmmc_config,
        .get_info = sdmmc_get_info,
        .erase_all = sdmmc_erase_all,
        .erase_range = sdmmc_erase_range,
        .read = sdmmc_read,
        .write = sdmmc_write};

/**
 * @brief 
 * 
 */
void zf_hal_drv_sdmmc_init(void)
{
  zf_driver_register(ZF_DRIVER_TYPE_SDMMC, &zf_sdmmc);
}
#endif
