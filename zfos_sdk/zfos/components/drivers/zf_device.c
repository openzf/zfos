#include "zf_device.h"

#define zf_device_task_tASK_MAX_NUM 20
struct zf_device_task_t *zf_device_task_list[zf_device_task_tASK_MAX_NUM];
int device_num = 0;

#include "./gpio/led.h"
#include "./gpio/key.h"
#include "./spi/spi_flash.h"
#include "./spi/qspi_flash.h"
#include "./i2c/zf_si2c.h"
#include "./spi/zf_sspi.h"

#include "zf_touch.h"
#include "zf_lcd_8080.h"
#include "./lcd/zf_lcd.h"
#include "./i2c/oled.h"
#include "./spi/lcd_spi.h"

static int test(void)
{
	return 0;
}

static int (*device_init_groups[])(void) =
    {
        test,
        dev_led_init,
        dev_key_init,
				dev_spi_flash_init,
        dev_qspi_flash_init,
        dev_si2c_init,
        dev_sspi_init,
        dev_touch_init,
        drv_lcd_8080_init,
        dev_oled_init,
        dev_lcd_spi_init,
        dev_lcd_init
		};

/**
 * @brief 
 * 
 * @param device 
 * @return int 
 */
int zf_device_register_task(struct zf_device_task_t *device)
{
    zf_device_task_list[device_num++] = (struct zf_device_task_t *)device;
    return (device_num - 1);
}

/**
 * @brief 
 * 
 * @return int 
 */
int zf_device_init(void)
{
    int i = 0;
		int state = 0;
    // 初始化设备
    for (i = 0; i < sizeof(device_init_groups) / 4; i++)
    {
        state =  (device_init_groups[i])();
				if(state != 0)
				{
					printf("zf_device init error :%d\r\n",i);
				}
    }

    // device task检测
    for(i=0;i<zf_device_task_tASK_MAX_NUM;i++)
    {
        if(zf_device_task_list[i]!=0)
        {
            printf("zf_device task %s init\r\n",zf_device_task_list[i]->name);
        }else{
			 
		}
    }

    return 0;
}
