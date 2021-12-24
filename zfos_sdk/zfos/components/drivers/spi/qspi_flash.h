#ifndef __ZF_QSPI_FLASH_H__
#define __ZF_QSPI_FLASH_H__
#include "zf_common.h"

#include "zf_driver.h"

int dev_qspi_flash_init(void);

// 初始化
int qspi_flash_init(void);
int qspi_flash_get_info(struct zf_storage_info_t *info);
// 获取状态
int qspi_flash_get_status(void);

// 擦除
int qspi_flash_erase_all(void);
int qspi_flash_erase_range(uint32_t addr, uint32_t size);

// 读数据
int qspi_flash_read(uint32_t addr, uint8_t *buff, uint32_t size);
// 写数据
int qspi_flash_write(uint32_t addr, uint8_t *buff, uint32_t size);

#endif
