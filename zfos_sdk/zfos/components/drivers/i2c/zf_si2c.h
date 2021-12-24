#ifndef __ZF_si2c_H__
#define __ZF_si2c_H__

#include "zf_common.h"

int dev_si2c_init(void);

void si2c_init(uint32_t speed);
void si2c_start(void);
void si2c_stop(void);
uint8_t si2c_wait_ack(void);
void si2c_sent_ack(void);
void si2c_sent_nack(void);
void si2c_sent_byte(uint8_t data);
uint8_t si2c_read_byte(uint8_t ack);

// 模仿硬件i2c
int si2c_write(uint8_t ch,uint8_t addr, uint8_t  reg_addr, uint8_t *data,uint16_t len );
#endif
