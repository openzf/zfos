/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"
#include "stdio.h"

// sdmmc
uint8_t retSD;  /* Return value for SD */
char SDPath[4]; /* SD logical drive path */
FATFS SDFatFS;  /* File system object for SD logical drive */
FIL SDFile;     /* File object for SD */

// spi flash
uint8_t retUSER;   /* Return value for USER */
char FLASHPath[4]; /* USER logical drive path */
FATFS FLASHFatFS;  /* File system object for USER logical drive */
FIL FLASHFile;     /* File object for USER */

// qspi flash
uint8_t retUSERQSPI;  /* Return value for USER */
char USERPathQSPI[4]; /* USER logical drive path */
FATFS USERFatFSQSPI;  /* File system object for USER logical drive */
FIL USERFileQSPI;     /* File object for USER */

// ramdisk
uint8_t retUSERRAM;  /* Return value for USER */
char USERPathRAM[4]; /* USER logical drive path */
FATFS USERFatFSRAM;  /* File system object for USER logical drive */
FIL USERFileRAM;     /* File object for USER */

/* USER CODE BEGIN Variables */
#define WORK_BUFFER_SIZE 4096
static char work_buffer[WORK_BUFFER_SIZE];
/* USER CODE END Variables */

// 磁盘数目修改 ffconfig.h #define _VOLUMES    2
void fatfs_init(void)
{

/*## FatFS: Link the SD driver ###########################*/
#if USE_SDMMC_FILESYSTEM
    retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
#endif
/*## FatFS: Link the USER driver ###########################*/
#if USE_SPI_FLASH_FILESYSTEM
    retUSER = FATFS_LinkDriver(&USER_Driver, FLASHPath);
#endif
// qspi flash
#if USE_QSPI_FLASH_FILESYSTEM
    retUSERQSPI = FATFS_LinkDriver(&USER_QSPI_FLASHDriver, USERPathQSPI);
#endif
// ramdisk
#if USE_RAMDISK_FILESYSTEM
    retUSER = FATFS_LinkDriver(&USER_RAMDISK_Driver, USERPathRAM);
#endif

    /* USER CODE BEGIN Init */
    /* additional user code for init */
    /* USER CODE END Init */
}

int fatfs_mkfs(char *path)
{
    FRESULT retSD;
    retSD = f_mkfs(path, FM_FAT, 0, work_buffer, WORK_BUFFER_SIZE);
    if (retSD != FR_OK)
    {

        return 1;
    }
    else
    {
    }
    return 0;
}

//   fatfs test
int fatfs_mount(FATFS *arg_fatfs, const char *path)
{
    uint8_t ret = 0;
    ret = f_mount(arg_fatfs, path, 0);
    if (ret)
    {
        if (ret == FR_NO_FILESYSTEM)
        {

            ret = f_mkfs(path, FM_FAT, 0, work_buffer, WORK_BUFFER_SIZE);
            if (ret == FR_OK)
            {
                ret = f_mount(arg_fatfs, path, 0);
                if (ret == FR_OK)
                {
                    printf("mount:%s ok\r\n", path);
                    return 0;
                }
            }
        }
    }
    else
    {
        printf("mount:%s ok\r\n", path);
        return 0;
    }
    printf("mount:%s error\r\n", path);
    return 1;
}

// 获取空间
uint8_t fatfs_getfree(uint8_t *drv, uint32_t *total, uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;
    res = (uint32_t)f_getfree((const TCHAR *)drv, (DWORD *)&fre_clust, &fs1);
    if (res == 0)
    {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize;
        fre_sect = fre_clust * fs1->csize;
#if _MAX_SS != 512
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
#endif
        *total = tot_sect >> 1;
        *free = fre_sect >> 1;
    }
    return 0;
}

DWORD get_fattime(void)
{
    /* USER CODE BEGIN get_fattime */
    return 0;
    /* USER CODE END get_fattime */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
