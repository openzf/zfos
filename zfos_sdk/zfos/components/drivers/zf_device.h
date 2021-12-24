#ifndef __ZF_DEVICE_H__
#define __ZF_DEVICE_H__

#include "zf_common.h"

// gpio
struct zf_device_task_t 
{   
    char name[20];
    int (*task_1ms_handler)(void);
    int (*task_1s_handler)(void);
};


int zf_device_register_task(struct zf_device_task_t  *device);
int zf_device_init(void);

#endif
