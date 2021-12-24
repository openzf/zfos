#include "zf_ramdisk.h"

uint8_t RamDisk[STORAGE_BLK_NBR * STORAGE_BLK_SIZ];

// api
/**
 * @brief 
 * 
 * @return int 
 */
int ramdisk_init(void)
{

    return 0;
}

/**
 * @brief 
 * 
 * @param info 
 * @return int 
 */
int ramdisk_get_info(struct zf_storage_info_t *info)
{

    return 0;
}

// 获取状态
/**
 * @brief 
 * 
 * @return int 
 */
int ramdisk_get_status(void)
{
    return 0;
}

// 擦除
/**
 * @brief 
 * 
 * @return int 
 */
int ramdisk_erase_all(void)
{
    return 0;
}

/**
 * @brief 
 * 
 * @param addr 
 * @param size 
 * @return int 
 */
int ramdisk_erase_range(uint32_t addr, uint32_t size)
{
    return 0;
}

// 读数据
/**
 * @brief 
 * 
 * @param addr 
 * @param buff 
 * @param size 
 * @return int 
 */
int ramdisk_read(uint32_t addr, uint8_t *buff, uint32_t size)
{

    memcpy(buff, (void *)(RamDisk + addr), size);
    return 0;
}

// 写数据
/**
 * @brief 
 * 
 * @param addr 
 * @param buff 
 * @param size 
 * @return int 
 */
int ramdisk_write(uint32_t addr, uint8_t *buff, uint32_t size)
{

    memcpy((void *)(RamDisk + addr), buff, size);
    return 0;
}

/**
 * @brief 
 * 
 */
void dev_ramdisk_init(void)
{
    
}
