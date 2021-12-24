#include "../zf_driver.h"
#include "../zf_device.h"

#include "zf_dts.h"
#include "led.h"

static struct zf_gpio_t *g_gpio;

/**
 * @brief 
 * 
 */
void led_on(void)
{
	g_gpio->write(LED0_GPIO_Port, LED0_Pin, LED_ON);
}

/**
 * @brief 
 * 
 */
void led_off(void)
{
	g_gpio->write(LED0_GPIO_Port, LED0_Pin, LED_OFF);
}

/**
 * @brief 
 * 
 * @param state 
 */
void led_switch(char state)
{
	g_gpio->write(LED0_GPIO_Port, LED0_Pin, state);
}

/**
 * @brief 
 * 
 * @return int 
 */
static int task_1ms_handler(void)
{

	return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
static int task_1s_handler(void)
{

	return 0;
}

static struct zf_device_task_t led =
	{
		.name = "led",
		.task_1ms_handler = task_1ms_handler,
		.task_1s_handler = task_1s_handler,
};

/**
 * @brief 
 * 
 * @return int 
 */
int dev_led_init(void)
{
	zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	if (check_driver(ZF_DRIVER_TYPE_GPIO,g_gpio))
	{
			
		return 1;
	}

	zf_device_register_task(&led);
	return 0;
}
