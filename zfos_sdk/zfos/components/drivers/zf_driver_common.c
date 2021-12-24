
#include "zf_driver.h"

static struct zf_uart_t *zf_uart;
static struct zf_delay_t  *zf_delay;

#define ZF_DRIVER_TYPE_MAX_NUM 50
char ZF_DRIVER_TYPE_NAME[ZF_DRIVER_TYPE_MAX_NUM][10]=
{
{"gpio"},
{"uart"},
{"delay"},
{"i2c"},
{"spi"},
{"qspi"},
{"sdmmc"},
{"lcd_rgb"},
{"lcd_8080"},
};

/**
 * @brief 
 * 
 * @return int 
 */
int zf_driver_common_init(void)
{
   zf_driver_get(ZF_DRIVER_TYPE_DELAY,(void**)&zf_delay);
   zf_driver_get(ZF_DRIVER_TYPE_UART,(void**)&zf_uart);
		
    return 0;
}

/**
 * @brief 
 * 
 * @param time 
 */
void delay_us(uint32_t time)
{
    zf_delay->delay_us(time);
}

/**
 * @brief 
 * 
 * @param time 
 */
void delay_ms(uint32_t time)
{

    zf_delay->delay_ms(time);
}

/**
 * @brief 
 * 
 * @param time 
 */
void delay_s(uint32_t time)
{
    zf_delay->delay_s(time);
}

/**
 * @brief 
 * 
 * @param ch 
 * @param fmt 
 * @param ... 
 */
void uart_printf(uint8_t ch,char *fmt, ...)
{
	char buffer[50]; 
	char *p = buffer;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, 50, fmt, arg_ptr);
	while(*p != '\0'){
			zf_uart->write(ch,(uint8_t*)p++,1);
	}
	va_end(arg_ptr);
}

/**
 * @brief 
 * 
 * @param ch 
 * @param out_data 
 * @param len 
 * @return int 
 */
int  uart_read(uint8_t ch, uint8_t *out_data,uint16_t len)
{
	return zf_uart->read(ch,out_data,len);
}

/**
 * @brief Get the driver type name object
 * 
 * @param type 
 * @param output 
 * @return int 
 */
 int get_driver_type_name(int type,char **output)
{
	if(type >= (ZF_DRIVER_TYPE_MAX_NUM-1))
	{
		return 1;
	}
	*output =  (char*)(&ZF_DRIVER_TYPE_NAME[type]);

	return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int assert_driver_ok(void)
{
	return 0;
}

/**
 * @brief 
 * 
 * @param type 
 * @param drv 
 * @param file 
 * @param line 
 * @return int 
 */
int assert_driver_failed(int type,void* drv,uint8_t* file, uint32_t line)
{
	char *type_name;
	get_driver_type_name( type,&type_name);

	#if ZFOS_USE_DEV
	printf("getdrv:%s %d error: file %s on line %d\r\n",type_name,type, file, line);
	#else
	printf("getdrv:%s %d error\r\n",type_name,type);
	#endif
	
	return 1;
}

