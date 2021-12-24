#include "gt91x.h"
#include "zf_si2c.h"

//si2c读写命令
#define GT_CMD_WR 0X28 //写命令
#define GT_CMD_RD 0X29 //读命令

//GT9147 部分寄存器定义
#define GT_CTRL_REG 0X8040  //GT9147控制寄存器
#define GT_CFGS_REG 0X8047  //GT9147配置起始地址寄存器
#define GT_CHECK_REG 0X80FF //GT9147校验和寄存器
#define GT_PID_REG 0X8140   //GT9147产品ID寄存器

#define GT_GSTID_REG 0X814E //GT9147当前检测到的触摸情况
#define GT_TP1_REG 0X8150   //第一个触摸点数据地址
#define GT_TP2_REG 0X8158   //第二个触摸点数据地址
#define GT_TP3_REG 0X8160   //第三个触摸点数据地址
#define GT_TP4_REG 0X8168   //第四个触摸点数据地址
#define GT_TP5_REG 0X8170   //第五个触摸点数据地址

//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
const uint8_t GT9147_CFG_TBL[] =
    {
        0X6A,
        0XE0,
        0X01,
        0X20,
        0X03,
        0X05,
        0X35,
        0X00,
        0X02,
        0X08,
        0X1E,
        0X08,
        0X50,
        0X3C,
        0X0F,
        0X05,
        0X00,
        0X00,
        0XFF,
        0X67,
        0X50,
        0X00,
        0X00,
        0X18,
        0X1A,
        0X1E,
        0X14,
        0X89,
        0X28,
        0X0A,
        0X30,
        0X2E,
        0XBB,
        0X0A,
        0X03,
        0X00,
        0X00,
        0X02,
        0X33,
        0X1D,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X32,
        0X00,
        0X00,
        0X2A,
        0X1C,
        0X5A,
        0X94,
        0XC5,
        0X02,
        0X07,
        0X00,
        0X00,
        0X00,
        0XB5,
        0X1F,
        0X00,
        0X90,
        0X28,
        0X00,
        0X77,
        0X32,
        0X00,
        0X62,
        0X3F,
        0X00,
        0X52,
        0X50,
        0X00,
        0X52,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X0F,
        0X0F,
        0X03,
        0X06,
        0X10,
        0X42,
        0XF8,
        0X0F,
        0X14,
        0X00,
        0X00,
        0X00,
        0X00,
        0X1A,
        0X18,
        0X16,
        0X14,
        0X12,
        0X10,
        0X0E,
        0X0C,
        0X0A,
        0X08,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X29,
        0X28,
        0X24,
        0X22,
        0X20,
        0X1F,
        0X1E,
        0X1D,
        0X0E,
        0X0C,
        0X0A,
        0X08,
        0X06,
        0X05,
        0X04,
        0X02,
        0X00,
        0XFF,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
        0XFF,
};

static uint8_t GT9147_Send_Cfg(uint8_t touch_num);
static uint8_t GT9147_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
static void GT9147_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
static uint8_t gt9147_scan(TouchTypedef *touch);


int gt91x_init(uint8_t (**arg_scan)(TouchTypedef *touch))
{
    uint8_t temp[5];
    GT9147_RD_Reg(GT_PID_REG, temp, 4); //读取产品ID
    temp[4] = 0;
    *arg_scan = gt9147_scan;

    if (strcmp((char *)temp, "9147") == 0) //ID==9147
    {
        
        temp[0] = 0X02;
        GT9147_WR_Reg(GT_CTRL_REG, temp, 1); //软复位GT9147
        GT9147_RD_Reg(GT_CFGS_REG, temp, 1); //读取GT_CFGS_REG寄存器

        GT9147_Send_Cfg(1);
        if (temp[0] < 0X60) //默认版本比较低,需要更新flash配置
        {

            GT9147_Send_Cfg(1);
        }
        temp[0] = 0X00;
        GT9147_WR_Reg(GT_CTRL_REG, temp, 1); //结束复位

        return 0;
    }

    return 1;
}

//发送GT9147配置参数
//touch_num:0,参数不保存到flash
//     1,参数保存到flash
static uint8_t GT9147_Send_Cfg(uint8_t touch_num)
{
    uint8_t buf[2];
    uint8_t i = 0;
    buf[0] = 0;
    buf[1] = touch_num; //是否写入到GT9147 FLASH?  即是否掉电保存
    for (i = 0; i < sizeof(GT9147_CFG_TBL); i++)
        buf[0] += GT9147_CFG_TBL[i]; //计算校验和
    buf[0] = (~buf[0]) + 1;
    GT9147_WR_Reg(GT_CFGS_REG, (uint8_t *)GT9147_CFG_TBL, sizeof(GT9147_CFG_TBL)); //发送寄存器配置
    GT9147_WR_Reg(GT_CHECK_REG, buf, 2);                                           //写入校验和,和配置更新标记
    return 0;
}

//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
static uint8_t GT9147_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    si2c_start();
    si2c_sent_byte(GT_CMD_WR); //发送写命令
    si2c_wait_ack();
    si2c_sent_byte(reg >> 8); //发送高8位地址
    si2c_wait_ack();
    si2c_sent_byte(reg & 0XFF); //发送低8位地址
    si2c_wait_ack();
    for (i = 0; i < len; i++)
    {
        si2c_sent_byte(buf[i]); //发数据
        ret = si2c_wait_ack();
        if (ret)
            break;
    }
    si2c_stop(); //产生一个停止条件
    return ret;
}

//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度
static void GT9147_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    si2c_start();
    si2c_sent_byte(GT_CMD_WR); //发送写命令
    si2c_wait_ack();
    si2c_sent_byte(reg >> 8); //发送高8位地址
    si2c_wait_ack();
    si2c_sent_byte(reg & 0XFF); //发送低8位地址
    si2c_wait_ack();
    si2c_start();
    si2c_sent_byte(GT_CMD_RD); //发送读命令
    si2c_wait_ack();
    for (i = 0; i < len; i++)
    {
        buf[i] = si2c_read_byte(i == (len - 1) ? 0 : 1); //发数据
    }
    si2c_stop(); //产生一个停止条件
}

const uint16_t GT9147_TPX_TBL[5] = {GT_TP1_REG, GT_TP2_REG, GT_TP3_REG, GT_TP4_REG, GT_TP5_REG};
static uint8_t gt9147_scan(TouchTypedef *touch)
{
    uint8_t buf[4];
    uint8_t res = 0;
    uint8_t temp;
    uint8_t temp1;

    GT9147_RD_Reg(GT_GSTID_REG, &temp, 1); //读取触摸点的状态
    if (temp & 0X80 && ((temp & 0XF) < 6))
    {
        temp1 = 0;
        GT9147_WR_Reg(GT_GSTID_REG, &temp1, 1); //清标志
    }

    if ((temp & 0XF) && ((temp & 0XF) < 6))
    {

        for (int i = 0; i < 1; i++)
        {
            uint16_t x = 0;
            uint16_t y = 0;
            GT9147_RD_Reg(GT9147_TPX_TBL[i], buf, 4); //读取XY坐标值
            if (touch->dir == 0)
            {
                x = ((uint16_t)buf[1] << 8) + buf[0];
                y = (((uint16_t)buf[3] << 8) + buf[2]);
            }
            else
            {
                y = ((uint16_t)buf[1] << 8) + buf[0];
                x = touch->pix_h - (((uint16_t)buf[3] << 8) + buf[2]);
            }
            touch->x[i] = x;
            touch->y[i] = y;
        }
        res = 1;
        touch->touch_num = 1;
    }
    else
    {
        touch->touch_num = 0;
        res = 0;
        return 0;
    }
    return res;
}
