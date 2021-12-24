#include "../zf_driver.h"
#include "../zf_device.h"

#include "zf_dts.h"
#include "key.h"

static struct zf_gpio_t *g_gpio;

void key_init(void)
{
}
// struct key
// {
//    uint32_t port;
//    uint32_t pin;
//    uint32_t mode;
//    uint32_t value;
// };
// struct key key_list[5] =
// {aaaaaaaaaaaaaaaaaaaaaaaccc
// {GPIOA,PIN2,UP,0x01},// {GPIOA,PIN2,UP,0x01},
// {GPIOA,PIN2,UP,0x01},
// }

/**
 * @brief 
 * 
 * @param key 
 * @return uint8_t 
 */
uint8_t key_read(uint8_t key)
{
	return g_gpio->read(KEY0_GPIO_Port, KEY0_Pin);
}

/**
 * @brief 
 * 
 * @return int 
 */
int dev_key_init(void)
{

	zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	if (check_driver(ZF_DRIVER_TYPE_GPIO,g_gpio))
	{
		return 1;
	}

	key_init();
	return 0;
}
