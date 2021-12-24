#include "../zf_driver.h"
#include "../zf_device.h"

#include "zf_touch.h"
#include "./i2c/gt91x.h"

#include "zf_dts.h"
static struct zf_gpio_t *g_gpio;

// 获取触摸
uint8_t (*g_scan)(TouchTypedef *touch);
TouchTypedef mtouch;

/**
 * @brief 
 * 
 * @return int 
 */
int dev_touch_init(void)
{
	zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	if (g_gpio == 0)
	{
		printf("get gpio drv error\r\n");
	}
	return 0;
}

/**
 * @brief 
 * 
 * @param pix_w 
 * @param pix_h 
 * @param lcd_dir 
 * @return uint8_t 
 */
uint8_t touch_init(uint16_t pix_w, uint16_t pix_h, uint8_t lcd_dir)
{
    g_gpio->write(TOUCH_RESET_GPIO_Port, TOUCH_RESET_Pin, 1);
    delay_ms(500);
    g_gpio->write(TOUCH_RESET_GPIO_Port, TOUCH_RESET_Pin, 0);
    delay_ms(500);
    g_gpio->write(TOUCH_RESET_GPIO_Port, TOUCH_RESET_Pin, 1);
    delay_ms(500);

    gt91x_init(&g_scan);
    // ft5206_init(&g_scan);
    return 0;
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @return uint8_t 
 */
uint8_t touch_get(uint16_t *x, uint16_t *y)
{
    if (g_scan(&mtouch))
    {
        *x = mtouch.x[0];
        *y = mtouch.y[0];
        printf("touch: %d %d\r\n", *x, *y);
        return 1;
    }
    else
    {

        return 0;
    }
}
