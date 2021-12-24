#include "zf_fs.h"
#include "zfos_conf.h"

#define FD_MAX 10
FIL fd_arr[FD_MAX];
int fd_index = 1;

// 磁盘数目修改 ffconfig.h #define _VOLUMES    2
/**
 * @brief 
 * 
 */
void zf_fs_init(void)
{

    // 初始化
    fatfs_init();
#if USE_SDMMC_FILESYSTEM
    fatfs_mount(&SDFatFS, SDPath);
#endif
#if USE_SPI_FLASH_FILESYSTEM
    fatfs_mount(&FLASHFatFS, FLASHPath);
#endif
#if USE_QSPI_FLASH_FILESYSTEM
    fatfs_mount(&USERFatFSQSPI, USERPathQSPI);
#endif
#if USE_RAMDISK_FILESYSTEM
    fatfs_mkfs(USERPathRAM);
    fatfs_mount(&USERFatFSRAM, USERPathRAM);
#endif
}

/**
 * @brief 
 * 
 * @param pathname 
 * @param flags 
 * @return int 
 */
int open(const char *pathname, int flags)
{

    if (fd_index >= FD_MAX | fd_index < 0)
    {
        return -1;
    }
    f_open(&fd_arr[fd_index], pathname, flags);
    fd_index++;
    return (fd_index - 1);
}

/**
 * @brief 
 * 
 * @param fd 
 * @return int 
 */
int close(int fd)
{
    if (fd >= FD_MAX | fd < 0)
    {
        return -1;
    }
    return f_close(&fd_arr[fd]);
}

/**
 * @brief 
 * 
 * @param fd 
 * @param buf 
 * @param count 
 * @return int 
 */
int read(int fd, void *buf, int count)
{
    if (fd >= FD_MAX | fd < 0)
    {
        return -1;
    }
    UINT size = 0;
    f_read(&fd_arr[fd], buf, count, &size);
    return size;
}

/**
 * @brief 
 * 
 * @param fd 
 * @param buf 
 * @param count 
 * @return int 
 */
int write(int fd, const void *buf, int count)
{
    if (fd >= FD_MAX | fd < 0)
    {
        return -1;
    }

    int size = 0;
    f_write(&fd_arr[fd], buf, count, (void *)&size);
    return size;
}

/**
 * @brief 
 * 
 * @param fd 
 * @param offset 
 * @param whence 
 * @return int 
 */
int lseek(int fd, int offset, int whence)
{
    return 0;
}
