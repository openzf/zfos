#include "ft5206.h"
#include "zf_si2c.h"

//I2C读写命令
#define FT_CMD_WR 0X70 //写命令
#define FT_CMD_RD 0X71 //读命令

//FT5206 部分寄存器定义
#define FT_DEVIDE_MODE 0x00    //FT5206模式控制寄存器
#define FT_REG_NUM_FINGER 0x02 //触摸状态寄存器

#define FT_TP1_REG 0X03 //第一个触摸点数据地址
#define FT_TP2_REG 0X09 //第二个触摸点数据地址
#define FT_TP3_REG 0X0F //第三个触摸点数据地址
#define FT_TP4_REG 0X15 //第四个触摸点数据地址
#define FT_TP5_REG 0X1B //第五个触摸点数据地址

#define FT_ID_G_LIB_VERSION 0xA1  //版本
#define FT_ID_G_MODE 0xA4         //FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP 0x80      //触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE 0x88 //激活状态周期设置寄存器

static const uint16_t FT5206_TPX_TBL[5] = {FT_TP1_REG, FT_TP2_REG, FT_TP3_REG, FT_TP4_REG, FT_TP5_REG};

static uint8_t FT5206_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    si2c_start();
    si2c_sent_byte(FT_CMD_WR); //发送写命令
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
//从FT5206读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度
static void FT5206_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    si2c_start();
    si2c_sent_byte(FT_CMD_WR); //发送写命令
    si2c_wait_ack();
    si2c_sent_byte(reg & 0XFF); //发送低8位地址
    si2c_wait_ack();
    si2c_start();
    si2c_sent_byte(FT_CMD_RD); //发送读命令
    si2c_wait_ack();
    for (i = 0; i < len; i++)
    {
        buf[i] = si2c_read_byte(i == (len - 1) ? 0 : 1); //发数据
    }
    si2c_stop(); //产生一个停止条件
}

// 扫描
uint8_t ft5206_scan(TouchTypedef *touch)
{
    uint8_t buf[4];
    uint8_t res = 0;
    uint8_t temp = 0;
    FT5206_RD_Reg(FT_REG_NUM_FINGER, &temp, 1); //读取触摸点的状态
    touch->touch_num = 0;
    if ((temp & 0XF) && ((temp & 0XF) < 6))
    {
        for (int i = 0; i < temp; i++)
        {
            FT5206_RD_Reg(FT5206_TPX_TBL[i], buf, 4);
            uint16_t x = 0;
            uint16_t y = 0;

            // 横屏
            if (touch->dir == 0)
            {
                y = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
                if (touch->lcd_id == 123)
                { // ���id��9148
                    x = touch->pix_w - (((uint16_t)(buf[2] & 0X0F) << 8) + buf[3]);
                }
                else
                {
                    x = (((uint16_t)(buf[2] & 0X0F) << 8) + buf[3]);
                }
            }
            else
            {
                x = touch->pix_h - (((uint16_t)(buf[0] & 0X0F) << 8) + buf[1]);
                y = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
            }
            touch->x[i] = x;
            touch->y[i] = y;
        }
        touch->touch_num = 1;
        res = 1;
    }
    else
    {
        touch->touch_num = 0;
        res = 0;
        return 0;
    }
    return res;
}

void device_ft5206_init(void)
{
}

int ft5206_init(uint8_t (**arg_scan)(TouchTypedef *touch))
{
    uint8_t temp[2];

    *arg_scan = ft5206_scan;
    temp[0] = 0;
    FT5206_WR_Reg(FT_DEVIDE_MODE, temp, 1);  //进入正常操作模式
    FT5206_WR_Reg(FT_ID_G_MODE, temp, 1);    //查询模式
    temp[0] = 22;                            //触摸有效值，22，越小越灵敏
    FT5206_WR_Reg(FT_ID_G_THGROUP, temp, 1); //设置触摸有效值
    temp[0] = 12;                            //激活周期，不能小于12，最大14
    FT5206_WR_Reg(FT_ID_G_PERIODACTIVE, temp, 1);
    //读取版本号，参考值：0x3003
    FT5206_RD_Reg(FT_ID_G_LIB_VERSION, &temp[0], 2);
    if ((temp[0] == 0X30 && temp[1] == 0X03) || temp[1] == 0X01 || temp[1] == 0X02) //版本:0X3003/0X0001/0X0002
    {
        printf("CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
        return 0;
    }
    printf("CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
    return 1;
}
