#ifndef __ZF_TOUCH_H__
#define __ZF_TOUCH_H__
#include "zf_common.h"

int dev_touch_init(void);
uint8_t touch_init(uint16_t pix_w, uint16_t pix_h, uint8_t lcd_dir);
uint8_t touch_get(uint16_t *x, uint16_t *y);
#endif
