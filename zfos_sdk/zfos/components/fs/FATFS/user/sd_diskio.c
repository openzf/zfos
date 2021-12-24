
#include "../zf_driver.h"

#define SDMMC_CH 0
static struct zf_sdmmc_t *g_sdmmc;

#include "../src/ff_gen_drv.h"
#include "sd_diskio.h"


#include <string.h>

 
#define SD_TIMEOUT 30 * 1000
#define SD_DEFAULT_BLOCK_SIZE 512



/* Private variables ---------------------------------------------------------*/
#if defined(ENABLE_SCRATCH_BUFFER)
#if defined (ENABLE_SD_DMA_CACHE_MAINTENANCE)
ALIGN_32BYTES(static uint8_t scratch[BLOCKSIZE]); // 32-Byte aligned for cache maintenance
#else
__ALIGN_BEGIN static uint8_t scratch[BLOCKSIZE] __ALIGN_END;
#endif
#endif
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

static volatile  UINT  WriteStatus = 0, ReadStatus = 0;
/* Private function prototypes -----------------------------------------------*/

DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
DRESULT SD_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read,
#if  _USE_WRITE == 1
  SD_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};


DSTATUS SD_initialize(BYTE lun)
{

	
	zf_driver_get(ZF_DRIVER_TYPE_SDMMC, (void **)&g_sdmmc);
	if (g_sdmmc == 0)
	{
		printf("get sdmmc drv error\r\n");
	}
	
  Stat = STA_NOINIT;
  Stat &= ~STA_NOINIT;

  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_status(BYTE lun)
{

  Stat = STA_NOINIT;
  Stat &= ~STA_NOINIT;

  return Stat;
}

/* USER CODE BEGIN beforeReadSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeReadSection */
/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */




DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  g_sdmmc->read(SDMMC_CH,sector,buff,count);
 return RES_OK;
}


/* USER CODE BEGIN beforeIoctlSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeIoctlSection */
/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  g_sdmmc->write(SDMMC_CH,sector,(uint8_t*)buff,count);
 return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/* USER CODE BEGIN beforeIoctlSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeIoctlSection */
/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;


  if (Stat & STA_NOINIT) return RES_NOTRDY;
	
	
	uint8_t card_type;
	uint16_t block_size;
	uint32_t bloack_num;
	unsigned long long size;
	g_sdmmc->get_info(SDMMC_CH,&card_type,&block_size,&bloack_num,&size);

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :

    *(DWORD*)buff = bloack_num;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :

    *(WORD*)buff = block_size;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff =block_size / SD_DEFAULT_BLOCK_SIZE;
    res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}

#endif /* _USE_IOCTL == 1 */

/* USER CODE BEGIN afterIoctlSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END afterIoctlSection */

/* USER CODE BEGIN callbackSection */
/* can be used to modify / following code or add new code */
/* USER CODE END callbackSection */
/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */


/* USER CODE BEGIN ErrorAbortCallbacks */
/*
==============================================================================================
  depending on the SD_HAL_Driver version, either the HAL_SD_ErrorCallback() or HAL_SD_AbortCallback()
  or both could be defined, activate the callbacks below when suitable and needed
==============================================================================================
void BSP_SD_AbortCallback(void)
{
}

void BSP_SD_ErrorCallback(void)
{
}
*/
/* USER CODE END ErrorAbortCallbacks */

/* USER CODE BEGIN lastSection */
/* can be used to modify / undefine previous code or add new code */
/* USER CODE END lastSection */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


