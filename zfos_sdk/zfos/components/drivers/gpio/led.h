#ifndef __ZF_LED_H__
#define __ZF_LED_H__
#include "zf_common.h"

int dev_led_init(void);
void led_on(void);
void led_off(void);
void led_switch(char state);
#endif
