/**
  ******************************************************************************
  * @file   fatfs.h
  * @brief  Header for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"

#include "zfos_conf.h"

#if USE_SDMMC_FILESYSTEM
#include "../user/sd_diskio.h" /* defines SD_Driver as external */
#endif 
#if USE_SPI_FLASH_FILESYSTEM
#include "../user/user_diskio.h" /* defines USER_Driver as external */
#endif
#if USE_QSPI_FLASH_FILESYSTEM
#include "../user/user_qspiflash.h"
#endif
#if  USE_RAMDISK_FILESYSTEM
#include "../user/user_ramdisk.h"
#endif


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
// sd
extern uint8_t retSD; /* Return value for SD */
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern FIL SDFile; /* File object for SD */

// spi flash
extern uint8_t retUSER;    /* Return value for USER */
extern char FLASHPath[4];   /* USER logical drive path */
extern FATFS FLASHFatFS;    /* File system object for USER logical drive */
extern FIL FLASHFile;       /* File object for USER */


// qspi flash
extern uint8_t retUSERQSPI;    /* Return value for USER */
extern char USERPathQSPI[4];   /* USER logical drive path */
extern FATFS USERFatFSQSPI;    /* File system object for USER logical drive */
extern FIL USERFileQSPI;       /* File object for USER */

// ramdisk
extern uint8_t retUSERRAM;    /* Return value for USER */
extern char USERPathRAM[4];   /* USER logical drive path */
extern FATFS USERFatFSRAM;    /* File system object for USER logical drive */
extern FIL USERFileRAM;       /* File object for USER */

void fatfs_init(void);
int fatfs_mkfs(char *path);
int fatfs_mount(FATFS *arg_fatfs, const char *path);
uint8_t fatfs_getfree(uint8_t *drv,uint32_t *total,uint32_t *free);


#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
