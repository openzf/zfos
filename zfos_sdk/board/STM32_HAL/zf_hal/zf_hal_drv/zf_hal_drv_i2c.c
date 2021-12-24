/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : zf_hal_drv_i2c.c
 * Author    : kirito
 * Brief     : 
 * Date      :  2021.12.17
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "zf_hal_drv_def.h"
#if USE_BSP_I2C
#include "i2c.h"
#define I2CX_FLAG_TIMEOUT ((uint32_t)1000)						//0x1100
#define I2CX_LONG_TIMEOUT ((uint32_t)(300 * I2CX_FLAG_TIMEOUT)) //was300

static I2C_HandleTypeDef *g_i2c_handle_group[ZF_I2C_MAX_NUM] =
	{
		&ZF_I2C0,
		&ZF_I2C1,
		&ZF_I2C2};

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int i2c_hw_init(uint8_t ch)
{
	return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @return int 
 */
static int i2c_hw_get(uint8_t ch, uint8_t cmd)
{
	return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param cmd 
 * @param data 
 * @return int 
 */
static int i2c_hw_set(uint8_t ch, uint8_t cmd, uint32_t data)
{
	return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return int 
 */
static int i2c_hw_close(uint8_t ch)
{
	return 0;
}

/**
 * @brief 
 * 
 * @param channel 
 * @param speed 
 * @return int 
 */
static int i2c_config(uint8_t channel, uint32_t speed)
{
	return 0;
}

/**
 * @brief 
 * 
 * @param ch 
 * @param addr 
 * @param reg_addr 
 * @param len 
 * @param data 
 * @return int 
 */
static int i2c_read(uint8_t ch, uint8_t addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{

	HAL_StatusTypeDef status = HAL_OK;

	I2C_HandleTypeDef *mh2ic = g_i2c_handle_group[ch];

	status = HAL_I2C_Mem_Read(mh2ic, addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, I2CX_FLAG_TIMEOUT);
	/* 检查通讯状态 */
	if (status != HAL_OK)
	{
		/* 总线出错处理 */
	}
	while (HAL_I2C_GetState(mh2ic) != HAL_I2C_STATE_READY)
	{
	}
	/* 检查SENSOR是否就绪进行下一次读写操作 */
	while (HAL_I2C_IsDeviceReady(mh2ic, addr, I2CX_FLAG_TIMEOUT, I2CX_FLAG_TIMEOUT) == HAL_TIMEOUT)
		;
	/* 等待传输结束 */
	while (HAL_I2C_GetState(mh2ic) != HAL_I2C_STATE_READY)
	{
	}
	return 0;
}

/**
 * @brief 
 * 
 * @param channel 
 * @param addr 
 * @param reg_addr 
 * @param len 
 * @param data 
 * @return int 
 */
static int i2c_write(uint8_t ch, uint8_t addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	HAL_StatusTypeDef status = HAL_OK;
	I2C_HandleTypeDef *mh2ic = g_i2c_handle_group[ch];
	status = HAL_I2C_Mem_Write(mh2ic, addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, I2CX_FLAG_TIMEOUT);
	/* 检查通讯状态 */
	if (status != HAL_OK)
	{
	}
	while (HAL_I2C_GetState(mh2ic) != HAL_I2C_STATE_READY)
	{
	}
	/* 检查SENSOR是否就绪进行下一次读写操作 */
	while (HAL_I2C_IsDeviceReady(mh2ic, addr, I2CX_FLAG_TIMEOUT, I2CX_FLAG_TIMEOUT) == HAL_TIMEOUT)
		;
	/* 等待传输结束 */
	while (HAL_I2C_GetState(mh2ic) != HAL_I2C_STATE_READY)
	{
	}
	return 0;
}

/**
 * @brief 
 * 
 */
static struct zf_i2c_t zf_i2c =
	{
		.hw = {
			.init = i2c_hw_init,
			.get = i2c_hw_get,
			.set = i2c_hw_set,
			.close = i2c_hw_close},

		.config = i2c_config,
		.read = i2c_read,
		.write = i2c_write};

/**
 * @brief 
 * 
 */
void zf_hal_drv_i2c_init(void)
{
	zf_driver_register(ZF_DRIVER_TYPE_I2C, &zf_i2c);
}

#endif
