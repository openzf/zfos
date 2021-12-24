
#include "../zf_driver.h"
#include "../zf_device.h"


#include "zf_si2c.h"
#include "zf_dts.h"
#include "zf_sspi.h"

static struct zf_gpio_t *g_gpio;



#define SSPI_SCLK_L() g_gpio->write(SSPI_SCL_PORT  ,SSPI_SCL_PIN  , 0)
#define SSPI_SCLK_H() g_gpio->write(SSPI_SCL_PORT  ,SSPI_SCL_PIN  , 1)

#define SSPI_MOSI_L() g_gpio->write(SSPI_MOSI_PORT  , SSPI_MOSI_PIN  , 0)
#define SSPI_MOSI_H() g_gpio->write(SSPI_MOSI_PORT  , SSPI_MOSI_PIN  , 1)

#define SSPI_MISO_L() g_gpio->write( SSPI_MISO_PORT  , SSPI_MISO_PIN  , 0)
#define SSPI_MISO_H() g_gpio->write( SSPI_MISO_PORT  , SSPI_MISO_PIN  , 1)


static int mspeed_delay = 0;
static void mdelay(void)
{
	int i = mspeed_delay;
	while(i--)
	{
		
	}
}

void sspi_write(uint8_t dat) 
{	
	uint8_t i;
	for(i=0;i<8;i++)
	{			  
		SSPI_SCLK_L();
		if(dat&0x80)
		{
		   SSPI_MOSI_H();
		}
		else
		{
		   SSPI_MOSI_L();
		}
		mdelay();
		SSPI_SCLK_H();
		mdelay();
		dat<<=1;
	}	
}



void sspi_init(uint32_t speed)
{
	mspeed_delay = 30;

	SSPI_SCLK_H();
	SSPI_MOSI_H();
}

int dev_sspi_init(void)
{
    zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	if (check_driver(ZF_DRIVER_TYPE_GPIO,g_gpio))
	{
				return 1;
	}
	sspi_init(0);
	return 0;
}
