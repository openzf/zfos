#include "zf_driver.h"

#define ZF_DRIVER_MAX_NUM 20
int *zf_driver_list[ZF_DRIVER_MAX_NUM];
int *zf_driver_misc_list[ZF_DRIVER_MAX_NUM];

// 注册驱动
/**
 * @brief 
 * 
 * @param type 
 * @param input_driver 
 * @return int 
 */
int zf_driver_register(enum ZF_DRIVER_TYPE type, void *input_driver)
{
    if (type > 128)
    {
        zf_driver_misc_list[type] = (int *)input_driver;
    }
    else
    {
        zf_driver_list[type] = (int *)input_driver;
    }
		return 0;
}

// 获取驱动
/**
 * @brief 
 * 
 * @param type 
 * @param output_driver 
 * @return int* 
 */
int *zf_driver_get(enum ZF_DRIVER_TYPE type, void **output_driver)
{
    if (type > 128)
    {
        *output_driver = (void *)zf_driver_misc_list[type];
    }
    else
    {
        *output_driver = (void *)zf_driver_list[type];
    }
    return 0;
}

// 检查驱动并且初始化
/**
 * @brief 
 * 
 * @return int 
 */
int zf_driver_init_hw(void)
{
    // 初始化
    int i =0;;
    for(i=0;i<ZF_DRIVER_MAX_NUM;i++)
    {
        if(zf_driver_list[i]!=0)
        {
					struct zf_demo_t  *zf_demo = (void *)zf_driver_list[i];
					  zf_demo->hw.init(0);
            printf("zf_driver %d has init\r\n",i);
        }else{
					 
		}
    }
		
	return 0;
}


