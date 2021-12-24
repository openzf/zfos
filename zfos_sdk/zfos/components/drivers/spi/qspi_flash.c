#include "../zf_driver.h"
#include "../zf_device.h"

#include "qspi_flash.h"

static struct zf_gpio_t *g_gpio;
static struct zf_qspi_t *g_qspi;


#define QSPI_INS_NONE 0
#define QSPI_INS_1_LINE 1
#define QSPI_INS_2_LINES 2
#define QSPI_INS_4_LINES 3

#define QSPI_ADDR_NONE 0
#define QSPI_ADDR_1_LINE 1
#define QSPI_ADDR_2_LINES 2
#define QSPI_ADDR_4_LINES 3

#define QSPI_ADDR_8_BITS 0
#define QSPI_ADDR_16_BITS 1
#define QSPI_ADDR_24_BITS 2
#define QSPI_ADDR_32_BITS 3



#define QSPI_DATA_SIZE_NONE 0
#define QSPI_DATA_SIZE_1_LINE 1
#define QSPI_DATA_SIZE_2_LINES 2
#define QSPI_DATA_SIZE_4_LINES 3


#define W25Q80 0XEF13
#define W25Q16 0XEF14
#define W25Q32 0XEF15
#define W25Q64 0XEF16
#define W25Q128 0XEF17
#define W25Q256 0XEF18

//////////////////////////////////////////////////////////////////////////////////
//指令表
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg1 0x05
#define W25X_ReadStatusReg2 0x35
#define W25X_ReadStatusReg3 0x15
#define W25X_WriteStatusReg1 0x01
#define W25X_WriteStatusReg2 0x31
#define W25X_WriteStatusReg3 0x11
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F
#define W25X_Enable4ByteAddr 0xB7
#define W25X_Exit4ByteAddr 0xE9
#define W25X_SetReadParam 0xC0
#define W25X_EnterQPIMode 0x38
#define W25X_ExitQPIMode 0xFF

#define USE_FUN  0
#define QUAD_INOUT_FAST_READ_CMD_4BYTE 0xEC

#define QSPI_FLASH_FATFS_SIZE (16 * 1024 * 1024) // 用作fatfs的大小
//  w25q64和w25q128使用24bits,  256使用32bits
#define QSPI_USE_ADRESS_BITS QSPI_ADDR_24_BITS
//#define QSPI_USE_ADRESS_BITS   QSPI_ADDR_32_BITS

uint16_t qspi_flash_readID(void);                                                             //读取FLASH ID
void qspi_flash_write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //写flash,不校验

uint16_t QSPI_FLASH_TYPE = W25Q128; //默认是W25Q256
uint8_t QSPI_FLASH_QPI_MODE = 0;    //QSPI模式标志:0,SPI模式;1,QPI模式.

static void qspi_flash_qspi_enable(void);                   //使能QSPI模式
#if USE_FUN
static void qspi_flash_qspi_disable(void);                  //关闭QSPI模式
#endif
static uint8_t qspi_flash_read_sr(uint8_t regno);           //读取状态寄存器

static void qspi_flash_write_sr(uint8_t regno, uint8_t sr); //写状态寄存器
static void qspi_flash_write_enable(void);                  //写使能
#if USE_FUN
static void qspi_flash_write_disable(void);                 //写保护
#endif
static void qspi_flash_wait_busy(void);                     //等待空闲

#define QSPI_FLASH_CH  0
/// ------------API   END--------

//W25QXX进入QSPI模式
static void qspi_flash_qspi_enable(void)
{
    uint8_t stareg2;
    stareg2 = qspi_flash_read_sr(2); //先读出状态寄存器2的原始值
    if ((stareg2 & 0X02) == 0)       //QE位未使能
    {
        qspi_flash_write_enable();       //写使能
        stareg2 |= 1 << 1;               //使能QE位
        qspi_flash_write_sr(2, stareg2); //写状态寄存器2
    }
    g_qspi->write_cmd(QSPI_FLASH_CH,W25X_EnterQPIMode, 0, 0, QSPI_INS_1_LINE, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //写command指令,地址为0,无数据_8位地址_无地址_单线传输指令,无空周期,0个字节数据
    QSPI_FLASH_QPI_MODE = 1;                                                                                              //标记QSPI模式
}


#if USE_FUN
//W25QXX退出QSPI模式
static void qspi_flash_qspi_disable(void)
{
    qspi_sent_cmd(W25X_ExitQPIMode, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //写command指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
    QSPI_FLASH_QPI_MODE = 0;                                                                                              //标记SPI模式
}
#endif
//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
static uint8_t qspi_flash_read_sr(uint8_t regno)
{
    uint8_t byte = 0, command = 0;
    switch (regno)
    {
    case 1:
        command = W25X_ReadStatusReg1; //读状态寄存器1指令
        break;
    case 2:
        command = W25X_ReadStatusReg2; //读状态寄存器2指令
        break;
    case 3:
        command = W25X_ReadStatusReg3; //读状态寄存器3指令
        break;
    default:
        command = W25X_ReadStatusReg1;
        break;
    }
    if (QSPI_FLASH_QPI_MODE)
        g_qspi->write_cmd(QSPI_FLASH_CH,command, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_4_LINES); //QPI,写command指令,地址为0,4线传数据_8位地址_无地址_4线传输指令,无空周期,1个字节数据
    else
        g_qspi->write_cmd(QSPI_FLASH_CH,command, 0, 0, QSPI_INS_1_LINE, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_1_LINE); //SPI,写command指令,地址为0,单线传数据_8位地址_无地址_单线传输指令,无空周期,1个字节数据
    g_qspi->read(QSPI_FLASH_CH,&byte, 1);
    return byte;
}

//写W25QXX状态寄存器
static void qspi_flash_write_sr(uint8_t regno, uint8_t sr)
{
    uint8_t command = 0;
    switch (regno)
    {
    case 1:
        command = W25X_WriteStatusReg1; //写状态寄存器1指令
        break;
    case 2:
        command = W25X_WriteStatusReg2; //写状态寄存器2指令
        break;
    case 3:
        command = W25X_WriteStatusReg3; //写状态寄存器3指令
        break;
    default:
        command = W25X_WriteStatusReg1;
        break;
    }
    if (QSPI_FLASH_QPI_MODE)
        g_qspi->write_cmd(QSPI_FLASH_CH,command, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_4_LINES); //QPI,写command指令,地址为0,4线传数据_8位地址_无地址_4线传输指令,无空周期,1个字节数据
    else
        g_qspi->write_cmd(QSPI_FLASH_CH,command, 0, 0, QSPI_INS_1_LINE, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_1_LINE); //SPI,写command指令,地址为0,单线传数据_8位地址_无地址_单线传输指令,无空周期,1个字节数据
    g_qspi->write(QSPI_FLASH_CH,&sr, 1);
}

//W25QXX写使能
//将S1寄存器的WEL置位
static void qspi_flash_write_enable(void)
{
    if (QSPI_FLASH_QPI_MODE)
        g_qspi->write_cmd(QSPI_FLASH_CH,W25X_WriteEnable, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //QPI,写使能指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
    else
        g_qspi->write_cmd(QSPI_FLASH_CH,W25X_WriteEnable, 0, 0, QSPI_INS_1_LINE, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //SPI,写使能指令,地址为0,无数据_8位地址_无地址_单线传输指令,无空周期,0个字节数据
}

#if UNUSE_FUN
//W25QXX写禁止
//将WEL清零
static void qspi_flash_write_disable(void)
{
    if (QSPI_FLASH_QPI_MODE)
        qspi_sent_cmd(W25X_WriteDisable, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //QPI,写禁止指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
    else
        qspi_sent_cmd(W25X_WriteDisable, 0, 0, QSPI_INS_1_LINE, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //SPI,写禁止指令,地址为0,无数据_8位地址_无地址_单线传输指令,无空周期,0个字节数据
}
#endif
//等待空闲
static void qspi_flash_wait_busy(void)
{
    while ((qspi_flash_read_sr(1) & 0x01) == 0x01)
        ; // 等待BUSY位清空
}





//返回值如下:
//0XEF13,表示芯片型号为W25Q80
//0XEF14,表示芯片型号为W25Q16
//0XEF15,表示芯片型号为W25Q32
//0XEF16,表示芯片型号为W25Q64
//0XEF17,表示芯片型号为W25Q128
//0XEF18,表示芯片型号为W25Q256
uint16_t qspi_flash_readID(void)
{
    uint8_t temp[2];
    uint16_t deviceid;
    if (QSPI_FLASH_QPI_MODE)
        g_qspi->write_cmd(QSPI_FLASH_CH,W25X_ManufactDeviceID, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_4_LINES, QSPI_USE_ADRESS_BITS, QSPI_DATA_SIZE_4_LINES); //QPI,读id,地址为0,4线传输数据_24位地址_4线传输地址_4线传输指令,无空周期,2个字节数据
    else
        g_qspi->write_cmd(QSPI_FLASH_CH,W25X_ManufactDeviceID, 0, 0, QSPI_INS_1_LINE, QSPI_ADDR_1_LINE, QSPI_USE_ADRESS_BITS, QSPI_DATA_SIZE_1_LINE); //SPI,读id,地址为0,单线传输数据_24位地址_单线传输地址_单线传输指令,无空周期,2个字节数据
    g_qspi->read(QSPI_FLASH_CH,temp, 2);
    deviceid = (temp[0] << 8) | temp[1];
    return deviceid;
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(最大32bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void qspi_flash_write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    qspi_flash_write_enable();                                                                                                           //写使能
    g_qspi->write_cmd(QSPI_FLASH_CH,W25X_PageProgram, WriteAddr, 0, QSPI_INS_4_LINES, QSPI_ADDR_4_LINES, QSPI_USE_ADRESS_BITS, QSPI_DATA_SIZE_4_LINES); //QPI,页写指令,地址为WriteAddr,4线传输数据_32位地址_4线传输地址_4线传输指令,无空周期,NumByteToWrite个数据
    g_qspi->write(QSPI_FLASH_CH,pBuffer, NumByteToWrite);
    qspi_flash_wait_busy(); //等待写入结束
}

//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(最大32bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void qspi_flash_write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite; //不大于256个字节
    while (1)
    {
        qspi_flash_write_Page(pBuffer, WriteAddr, pageremain);
        if (NumByteToWrite == pageremain)
            break; //写入结束了
        else       //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain; //减去已经写入了的字节数
            if (NumByteToWrite > 256)
                pageremain = 256; //一次可以写入256个字节
            else
                pageremain = NumByteToWrite; //不够256个字节了
        }
    }
}


//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q256
//容量为32M字节,共有512个Block,8192个Sector

//初始化SPI FLASH的IO口
int qspi_flash_init(void)
{
    uint8_t temp;

    qspi_flash_qspi_enable();              //使能QSPI模式
    QSPI_FLASH_TYPE = qspi_flash_readID(); //读取FLASH ID.

    if (QSPI_FLASH_TYPE == W25Q128) //SPI FLASH为W25Q256
    {
        temp = qspi_flash_read_sr(3); //读取状态寄存器3，判断地址模式
        if ((temp & 0X01) == 0)       //如果不是4字节地址模式,则进入4字节地址模式
        {
            qspi_flash_write_enable();                                                                                                //写使能
            g_qspi->write_cmd(QSPI_FLASH_CH,W25X_Enable4ByteAddr, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //QPI,使能4字节地址指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
        }
        qspi_flash_write_enable();                                                                                                //写使能
        g_qspi->write_cmd(QSPI_FLASH_CH,W25X_SetReadParam, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_4_LINES); //QPI,设置读参数指令,地址为0,4线传数据_8位地址_无地址_4线传输指令,无空周期,1个字节数据
        temp = 3 << 4;                                                                                                            //设置P4&P5=11,8个dummy clocks,104M
        g_qspi->write(QSPI_FLASH_CH,&temp, 1);                                                                                                      //发送1个字节
    }
	
    return 0;
}

int qspi_flash_get_info(struct zf_storage_info_t *info)
{
    info->size = ((unsigned long long)QSPI_FLASH_FATFS_SIZE);
    info->block_num = info->size / 512;
    info->block_size = 512;
    info->card_type = 9;
    return 0;
}
// 获取状态
int qspi_flash_get_status(void)
{

    return 0;
}

//擦除整个芯片
//等待时间超长...
int qspi_flash_erase_all(void)
{
    qspi_flash_write_enable(); //SET WEL
    qspi_flash_wait_busy();
    g_qspi->write_cmd(QSPI_FLASH_CH,W25X_ChipErase, 0, 0, QSPI_INS_4_LINES, QSPI_ADDR_NONE, QSPI_ADDR_8_BITS, QSPI_DATA_SIZE_NONE); //QPI,写全片擦除指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
    qspi_flash_wait_busy();                                                                                             //等待芯片擦除结束
    return 0;
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
int qspi_flash_erase_range(uint32_t Dst_Addr, uint32_t size)
{

    Dst_Addr *= 4096;
    qspi_flash_write_enable(); //SET WEL
    qspi_flash_wait_busy();
    g_qspi->write_cmd(QSPI_FLASH_CH,W25X_SectorErase, Dst_Addr, 0, QSPI_INS_4_LINES, QSPI_ADDR_4_LINES, QSPI_USE_ADRESS_BITS, QSPI_DATA_SIZE_NONE); //QPI,写扇区擦除指令,地址为0,无数据_32位地址_4线传输地址_4线传输指令,无空周期,0个字节数据
    qspi_flash_wait_busy();                                                                                                          //等待擦除完成
    return 0;
}

//读取SPI FLASH,仅支持QPI模式
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(最大32bit)
//NumByteToRead:要读取的字节数(最大65535)
int qspi_flash_read(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumByteToRead)
{
    g_qspi->write_cmd(QSPI_FLASH_CH,W25X_FastReadData, ReadAddr, 8, QSPI_INS_4_LINES, QSPI_ADDR_4_LINES, QSPI_USE_ADRESS_BITS, QSPI_DATA_SIZE_4_LINES); //QPI,快速读数据,地址为ReadAddr,4线传输数据_32位地址_4线传输地址_4线传输指令,8空周期,NumByteToRead个数据
    g_qspi->read(QSPI_FLASH_CH,pBuffer, NumByteToRead);
    return 0;
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(最大32bit)
//NumByteToWrite:要写入的字节数(最大65535)
uint8_t QSPI_FLASH_BUFFER[4096];
/**
 * @brief 
 * 
 * @param WriteAddr 
 * @param pBuffer 
 * @param NumByteToWrite 
 * @return int 
 */
int qspi_flash_write(uint32_t WriteAddr, uint8_t *pBuffer, uint32_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *QSPI_FLASH_BUF;
    QSPI_FLASH_BUF = QSPI_FLASH_BUFFER;
    secpos = WriteAddr / 4096; //扇区地址
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小
    //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite; //不大于4096个字节
    while (1)
    {
        qspi_flash_read(secpos * 4096, QSPI_FLASH_BUF, 4096); //读出整个扇区的内容
        for (i = 0; i < secremain; i++)                       //校验数据
        {
            if (QSPI_FLASH_BUF[secoff + i] != 0XFF)
                break; //需要擦除
        }
        if (i < secremain) //需要擦除
        {
            qspi_flash_erase_range(secpos, 0); //擦除这个扇区
            for (i = 0; i < secremain; i++)    //复制
            {
                QSPI_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            qspi_flash_write_NoCheck(QSPI_FLASH_BUF, secpos * 4096, 4096); //写入整个扇区
        }
        else
            qspi_flash_write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        if (NumByteToWrite == secremain)
            break; //写入结束了
        else       //写入未结束
        {
            secpos++;   //扇区地址增1
            secoff = 0; //偏移位置为0

            pBuffer += secremain;        //指针偏移
            WriteAddr += secremain;      //写地址偏移
            NumByteToWrite -= secremain; //字节数递减
            if (NumByteToWrite > 4096)
                secremain = 4096; //下一个扇区还是写不完
            else
                secremain = NumByteToWrite; //下一个扇区可以写完了
        }
    };
    return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int dev_qspi_flash_init(void)
{
	zf_driver_get(ZF_DRIVER_TYPE_GPIO, (void **)&g_gpio);
	zf_driver_get(ZF_DRIVER_TYPE_QSPI, (void **)&g_qspi);
	if (check_driver(ZF_DRIVER_TYPE_GPIO,g_gpio))
	{
				return 1;
	}else {
		
		if (check_driver(ZF_DRIVER_TYPE_QSPI,g_qspi))
		{
					return 1;
		}
	}
	
	if (qspi_flash_init() == 0)
	{
		return 0;
	}
	 return 0;
}

