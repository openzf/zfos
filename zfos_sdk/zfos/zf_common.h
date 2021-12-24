#ifndef __ZF_COMMON_H__
#define __ZF_COMMON_H__

#include "stdint.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include "zfos_conf.h"
#define check_driver(type,expr) ((expr) ? assert_driver_ok() : assert_driver_failed(type,(void*)expr,(uint8_t *)__FILE__, __LINE__))
int assert_driver_failed(int type,void* drv,uint8_t* file, uint32_t line);
int assert_driver_ok(void);
#endif
