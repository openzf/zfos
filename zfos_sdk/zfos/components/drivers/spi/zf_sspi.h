#ifndef __ZF_sspi_H__
#define __ZF_sspi_H__

#include "zf_common.h"


int dev_sspi_init(void);


void sspi_init(uint32_t speed);
void sspi_write(uint8_t dat);
#endif
