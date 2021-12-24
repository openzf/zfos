#ifndef __ZF_H__
#define __ZF_H__

#include "zfos_conf.h"
#include "components/components.h"

#define ZFOS_VERSION  1

enum platform
{
ZFOS_PLATFORM_STM32=0,
ZFOS_PLATFORM_NXPIMXRT,
ZFOS_PLATFORM_ESP32,
ZFOS_PLATFORM_LINUX
};

struct zfos_t
{
    // 平台
    enum platform platform;
    // 系列
    uint16_t series;
    // 板子型号
    char board_name[50];
    // 板子版本
    uint32_t board_version;
    // os版本
    uint32_t os_version;
    /* data */
};


char zfos_init(struct zfos_t *info);
extern struct zfos_t zfos;
#endif
