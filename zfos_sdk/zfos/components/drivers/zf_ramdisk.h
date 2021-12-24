#ifndef __ZF_RAMDISK_H__
#define __ZF_RAMDISK_H__

#include "zf_common.h"
#include "zf_storage.h"

#define STORAGE_CAPPACITY 1024 * 100
#define STORAGE_BLK_SIZ 512
#define STORAGE_BLK_NBR STORAGE_CAPPACITY / STORAGE_BLK_SIZ
#define MEDIA_DESCRIPTOR 0xf8

void dev_ramdisk_init(void);


//  模仿fatfs api
// 初始化
int ramdisk_init(void);
int ramdisk_get_info(struct zf_storage_info_t *info);
// 获取状态
int ramdisk_get_status(void);

// 擦除
int ramdisk_erase_all(void);
int ramdisk_erase_range(uint32_t addr, uint32_t size);

// 读数据
int ramdisk_read(uint32_t addr, uint8_t *buff, uint32_t size);
// 写数据
int ramdisk_write(uint32_t addr, uint8_t *buff, uint32_t size);

#endif
