
#include "../zf_driver.h"
#include "../zf_device.h"


#include "zf_si2c.h"
#include "zf_dts.h"


static struct zf_gpio_t *g_gpio;

#define si2c_sda_h() g_gpio->write(SIIC_SDA_PORT, SIIC_SDA_PIN, 1)
#define si2c_sda_l() g_gpio->write(SIIC_SDA_PORT, SIIC_SDA_PIN, 0)

#define si2c_scl_h() g_gpio->write(SIIC_SCL_PORT, SIIC_SCL_PIN, 1)
#define si2c_scl_l() g_gpio->write(SIIC_SCL_PORT, SIIC_SCL_PIN, 0)


#define si2c_speed_delay() delay_us(2)
#define si2c_speed_start_delay() delay_us(30)

/**
 * @brief 
 * 
 */
void si2c_sda_out()
{
     g_gpio->config(SIIC_SDA_PORT, SIIC_SDA_PIN, 1);
}

/**
 * @brief 
 * 
 */
void si2c_sda_in()
{
     g_gpio->config(SIIC_SDA_PORT, SIIC_SDA_PIN, 0);
}

/**
 * @brief 
 * 
 * @param speed 
 */
void si2c_init(uint32_t speed)
{
    si2c_sda_out();
    si2c_sda_h();
    si2c_scl_h();
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t si2c_sda_read(void)
{
    return  g_gpio->read(SIIC_SDA_PORT, SIIC_SDA_PIN);
}

/**
 * @brief 
 * 
 * @param state 
 */
void si2c_sda_write(uint8_t state)
{
    g_gpio->write(SIIC_SDA_PORT, SIIC_SDA_PIN, state);
}

// -- 

// si2c信号
/**
 * @brief 
 * 
 */
void si2c_start(void)
{
 
    si2c_sda_out(); //sda线输出
    si2c_sda_h();
    si2c_scl_h();
    si2c_speed_start_delay();
    si2c_sda_l(); //START:when CLK is high,DATA change form high to low
    si2c_speed_delay();
    si2c_scl_l(); //钳住si2c总线，准备发送或接收数据
}

/**
 * @brief 
 * 
 */
void si2c_stop(void)
{
    si2c_sda_out(); //sda线输出
    si2c_scl_l();
    si2c_sda_l(); //STOP:when CLK is high DATA change form low to high
    si2c_speed_start_delay();
    si2c_scl_h();
    si2c_speed_delay();
    si2c_sda_h(); //发送si2c总线结束信号
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t si2c_wait_ack(void)
{
    uint8_t ucErrTime = 0;
    si2c_sda_in();
    si2c_sda_h();
    si2c_scl_h();
    si2c_speed_delay();
    while (si2c_sda_read())
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            si2c_stop();
            return 1;
        }
        si2c_speed_delay();
    }
    si2c_scl_l(); //时钟输出0
    return 0;
}

/**
 * @brief 
 * 
 */
void si2c_sent_ack(void)
{
    si2c_scl_l();
    si2c_sda_out();
    si2c_sda_l();
    si2c_speed_delay();
    si2c_scl_h();
    si2c_speed_delay();
    si2c_scl_l();
}

/**
 * @brief 
 * 
 */
void si2c_sent_nack(void)
{
    si2c_scl_l();
    si2c_sda_out();
    si2c_sda_h();
    si2c_speed_delay();
    si2c_scl_h();
    si2c_speed_delay();
    si2c_scl_l();
}

/**
 * @brief 
 * 
 * @param data 
 */
void si2c_sent_byte(uint8_t data)
{
    uint8_t t;
    si2c_sda_out();
    si2c_scl_l(); //拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {

        si2c_sda_write(((data & 0x80) >> 7));
        data <<= 1;
        si2c_speed_delay(); //对TEA5767这三个延时都是必须的
        si2c_scl_h();
        si2c_speed_delay();
        si2c_scl_l();
        si2c_speed_delay();
    }
}

/**
 * @brief 
 * 
 * @param ack 
 * @return uint8_t 
 */
uint8_t si2c_read_byte(uint8_t ack)
{
    unsigned char i, receive = 0;
    si2c_sda_in(); //SDA设置为输入
    si2c_speed_start_delay();
    for (i = 0; i < 8; i++)
    {
        si2c_scl_l();
        si2c_speed_delay();
        si2c_scl_h();
        receive <<= 1;
        if (si2c_sda_read())
        {

            receive++;
        }
        si2c_speed_delay();
    }
    if (!ack)
        si2c_sent_nack(); //发送nACK
    else
        si2c_sent_ack(); //发送ACK
    return receive;
}

int si2c_write(uint8_t ch,uint8_t addr, uint8_t  reg_addr, uint8_t *data,uint16_t len )
{
    int i= 0;
    si2c_start();
	si2c_sent_byte(addr);
	si2c_wait_ack();

    si2c_sent_byte(reg_addr);
    si2c_wait_ack();

    while(i<len)
    {
        si2c_sent_byte(data[i]);
        si2c_wait_ack();
        si2c_speed_delay();
        i++;
    }
	si2c_stop();
    return 0;
}
/**
 * @brief 
 * 
 * @return int 
 */
int dev_si2c_init(void)
{
    zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	if (check_driver(ZF_DRIVER_TYPE_GPIO,g_gpio))
	{
				return 1;
	}

    si2c_init(0);
	return 0;
}

