#ifndef __ZF_DRIVER_H__
#define __ZF_DRIVER_H__

#include "zf_common.h"
#include "zf_driver_common.h"

#include "zf_lcd_define.h"
enum ZF_DRIVER_TYPE
{
ZF_DRIVER_TYPE_GPIO=0,
ZF_DRIVER_TYPE_UART,
ZF_DRIVER_TYPE_DELAY,
ZF_DRIVER_TYPE_I2C,
ZF_DRIVER_TYPE_SPI,
ZF_DRIVER_TYPE_QSPI,
ZF_DRIVER_TYPE_SDMMC,
ZF_DRIVER_TYPE_LCD_RGB,
ZF_DRIVER_TYPE_LCD_8080,
ZF_DRIVER_TYPE_LCD_8080_STD,
ZF_DRIVER_TYPE_LCD_OLED_STD,
ZF_DRIVER_TYPE_LCD_SPI_STD,
};
// type name
extern char ZF_DRIVER_TYPE_NAME[50][10];
 int get_driver_type_name(int type,char **output);

struct zf_storage_info_t
{
    unsigned long long block_num;
    uint16_t block_size;
    uint8_t card_type;
    uint32_t size;
};


struct zf_hw_t
{   
    // 硬件初始化
    int (*init)(uint8_t ch);
    // 获取通讯之类的状态
    int (*get)(uint8_t ch,uint8_t cmd);
    // 设置清空标志位之类的
    int (*set)(uint8_t ch,uint8_t cmd, uint32_t data);
    // 关闭外设节省电量
    int (*close)(uint8_t ch);
};

// gpio
struct zf_gpio_t 
{   
    struct zf_hw_t hw;
    int (*config)(void *pin_port, uint16_t pin, uint16_t io_mode);
    int (*read)(void *pin_port, uint16_t pin);
    int (*write)(void *pin_port, uint16_t pin, uint16_t value);
};

// uart
struct zf_uart_t 
{   
    struct zf_hw_t hw;
    
    int (*config)(uint8_t ch, uint32_t baud, uint8_t data, uint8_t stop);
    int (*set_rec_callback)(uint8_t ch,  int (*arg_callback)(uint8_t *data,uint16_t len)   );
    int (*read)(uint8_t ch, uint8_t *data, uint16_t len);
    int (*write)(uint8_t ch, uint8_t *data, uint16_t len);
};


struct zf_demo_t 
{   
    struct zf_hw_t hw;
};


// delay
struct zf_delay_t 
{   
    struct zf_hw_t hw;
    int (*config)(void);
    void (*delay_ns)(uint32_t time);
    void (*delay_us)(uint32_t time);
    void (*delay_ms)(uint32_t time);
    void (*delay_s)(uint32_t time);
};

// i2c
struct zf_i2c_t 
{   
    struct zf_hw_t hw;
    int (*config)(uint8_t ch,uint32_t speed);
    int (*read)(uint8_t ch, uint8_t addr, uint8_t  reg_addr, uint8_t *data,uint16_t len );
    int (*write)(uint8_t ch,uint8_t addr, uint8_t  reg_addr, uint8_t *data,uint16_t len );
};

// spi
struct  zf_spi_t 
{   
    struct zf_hw_t hw;
    int (*config)(uint8_t ch,uint32_t speed);
    int (*read)(uint8_t ch,uint8_t *data,uint16_t len);
    int (*write)(uint8_t ch,uint8_t *data,uint16_t len);
    int (*write_read)(uint8_t ch, uint8_t *send_data, uint16_t send_len, uint8_t *read_data, uint16_t read_len);
};

// qspi

struct zf_qspi_t 
{   
    struct zf_hw_t hw;
    int (*config)(uint8_t ch,uint32_t speed);
    int (*read)(uint8_t ch,uint8_t *data,uint32_t len);
    int (*write)(uint8_t ch,uint8_t *data,uint32_t len);
    int (*write_cmd)(uint8_t ch,uint32_t instruction, uint32_t address, uint32_t dummyCycles, uint32_t instructionMode, uint32_t addressMode, uint32_t addressSize, uint32_t dataMode);
};


// sdmmc

struct zf_sdmmc_t 
{   
    struct zf_hw_t hw;
    int (*config)(uint8_t ch,uint32_t speed);
    int (*get_info)(uint8_t ch,uint8_t *card_type, uint16_t *block_size, uint32_t *bloack_num, unsigned long long *size);
    int (*erase_all)(uint8_t ch);
    int (*erase_range)(uint8_t ch,uint32_t addr, uint32_t size);
    int (*read)(uint8_t ch,uint32_t addr, uint8_t *buff, uint32_t size);
    int (*write)(uint8_t ch,uint32_t addr, uint8_t *buff, uint32_t size);
};






// lcd type
struct zf_lcd_t 
{   
    struct zf_hw_t hw;
    int (*config)(uint8_t ch,void *config);
	int (*draw_point)(uint8_t ch,uint16_t x, uint16_t y, uint32_t color);
    
	int (*fill)(uint8_t ch,uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color);
	int (*color_fill)(uint8_t ch,uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);
    int (*flush)(uint8_t ch);
};

// lcd type
struct zf_lcd_8080_t 
{   
    struct zf_hw_t hw;
    int (*config)(uint8_t ch,void *config);
    int (*get_gram_addr)(uint8_t ch,uint32_t *addr);
};



int zf_driver_register(enum ZF_DRIVER_TYPE type,void *input_driver);
int *zf_driver_get(enum ZF_DRIVER_TYPE type,void **output_driver);
int zf_driver_init_hw(void);

#endif
