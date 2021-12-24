#ifndef __ZF_DRIVER_COMMON_H__
#define __ZF_DRIVER_COMMON_H__

#include "zf_common.h"


int zf_driver_common_init(void);
void delay_us(uint32_t time);
void delay_ms(uint32_t time);
void delay_s(uint32_t time);
void uart_printf(uint8_t ch,char *fmt, ...);
int uart_read(uint8_t ch, uint8_t *out_data,uint16_t len);

#endif
