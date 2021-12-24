#include "../zf_driver.h"
#include "../zf_device.h"

#include "spi_flash.h"
#include "zf_dts.h"
static struct zf_gpio_t *g_gpio;
static struct zf_spi_t *g_spi;

#define W25Q80 0XEF13
#define W25Q16 0XEF14
#define W25Q32 0XEF15
#define W25Q64 0XEF16
#define W25Q128 0XEF17
#define W25Q256 0XEF18

//指令表
#define SPI_FLASH_W25X_WriteEnable 0x06
#define SPI_FLASH_W25X_WriteDisable 0x04
#define SPI_FLASH_W25X_ReadStatusReg1 0x05
#define SPI_FLASH_W25X_ReadStatusReg2 0x35
#define SPI_FLASH_W25X_ReadStatusReg3 0x15
#define SPI_FLASH_W25X_WriteStatusReg1 0x01
#define SPI_FLASH_W25X_WriteStatusReg2 0x31
#define SPI_FLASH_W25X_WriteStatusReg3 0x11
#define SPI_FLASH_W25X_ReadData 0x03
#define SPI_FLASH_W25X_FastReadData 0x0B
#define SPI_FLASH_W25X_FastReadDual 0x3B
#define SPI_FLASH_W25X_PageProgram 0x02
#define SPI_FLASH_W25X_BlockErase 0xD8
#define SPI_FLASH_W25X_SectorErase 0x20
#define SPI_FLASH_W25X_ChipErase 0xC7
#define SPI_FLASH_W25X_PowerDown 0xB9
#define SPI_FLASH_W25X_ReleasePowerDown 0xAB
#define SPI_FLASH_W25X_DeviceID 0xAB
#define SPI_FLASH_W25X_ManufactDeviceID 0x90
#define SPI_FLASH_W25X_JedecDeviceID 0x9F
#define SPI_FLASH_W25X_Enable4ByteAddr 0xB7
#define SPI_FLASH_W25X_Exit4ByteAddr 0xE9

//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize 256
#define SPI_FLASH_PerWritePageSize 256

#define SPI_FLASH_FATFS_SIZE (16 * 1024 * 1024) // 用作fatfs的大小
#define SPI_FLASH_BLOCK_SIZE 512
#define SPI_FLASH_FATFS_SELECTOR_SIZE (SPI_FLASH_FATFS_SIZE / SPI_FLASH_BLOCK_SIZE) // fatfs占用的扇区
#define SPI_FLASH_USER_ADDR SPI_FLASH_FATFS_SIZE + (512 * 1024)						// 偏移512字节

static uint16_t SPI_FLASH_TYPE = W25Q128; //默认是W25Q256

int spi_flash_write_nocheck(uint32_t addr, uint8_t *buff, uint32_t size);

uint16_t spi_flash_read_id(void); //读取FLASH ID
void spi_flash_power_down(void);  //进入掉电模式
void spi_flash_wakeup(void);	  //唤醒
static uint8_t spi_flash_send_byte(uint8_t byte);
uint8_t spi_flash_read_sr(uint8_t regno);			//读取状态寄存器
void spi_flash_4byteaddr_enable(void);				//使能4字节地址模式
void spi_flash_write_sr(uint8_t regno, uint8_t sr); //写状态寄存器
void spi_flash_write_enable(void);					//写使能
void spi_flash_write_disable(void);					//写保护
void spi_flash_wait_busy(void);						//等待空闲

// 内部函数

// test
#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress

uint8_t SPI_FLASH_BUFFER[4096];

// ----- 移植API

// cs -low
static void spi_flash_cs_low()
{
	g_gpio->write(FLASH_CS_GPIO_Port, FLASH_CS_Pin, 0);
}

// cs- high
static void spi_flash_cs_high()
{
	g_gpio->write(FLASH_CS_GPIO_Port, FLASH_CS_Pin, 1);
}

static uint8_t spi_flash_send_byte(uint8_t byte)
{
	uint8_t Rxdata;
	g_spi->write_read(0, &byte, 1, &Rxdata, 1);
	return Rxdata;
}
// ---------------- 内部API ----
uint8_t spi_flash_read_sr(uint8_t regno)
{
	uint8_t byte = 0, command = 0;
	switch (regno)
	{
	case 1:
		command = SPI_FLASH_W25X_ReadStatusReg1; //读状态寄存器1指令
		break;
	case 2:
		command = SPI_FLASH_W25X_ReadStatusReg2; //读状态寄存器2指令
		break;
	case 3:
		command = SPI_FLASH_W25X_ReadStatusReg3; //读状态寄存器3指令
		break;
	default:
		command = SPI_FLASH_W25X_ReadStatusReg1;
		break;
	}
	spi_flash_cs_low();				  //使能器件
	spi_flash_send_byte(command);	  //发送读取状态寄存器命令
	byte = spi_flash_send_byte(0Xff); //读取一个字节
	spi_flash_cs_high();			  //取消片选
	return byte;
}
void spi_flash_write_sr(uint8_t regno, uint8_t sr)
{
	uint8_t command = 0;
	switch (regno)
	{
	case 1:
		command = SPI_FLASH_W25X_WriteStatusReg1; //写状态寄存器1指令
		break;
	case 2:
		command = SPI_FLASH_W25X_WriteStatusReg2; //写状态寄存器2指令
		break;
	case 3:
		command = SPI_FLASH_W25X_WriteStatusReg3; //写状态寄存器3指令
		break;
	default:
		command = SPI_FLASH_W25X_WriteStatusReg1;
		break;
	}
	spi_flash_cs_low();			  //使能器件
	spi_flash_send_byte(command); //发送写取状态寄存器命令
	spi_flash_send_byte(sr);	  //写入一个字节
	spi_flash_cs_high();		  //取消片选
}

void spi_flash_write_enable(void)
{
	spi_flash_cs_low();								 //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_WriteEnable); //发送写使能
	spi_flash_cs_high();							 //取消片选
}

void spi_flash_write_disable(void)
{
	spi_flash_cs_low();								  //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_WriteDisable); //发送写禁止指令
	spi_flash_cs_high();							  //取消片选
}
uint16_t spi_flash_read_id(void)
{
	uint16_t Temp = 0;
	spi_flash_cs_low();
	spi_flash_send_byte(0x90); //发送读取ID命令
	spi_flash_send_byte(0x00);
	spi_flash_send_byte(0x00);
	spi_flash_send_byte(0x00);
	Temp |= spi_flash_send_byte(0xFF) << 8;
	Temp |= spi_flash_send_byte(0xFF);
	spi_flash_cs_high();
	return Temp;
}

//等待空闲
void spi_flash_wait_busy(void)
{
	while ((spi_flash_read_sr(1) & 0x01) == 0x01)
		; // 等待BUSY位清空
}
//进入掉电模式
void spi_flash_power_down(void)
{
	spi_flash_cs_low();							   //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_PowerDown); //发送掉电命令
	spi_flash_cs_high();						   //取消片选
												   //等待TPD
}
//唤醒
void spi_flash_wakeup(void)
{
	spi_flash_cs_low();									  //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_ReleasePowerDown); //  send SPI_FLASH_W25X_PowerDown command 0xAB
	spi_flash_cs_high();								  //取消片选												  //等待TRES1
}

void spi_flash_write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t i;
	spi_flash_write_enable();						 //SET WEL
	spi_flash_cs_low();								 //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_PageProgram); //发送写页命令
	if (SPI_FLASH_TYPE == W25Q256)					 //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		spi_flash_send_byte((uint8_t)((WriteAddr) >> 24));
	}
	spi_flash_send_byte((uint8_t)((WriteAddr) >> 16)); //发送24bit地址
	spi_flash_send_byte((uint8_t)((WriteAddr) >> 8));
	spi_flash_send_byte((uint8_t)WriteAddr);
	for (i = 0; i < NumByteToWrite; i++)
		spi_flash_send_byte(pBuffer[i]); //循环写数
	spi_flash_cs_high();				 //取消片选
	spi_flash_wait_busy();				 //等待写入结束
}

//---- 导出API



// ---- 内部使用
int spi_flash_init(void)
{

	// cs -high
	spi_flash_cs_high();
	// enable spi1

	spi_flash_wakeup();

	// read flash_id
	uint16_t id = spi_flash_read_id();

	if (id == SPI_FLASH_TYPE)
	{	
		
		return 0;
	}
	return 1;
}

int spi_flash_get_info(struct zf_storage_info_t *info)
{
	info->size = ((unsigned long long)SPI_FLASH_FATFS_SIZE);
	info->block_num = info->size / 512;
	info->block_size = 512;
	info->card_type = 9;
	return 0;
}
// 获取状态
int spi_flash_get_status(void)
{

	return 0;
}
int spi_flash_erase_all(void)
{
	spi_flash_write_enable(); //SET WEL
	spi_flash_wait_busy();
	spi_flash_cs_low();							   //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_ChipErase); //发送片擦除命令
	spi_flash_cs_high();						   //取消片选
	spi_flash_wait_busy();						   //等待芯片擦除结束
	return 0;
}

//擦除一个扇区的最少时间:150ms
int spi_flash_erase_range(uint32_t addr, uint32_t size)
{
	//监视falsh擦除情况,测试用
	//printf("fe:%x\r\n",Dst_Addr);
	addr *= 4096;
	spi_flash_write_enable(); //SET WEL
	spi_flash_wait_busy();
	spi_flash_cs_low();								 //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_SectorErase); //发送扇区擦除指令
	if (SPI_FLASH_TYPE == W25Q256)					 //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		spi_flash_send_byte((uint8_t)((addr) >> 24));
	}
	spi_flash_send_byte((uint8_t)((addr) >> 16)); //发送24bit地址
	spi_flash_send_byte((uint8_t)((addr) >> 8));
	spi_flash_send_byte((uint8_t)addr);
	spi_flash_cs_high();   //取消片选
	spi_flash_wait_busy(); //等待擦除完成
	return 0;
}

int spi_flash_read(uint32_t addr, uint8_t *buff, uint32_t size)
{
	uint16_t i;
	spi_flash_cs_low();							  //使能器件
	spi_flash_send_byte(SPI_FLASH_W25X_ReadData); //发送读取命令
	if (SPI_FLASH_TYPE == W25Q256)				  //如果是W25Q256的话地址为4字节的，要发送最高8位
	{
		spi_flash_send_byte((uint8_t)((addr) >> 24));
	}
	spi_flash_send_byte((uint8_t)((addr) >> 16)); //发送24bit地址
	spi_flash_send_byte((uint8_t)((addr) >> 8));
	spi_flash_send_byte((uint8_t)addr);
	for (i = 0; i < size; i++)
	{
		buff[i] = spi_flash_send_byte(0XFF); //循环读数
	}
	spi_flash_cs_high();
	
	return 0;
}

int spi_flash_write_nocheck(uint32_t addr, uint8_t *buff, uint32_t size)
{
	uint16_t pageremain;
	pageremain = 256 - addr % 256; //单页剩余的字节数
	if (size <= pageremain)
		pageremain = size; //不大于256个字节
	while (1)
	{
		spi_flash_write_Page(buff, addr, pageremain);
		if (size == pageremain)
			break; //写入结束了
		else	   //NumByteToWrite>pageremain
		{
			buff += pageremain;
			addr += pageremain;

			size -= pageremain; //减去已经写入了的字节数
			if (size > 256)
				pageremain = 256; //一次可以写入256个字节
			else
				pageremain = size; //不够256个字节了
		}
	};
	return 0;
}

int spi_flash_write(uint32_t addr, uint8_t *buff, uint32_t size)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *SPI_FLASH_BUF;
	SPI_FLASH_BUF = SPI_FLASH_BUFFER;
	secpos = addr / 4096;	   //扇区地址
	secoff = addr % 4096;	   //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
	if (size <= secremain)
		secremain = size; //不大于4096个字节
	while (1)
	{
		spi_flash_read(secpos * 4096, SPI_FLASH_BUF, 4096); //读出整个扇区的内容
		for (i = 0; i < secremain; i++)						//校验数据
		{
			if (SPI_FLASH_BUF[secoff + i] != 0XFF)
				break; //需要擦除
		}
		if (i < secremain) //需要擦除
		{
			spi_flash_erase_range(secpos, 1); //擦除这个扇区
			for (i = 0; i < secremain; i++)	  //复制
			{
				SPI_FLASH_BUF[i + secoff] = buff[i];
			}
			spi_flash_write_nocheck(secpos * 4096, SPI_FLASH_BUF, 4096); //写入整个扇区
		}
		else
			spi_flash_write_nocheck(addr, buff, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		if (size == secremain)
			break; //写入结束了
		else	   //写入未结束
		{
			secpos++;	//扇区地址增1
			secoff = 0; //偏移位置为0

			buff += secremain; //指针偏移
			addr += secremain; //写地址偏移
			size -= secremain; //字节数递减
			if (size > 4096)
				secremain = 4096; //下一个扇区还是写不完
			else
				secremain = size; //下一个扇区可以写完了
		}
	};

	return 0;
}


int dev_spi_flash_init(void)
{
	
	zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	zf_driver_get(ZF_DRIVER_TYPE_SPI, (void **)&g_spi);
	if (check_driver(ZF_DRIVER_TYPE_GPIO,g_gpio))
	{
				return 1;
	}else 
	{
	
		if (check_driver(ZF_DRIVER_TYPE_SPI,g_spi))
		{
			return 1;
		}
	}

	if (spi_flash_init() == 0)
	{
	
		return 0;
	}
			
	return 1;
}
