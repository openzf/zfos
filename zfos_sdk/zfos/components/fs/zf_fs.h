#ifndef __ZF_FS_H__
#define __ZF_FS_H__


#include "./FATFS/zf_fatfs.h"

#define O_RDONLY FA_READ
#define O_WRONLY FA_WRITE
#define O_RDWR (FA_WRITE|FA_READ)
#define O_CREAT FA_CREATE_ALWAYS
#define O_EXCL  FA_OPEN_EXISTING

void zf_fs_init(void);

int open(const char *pathname, int flags);
int close(int fd);
int read(int fd, void *buf, int count);
int write(int fd, const void *buf, int count);
int lseek(int fd, int offset, int whence);
#endif
